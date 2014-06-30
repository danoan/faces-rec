#include "../headers/trainer.h"

void Trainer::inputInfo(){
    Logger::logger->log("INPUT INFO\n\n");
    Logger::logger->log("ARDIS_WIDTH: %d\nARDIS_HEIGHT: %d\n",_ardis.x,_ardis.y);
    Logger::logger->log("SHIFT_STEP: %d\nRESIZE_FACTOR: %f\nSTART_WIDTH: %d\nSTART_HEIGHT: %d\n",_shift_w,_resize_factor,_start_w,_start_h);
    Logger::logger->log("FINAL_FALSE_POSITIVE_RATE: %f\nFINAL_DETECTION_RATE: %f\n",_final_fp_rate,_final_det_rate);
    Logger::logger->log("STAGE_MAX_FALSE_POSITIVE_RATE: %f\nSTAGE_MIN_DETECTION_RATE: %f\n",_max_fp_rate,_min_det_rate);
    Logger::logger->log("MAX_STATES: %d\nBETA_MIN_VALUE: %f\n\n",Config::CLASSIFIER_MAX_STAGES, Config::CLASSIFIER_BETA_MIN_VALUE);

    Logger::logger->log("FEATURES INFO\n\n");

    Logger::logger->log("Total Features: %d\n",_facesFactory._facesFeatures.size());    
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

    _ts = ts;
    _vs = vs;

    _tir.init(_facesFactory._facesFeatures.size());

    for(register int j=0;j<_ts._faces.size();j++){
        _tir.addFace( _ts._faces[j], FACE );
    }    

    inputInfo();
}

void Trainer::prepareTrainer(){
    printf("PREPARE TRAINER\n");

    _tir.clearScenes();
    for(register int j=0;j<_ts._scenes.size();j++){
        _tir.addScene( _ts._scenes[j], SCENE );
    }    
    printf("END TRAINER\n");

    for(int i=0;i<THREADS_NUMBER;i++) _ct[i] = new ClassificationTable();        
}

Classifier Trainer::startTraining(){
    for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->initTable(_tir);
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

    int total_features = 0;
    while( _fp_rate>_final_fp_rate && _stage_number < Config::CLASSIFIER_MAX_STAGES ){          
        prepareTrainer(); 
        Logger::debug->log("End Preparation \n\n");
        
        for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->initTable(_tir);

        Logger::debug->log("Start Training Stage %d\n\n", _stage_number);
        _feature_number=0;

        Classifier fc;    
        startClock();       
        keepTraining(fc);
        stopClock("KEEP TRAINING");

        if(_stage_number<5){
            while(!firstStagesCheckClassifier(fc,&ac,&fi,&di,_stage_number,_feature_number)){
                startClock();       
                keepTraining(fc);
                stopClock("KEEP TRAINING");
            }
        }else{
            while(!checkClassifier(fc,&ac,&fi,&di) && _feature_number < Config::CLASSIFIER_MAX_HYPOTHESIS_PER_STAGE){
                startClock();       
                keepTraining(fc);
                stopClock("KEEP TRAINING");
            }            
        }


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

        if(_ts.resetScenesSet(cascade,_vs)==-1) break;                
        if(_vs.resetScenesSet()==-1) break;

        _stage_number++; 	

        endTrainer();       
    }
    

    return cascade;
}

void* getBestFromFeature(void* params){
    // printf("INIT\n");
    elem_params* ep = (elem_params*) params;

    int from = ep->factor*ep->thread_number;
    int to = ep->factor*(ep->thread_number+1);

    if(ep->final==1){
        to = ep->t->_facesFactory._facesFeatures.size()-1;
    }

    // printf("OK %d %d %d %d\n", from, to, ep->factor, ep->t->_featureMasks.size());    

    TableItem partialBest;
    for(int i=from;i<to;i++){
        // printf("GO %d\n", i);
        partialBest = ep->t->_ct[ep->thread_number]->getBestTableItem( (ep->t->_facesFactory._facesFeatures)[i], ep->t->_tir );
        // printf("STOP\n");

        if(partialBest._error<ep->best._error){
            ep->best = partialBest;
        }   
    }

    // printf("THE END\n");

    pthread_exit(NULL);
}


void Trainer::keepTraining(Classifier& cl){
    Logger::debug->log("FEATURE %d\n\n", _feature_number);

    elem_params ep[THREADS_NUMBER];

    int factor = (int) floor(_facesFactory._facesFeatures.size()/THREADS_NUMBER);

    for(register int t=0;t<THREADS_NUMBER;t++){
        ep[t].t = this;
        ep[t].thread_number = t;
        ep[t].factor = factor;
        ep[t].best = TableItem(0,0,FACE,1.0,1,-1);
        ep[t].final = (t==4?1:0);

        pthread_create(&(_threads[t]), NULL, getBestFromFeature, &ep[t]);
    }

    for(register int t=0;t<THREADS_NUMBER;t++){
        pthread_join(_threads[t],NULL);
    }

    TableItem theBest = ep[0].best;
    for(register int t=1;t<THREADS_NUMBER;t++){
        if(ep[t].best._error < theBest._error){
            theBest = ep[t].best;
        }
    }

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
    Logger::debug->log("HYPOTHESY %lu %d %lf %d \n",h._threshold,h._direction,h._alpha,h._fm._id);

    cl.addHypothesy(h);
    cl._ardis = _ardis;

    for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->updateWeights(b_t,h,_tir);
    
    _feature_number+=1;
}

bool Trainer::firstStagesCheckClassifier(Classifier& cc, double* ac, double* fi, double* di, int stage, int featureNumber){
    std::vector<TID>::iterator it;

    double rate_fp;
    double rate_det;

    *ac = 0.8;
    while( *ac>=0.20){
        *ac-=0.1;
        rate_fp=0;

        for(it=_vs._scenes.begin();it!=_vs._scenes.end();it++){
            IntegralImage ii(it->_img_path);
            rate_fp += cc.isFace(ii,*ac);
        }

        rate_fp = rate_fp/_vs._scenes.size();
        Logger::debug->log("RATE FP: %.4f\n",rate_fp);

        if(rate_fp>0.75) break;    //I have to put more features in the classifier        

        rate_det=0;
        for(it=_vs._faces.begin();it!=_vs._faces.end();it++){
            IntegralImage ii(it->_img_path);
            rate_det += cc.isFace(ii,*ac);
        }    

        rate_det = rate_det/_vs._faces.size();
        Logger::debug->log("RATE DET: %.4f (MIN:%.4f)\n",rate_det,_min_det_rate);

        *fi = rate_fp;
        *di = rate_det;        

        if(rate_det<_min_det_rate) continue;

        return true;
    }

    if( featureNumber < _firstStagesMaxFeature[stage]){
        return false;
    }else{
        return true;        
    }

}

bool Trainer::checkClassifier(Classifier& cc, double* ac, double* fi, double* di){
    std::vector<TID>::iterator it;

    double rate_fp;
    double rate_det;

    *ac = 0.525;
    while( *ac>=0.25){
        *ac-=0.025;
        rate_fp=0;

        for(it=_vs._scenes.begin();it!=_vs._scenes.end();it++){
            IntegralImage ii(it->_img_path);
            rate_fp += cc.isFace(ii,*ac);
        }

        rate_fp = rate_fp/_vs._scenes.size();
        Logger::debug->log("RATE FP: %.4f\n",rate_fp);

        if(rate_fp>_max_fp_rate) return false;    //I have to put more features in the classifier

        rate_det=0;
        for(it=_vs._faces.begin();it!=_vs._faces.end();it++){
            IntegralImage ii(it->_img_path);
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
