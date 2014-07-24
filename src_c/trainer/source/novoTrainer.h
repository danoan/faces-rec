CascadeClassifier Trainer::startTrainingCascade(){    
    CascadeClassifier cascade;
    double _fp_rate = 1.0;
    double _det_rate = 1.0;

    double ac;
    double fi;
    double di;

    int total_features = 0;
    int r;
    while( _fp_rate>_final_fp_rate && _stage_number < Config::CLASSIFIER_MAX_STAGES ){          
        prepareTrainer(); 

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
        sprintf(path,"%s/classifier_%d_%d_%d",Config::STATES_PATH.c_str(), _tsm->trs()._faces.size()+_tsm->vas()._scenes.size(), _stage_number, total_features);
        printf("%s\n",path);
        cascade.save(std::string(path));

        _stage_number++;    
        r = _tsm->resetSets(_stage_number,cascade);
        printf("END RESET\n");
        
        endTrainer();    
        if(r==-1) break;   
    }
    

    return cascade;
}

void Trainer::keepTraining(Classifier& cl){
    Logger::debug->log("FEATURE %d\n\n", _feature_number);


    bool isAllFeaturesCovered = false;    
    TableItem theBest = TableItem(0,0,FACE,1.0,1);
    while(!isAllFeaturesCovered){
        GPUBuffer* gpb = gpuManager.getFilledBuffer(&isAllFeaturesCovered);    

        if(gpb==NULL) exit(1);

        TableItem partial = cpuManager.consumeBuffer(gpb);
        if(partial._error < theBest._error) theBest = partial;
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

    for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->updateWeights(b_t,h,_tsm->trs());
    
    _feature_number+=1;
}