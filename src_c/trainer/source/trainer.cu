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

Trainer::Trainer(TrainerSetManager* tsm){
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

    _tsm = tsm;
    _tsm->init(_facesFactory._facesFeatures.size(),Config::CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE);
    
    _stage_number = 0;
    _feature_number = 0;

    inputInfo();
    
	_firstStagesMaxFeature[0] = 2;
	_firstStagesMaxFeature[1] = 5;
	_firstStagesMaxFeature[2] = 10;
	_firstStagesMaxFeature[3] = 25;
	_firstStagesMaxFeature[4] = 50;
	
	gpuManager = new GPUManager( _tsm->trs(), _facesFactory._facesFeatures.size(), tsm->trs().size() );
	cpuManager = new CPUManager();
}

void Trainer::prepareTrainer(){
    for(int i=0;i<CPU_THREADS;i++) _ct[i] = new ClassificationTable();        
    for(int i=0;i<CPU_THREADS;i++) _ct[i]->initTable(_tsm->trs());
}

Classifier Trainer::startTraining(){
    prepareTrainer();
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
        gpuManager->resetImageData(_tsm->trs());
        if(r==-1) break;   
    }
    

    return cascade;
}


void Trainer::keepTraining(Classifier& cl){
    Logger::debug->log("FEATURE %d\n\n", _feature_number);

    bool isAllFeaturesCovered = false;    
    TableItem theBest = TableItem(0,0,FACE,1.0,1);
    gpuManager->restart();

    while(!gpuManager->END_STAGE){
        GPUBuffer* gpb = gpuManager->getFilledBuffer();    

        if(gpb!=NULL){
			printf("CONSUME BUFFER\n");
			TableItem partial = cpuManager->consumeBuffer(gpb,gpuManager,this);
			if(partial._error < theBest._error) theBest = partial;
		}
    }
    
    gpuManager->wait();
    
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

    for(int i=0;i<CPU_THREADS;i++) _ct[i]->updateWeights(b_t,h,_tsm->trs());
    gpuManager->resetManager();
    
    _feature_number+=1;
}

bool Trainer::firstStagesCheckClassifier(Classifier& cc, double* ac, double* fi, double* di, int stage, int featureNumber){
    bool check = _tsm->checkClassifier(cc,ac,fi,di,0.75,_min_det_rate,0.4,0.025,0.1);
    if(check==false){
         if( featureNumber >= _firstStagesMaxFeature[stage] ) return true;
    }

    return check;
}

bool Trainer::checkClassifier(Classifier& cc, double* ac, double* fi, double* di){
    return _tsm->checkClassifier(cc,ac,fi,di,_max_fp_rate,_min_det_rate,0.525,0.025,0.25);
}
