#include "../../headers/classifier/trainer.h"

ulong countFeatures(FMF factory){
    ulong c=0;

    while(factory.hasNext()==1){
        factory.next();
        c++;
    }
    factory.restart();

    return c;
}

void Trainer::inputInfo(){
    Logger::logger->log("INPUT INFO\n\n");
    Logger::logger->log("ARDIS_WIDTH: %d\nARDIS_HEIGHT: %d\n",_ardis.x,_ardis.y);
    Logger::logger->log("SHIFT_STEP: %d\nRESIZE_FACTOR: %f\nSTART_WIDTH: %d\nSTART_HEIGHT: %d\n",_shift_w,_resize_factor,_start_w,_start_h);
    Logger::logger->log("FINAL_FALSE_POSITIVE_RATE: %f\nFINAL_DETECTION_RATE: %f\n",_final_fp_rate,_final_det_rate);
    Logger::logger->log("STAGE_MAX_FALSE_POSITIVE_RATE: %f\nSTAGE_MIN_DETECTION_RATE: %f\n",_max_fp_rate,_min_det_rate);
    Logger::logger->log("MAX_STATES: %d\nBETA_MIN_VALUE: %f\nMAX_VALIDATION_SET: %d\n\n",Config::CLASSIFIER_MAX_STAGES, Config::CLASSIFIER_BETA_MIN_VALUE, Config::MAX_LENGHT_VALIDATION_SET);

    Logger::logger->log("FEATURES INFO\n\n");

    int cm2h = countFeatures(_m2hf);
    int cm2v = countFeatures(_m2vf);
    int cm3h = countFeatures(_m3hf);
    int cm3v = countFeatures(_m3vf);
    int cd = countFeatures(_mdf);

    FMF::resetCounter(); //Reset the FeatureMask id counter

    Logger::logger->log("Two Horizontal: %d\n",cm2h);
    Logger::logger->log("Two Vertical: %d\n",cm2v);
    Logger::logger->log("Three Horizontal: %d\n",cm3h);
    Logger::logger->log("Three Vertical: %d\n",cm3v);
    Logger::logger->log("Diagonal: %d\n",cd);    
    Logger::logger->log("Total Features: %d\n",cm2h+cm2v+cm3h+cm3v+cd);    
}

Trainer::Trainer(TrainingSet& ts, ValidationSet& vs){
    _ardis.x = Config::ARDIS_WIDTH; _ardis.y = Config::ARDIS_HEIGHT;

    _shift_w = Config::CLASSIFIER_SHIFT_STEP;
    _shift_h = Config::CLASSIFIER_SHIFT_STEP;

    _resize_factor = Config::CLASSIFIER_RESIZE_FACTOR;
    _start_w = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    _start_h = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;    

    _final_fp_rate = Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE;
    _final_det_rate = Config::CLASSIFIER_FINAL_DETECTION_RATE;

    _max_fp_rate = Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE;
    _min_det_rate = Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE;    


    _m2hf = MaskTwoHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m2vf = MaskTwoVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m3hf = MaskThreeHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m3vf = MaskThreeVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _mdf = MaskDiagonalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);        


    _ts = ts;
    _vs = vs;

    inputInfo();
}

void Trainer::prepareTrainer(){
    _ct = new ClassificationTable();

    for(register int j=0;j<_ts._faces.size();j++){
        addTrainingImage( _ts._faces[j], FACE );
    }

    for(register int j=0;j<_ts._scenes.size();j++){
        addTrainingImage( _ts._scenes[j], SCENE );
    }    
}

void Trainer::addTrainingImage(std::string imagePath, TrainingType imageType){
    _ct->addTrainingImage( imagePath,imageType );
}

void Trainer::getBestFromFeature(TableItem& theBest, FMF& factory){
    FeatureMask fm;    
    int i=0;
    while( factory.hasNext()==1 ){
        fm=factory.next();
        // printf("GET BEST %d\n",fm._id);

        TableItem partialBest = _ct->getBestTableItem(fm);
        if(partialBest._error<theBest._error){
            theBest = partialBest;
        }
        i++;
    }
    factory.restart();
}

Classifier Trainer::startTraining(){
    _ct->initTable();
    Logger::debug->log("Start Training Stage %d\n\n", _stage_number++);
    _feature_number=0;

    Classifier fc;   
    keepTraining(fc);

    return fc;
}

CascadeClassifier Trainer::startTrainingCascade(){    
    CascadeClassifier cascade;
    double _fp_rate = 1.0;
    double _det_rate = 1.0;

    double ac;
    double fi;
    double di;

    int features_to_check = 1;  //Number of features to create before check classifier
    int total_features = 0;
    while( _fp_rate>_final_fp_rate && _stage_number < Config::CLASSIFIER_MAX_STAGES ){          
        prepareTrainer(); 
        Logger::debug->log("End Preparation \n\n");
        
        _ct->initTable();

        Logger::debug->log("Start Training Stage %d\n\n", _stage_number++);
        _feature_number=0;

        Classifier fc;    
        startClock();       
        keepTraining(fc);
        stopClock("KEEP TRAINING");

        while(!checkClassifier(fc,&ac,&fi,&di) && _feature_number < Config::MAX_HYPOTHESIS_PER_STAGE){
            for(int i=0;i<features_to_check;i++){
                startClock();       
                keepTraining(fc);
                stopClock("KEEP TRAINING");
            }                        
        }
        features_to_check *= 1.5;    //*_stage_number*4

        _fp_rate*=fi;
        _det_rate*=di;

        Logger::debug->log("FINAL F: %.4f\nFINAL D: %.4f\n",_fp_rate,_det_rate);

        fc._ac = ac;
        cascade.addClassifier(fc);

        char path[128];
        total_features += _feature_number;
        sprintf(path,"%s/classifier_%d_%d_%d",Config::STATES_PATH.c_str(), _ts._faces.size()+_ts._scenes.size(), _stage_number, total_features);
        printf("%s\n",path);
        cascade.save(std::string(path));

        if(_ts.resetScenesSet(fc,_vs,_stage_number)==-1) break;                
        if(_vs.resetScenesSet(_stage_number)==-1) break;  

        endTrainer();       
    }
    

    return cascade;
}

void Trainer::keepTraining(Classifier& cl){
    Logger::debug->log("FEATURE %d\n\n", _feature_number);

    TableItem theBest(0,0,FACE,1.0,1,-1);

    getBestFromFeature(theBest,_m2hf);
    getBestFromFeature(theBest,_m2vf);
    getBestFromFeature(theBest,_m3hf);
    getBestFromFeature(theBest,_m3vf);
    getBestFromFeature(theBest,_mdf);

    FMF::resetCounter(); //Reset the FeatureMask id counter

    double e_t = theBest._error;
    double b_t = (e_t)/(1-e_t);
    double a_t = 0;

    if (b_t <= Config::CLASSIFIER_BETA_MIN_VALUE){
        a_t = log( 1.0/(Config::CLASSIFIER_BETA_MIN_VALUE) );
    }else{
        a_t = log( 1.0/b_t );
    }
    Logger::debug->log( "IT: %d-%d \n E_T: %.12f \n B_T: %.12f \n A_T %.12f \n\n",_stage_number,_feature_number,e_t,b_t,a_t);

    Hypothesy h (theBest._filter_value, theBest._direction, a_t, theBest._fm );
    Logger::debug->log("HYPOTHESY %lu %d %lf\n",h._threshold,h._direction,h._alpha);

    cl.addHypothesy(h);
    cl._ardis = _ardis;
    _ct->updateWeights(b_t,h);
    _feature_number+=1;
}

bool Trainer::checkClassifier(Classifier& cc, double* ac, double* fi, double* di){
    std::vector<std::string>::iterator it;

    double rate_fp;
    double rate_det;

    *ac = 0.525;
    while( *ac>=0.25){
        *ac-=0.025;
        rate_fp=0;

        for(it=_vs._scenes.begin();it!=_vs._scenes.end();it++){
            IntegralImage ii(*it);
            rate_fp += cc.isFace(ii,*ac);
        }

        rate_fp = rate_fp/_vs._scenes.size();
        Logger::debug->log("RATE FP: %.4f\n",rate_fp);

        if(rate_fp>_max_fp_rate) return false;    //I have to put more features in the classifier

        rate_det=0;
        for(it=_vs._faces.begin();it!=_vs._faces.end();it++){
            IntegralImage ii(*it);
            rate_det += cc.isFace(ii,*ac);
        }    

        rate_det = rate_det/_vs._faces.size();
        Logger::debug->log("RATE DET: %.4f (MIN:%.4f)\n",rate_det,_min_det_rate);

        if(rate_det<_min_det_rate) continue;

        *fi = rate_fp;
        *di = rate_det;
        return true;
    }
    
    return false;
}
