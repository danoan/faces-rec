#include "../../headers/classifier/trainer.h"

Trainer::Trainer(TrainingSet& ts, ValidationSet& vs){
    _ardis.x = 64; _ardis.y = 64;
    _shift_w = 2;
    _shift_h = 2;
    _resize_factor = 1.25;
    _start_w = 8;
    _start_h = 8;

    _m2hf = MaskTwoHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m2vf = MaskTwoVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m3hf = MaskThreeHorizontalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _m3vf = MaskThreeVerticalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    _mdf = MaskDiagonalFactory(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);        


    _ts = ts;
    _vs = vs;
}

void Trainer::prepareTrainer(){
    _ct = new ClassificationTable();

    for(register int j=0;j<_ts._faces.size();j++){
        addTrainingImage( _ts._faces[j], FACE );
    }

    for(register int j=0;j<_ts._scenes.size();j++){
        addTrainingImage( _ts._scenes[j], SCENE);
    }    
}

void Trainer::addTrainingImage(std::string imagePath, TrainingType imageType ){
    _ct->addTrainingImage( imagePath,imageType );
}

void Trainer::getBestFromFeature(TableItem& theBest, FMF& factory){
    FeatureMask fm;    
    int i=0;
    while( factory.hasNext()==1 ){
        fm=factory.next();
        TableItem partialBest = _ct->getBestTableItem(fm);
        if(partialBest._error<theBest._error){
            theBest = partialBest;
        }
        i++;
    }
    // printf("%d\n",i);
    factory.restart();
}

Classifier Trainer::startTraining(){
    _ct->initTable();
    printf("Start Training Stage %d\n\n", _stage_number++);
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

    
    while( _fp_rate>_final_fp_rate ){    
        prepareTrainer();    
        _ct->initTable();

        printf("Start Training Stage %d\n\n", _stage_number++);
        _feature_number=0;

        Classifier fc;           
        keepTraining(fc);

        while(!checkClassifier(fc,&ac,&fi,&di)){
            keepTraining(fc);            
        }

        _fp_rate*=fi;
        _det_rate*=di;

        printf("FINAL F: %.4f\n",_fp_rate);
        printf("FINAL D: %.4f\n",_det_rate);

        fc._ac = ac;
        cascade.addClassifier(fc);

        if(_ts.resetScenesSet(fc,_vs)==-1) break;                
    }
    

    return cascade;
}

void Trainer::keepTraining(Classifier& cl){
    printf("FEATURE %d\n\n", _feature_number);

    TableItem theBest(0,0,FACE,1.0,1,-1);

    getBestFromFeature(theBest,_m2hf);
    getBestFromFeature(theBest,_m2vf);
    getBestFromFeature(theBest,_m3hf);
    getBestFromFeature(theBest,_m3vf);
    getBestFromFeature(theBest,_mdf);


    double e_t = theBest._error;
    double b_t = (e_t)/(1-e_t);
    double a_t = 0;

    if (b_t <= 1e-8){
        a_t = log( 1.0/(1e-8) );
    }else{
        a_t = log( 1.0/b_t );
    }
    printf( "IT: %d \n E_T: %.12f \n B_T: %.12f \n A_T %.12f \n", 0,e_t,b_t,a_t);

    Hypothesy h (theBest._filter_value, theBest._direction, a_t, theBest._fm );
    printf("HYPOTHESY %lu %d %lf\n",h._threshold,h._direction,h._alpha);

    cl.addHypothesy(h);
    _ct->updateWeights(b_t,h);
    _feature_number+=1;
}

bool Trainer::checkClassifier(Classifier& cc, double* ac, double* fi, double* di){
    std::vector<std::string>::iterator it;
    
    Point wr;
    wr.x=64;
    wr.y=64;

    Subwindow sw (0,0,wr,1,1);    

    double rate_fp;
    double rate_det;

    *ac = 0.8;
    while( *ac>=0.3){
        *ac-=0.1;
        rate_fp=0;

        for(it=_vs._scenes.begin();it!=_vs._scenes.end();it++){
            IntegralImage ii(*it);
            rate_fp += cc.isFace(ii,sw,*ac);
        }

        rate_fp = rate_fp/_vs._scenes.size();
        printf("RATE FP: %.4f\n",rate_fp);

        if(rate_fp>_max_fp_rate) return false;    //I have to put more features in the classifier

        rate_det=0;
        for(it=_vs._faces.begin();it!=_vs._faces.end();it++){
            IntegralImage ii(*it);
            rate_det += cc.isFace(ii,sw,*ac);
        }    

        rate_det = rate_det/_vs._faces.size();
        printf("RATE DET: %.4f\n",rate_det);

        if(rate_det<_min_det_rate) continue;

        *fi = rate_fp;
        *di = rate_det;
        return true;
    }
    
    return false;
}
