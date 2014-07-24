#include "../headers/trainer.h"

FeatureMaskDev convertFeatureMask(FeatureMask fm){
	FeatureMaskDev fmd;
	fmd._mask_size = fm._mask._size;
	fmd._location = fm._location;
	fmd._white_length = fm._mask._white.size();
	fmd._black_length = fm._mask._black.size();
	fmd._id = fm._id;
	
	for(int i=0;i<fmd._white_length;i++){
		fmd._white[i]._points[0] = fm._mask._white[i]._points[0];
		fmd._white[i]._points[1] = fm._mask._white[i]._points[1];
		fmd._white[i]._points[2] = fm._mask._white[i]._points[2];
		fmd._white[i]._points[3] = fm._mask._white[i]._points[3];
		
		fmd._white[i]._w = fm._mask._white[i]._w;
		fmd._white[i]._h = fm._mask._white[i]._h;
	}
	
	for(int i=0;i<fmd._black_length;i++){
		fmd._black[i]._points[0] = fm._mask._black[i]._points[0];
		fmd._black[i]._points[1] = fm._mask._black[i]._points[1];
		fmd._black[i]._points[2] = fm._mask._black[i]._points[2];
		fmd._black[i]._points[3] = fm._mask._black[i]._points[3];
		
		fmd._black[i]._w = fm._mask._black[i]._w;
		fmd._black[i]._h = fm._mask._black[i]._h;
	}	
	
	return fmd;
}

CUDA_CALLABLE_MEMBER_DEVICE long IntegralImageDev::getFromData(Point* points){
	long sum[4]={0,0,0,0};

	for(int i=0;i<4;++i){
		Point p = points[i];
		if(outsideLimits(p)){
			sum[i] = 0;
		}else{
			sum[i] = _data[p.y*24+p.x];
		}
	}

	return (sum[0]+sum[3]-sum[2]-sum[1]);	
}   

CUDA_CALLABLE_MEMBER_DEVICE long IntegralImageDev::computeBlock(MaskBlockDev* b, ulong mask_length, Point location){
	long block_sum = 0;

	for(int i=0;i<mask_length;++i){
		Point pa,pb,pc,pd;

		pa.y = location.y + b[i]._points[0].y - 1;
		pa.x = location.x + b[i]._points[0].x - 1;

		pb.y = location.y + b[i]._points[1].y - 1;
		pb.x = location.x + b[i]._points[1].x;

		pc.y = location.y + b[i]._points[2].y;
		pc.x = location.x + b[i]._points[2].x - 1;

		pd.y = location.y + b[i]._points[3].y;
		pd.x = location.x + b[i]._points[3].x;						


		Point points[4] = {pa,pb,pc,pd};

		// printf("%ld (%lu %lu) [ (%lu %lu) (%lu %lu) (%lu %lu) (%lu %lu) ]\n",getFromData(points),location.x,location.y,pa.y,pa.x,pb.y,pb.x,pc.y,pc.x,pd.y,pd.x);
		// printf("%ld (%lu %lu)",b[i]._points[0].y,b[i]._points[0].x);
		
		block_sum+=getFromData(points);		
	}

	return block_sum;	
}

CUDA_CALLABLE_MEMBER_DEVICE ulong IntegralImageDev::filter(FeatureMaskDev* fm){
	if( (fm->_mask_size.x > _size.x) || (fm->_mask_size.y > _size.y) ){
		return 120;
	}

	// printf("%d\n", abs( computeBlock(fm._mask._black,fm._location)  - computeBlock(fm._mask._white,fm._location) ) );

	return abs( computeBlock(fm->_black,fm->_black_length,fm->_location)  - computeBlock(fm->_white,fm->_white_length,fm->_location) );	
}

void loadImage(ulong* data, const char* filepath,Point* size){
	MagickWand * image_wand;
	PixelIterator* iterator;
	MagickBooleanType status;
	MagickPixelPacket pixel;
	PixelWand** pixels;

	long x,y;
	ulong line_sum;

	image_wand = NewMagickWand();
	status=MagickReadImage(image_wand,filepath);	

	iterator=NewPixelIterator(image_wand);

	size->y = MagickGetImageHeight(image_wand);
	size->x = MagickGetImageWidth(image_wand);

	// printf("MAGICK %lu %lu\n",size->y,size->x);

	if(data==NULL){
		return;
	}

	for (y=0; y < size->y; ++y){		
	    pixels=PixelGetNextIteratorRow(iterator,&(size->x));

	    if ( (pixels == (PixelWand **) NULL) ) break;

	    line_sum=0;
	    for (x=0; x < (long) size->x; ++x){
	    	line_sum+= PixelGetBlue(pixels[x])*255;

	    	if(y>0){
	    		data[y*size->x+x] = data[(y-1)*size->x+x] + line_sum;
	    	}else{
	    		data[y*size->x+x] = line_sum;
	    	}
	
	    	// printf("%lu\n",(*data)[y][x]);
    	}	
    }
    DestroyPixelIterator(iterator);
    DestroyMagickWand(image_wand);	    
}

void prepareData(TrainingSet& ts, Point* size_image, ulong** data_host, ulong** data_device, int nfeatures, FeatureMaskDev** fmd_host, FeatureMaskDev** fmd_device, int sizeAnswer, ulong** answer_host, ulong** answer_device, ulong** answer_host_buffer, ulong** answer_device_buffer,  bool alloc){
	int ardis_w = 24;
	int ardis_h = 24;
	int nimages = ts.size();
	int totalPixels = nimages*ardis_w*ardis_h;
	
	//Preparing Images	
	if(alloc){
		(*data_host) = (ulong*) malloc(sizeof(ulong)*totalPixels);
		cudaMalloc( (void**) data_device, sizeof(ulong)*totalPixels );	
	}
	
	
	for(int i=0;i<nimages;i++){		
		TrainingImage* ti = ts.get(i);
		for(int h=0;h<ardis_h;h++){
			for(int w=0;w<ardis_w;w++){	
				( (*data_host)+(i*ardis_w*ardis_h) )[h*ardis_w+w] = ti->_ii->_data[h][w];
			}
		}
	}	
	/*
	for(int i=0;i<totalPixels;i++){
		printf("%d\n", (*data_host)[i] );
	}
	*/	
	
	cudaMemcpy( *data_device, *data_host, sizeof(ulong)*totalPixels, cudaMemcpyHostToDevice );	
	
		
	//Preparing Features
	FacesFeatureFactory fff;
	
	if(alloc) (*fmd_host) = (FeatureMaskDev*) malloc(sizeof(FeatureMaskDev)*nfeatures);
	
	for(int i=0;i<nfeatures;i++){
		(*fmd_host)[i] = convertFeatureMask(fff._facesFeatures[i]);
	}
	
	if(alloc) cudaMalloc( (void**) fmd_device, sizeof(FeatureMaskDev)*nfeatures );
	
	cudaMemcpy( (*fmd_device), (*fmd_host), sizeof(FeatureMaskDev)*nfeatures, cudaMemcpyHostToDevice );
		
	//Preparing Answer	
	if(alloc){
		(*answer_host) = (ulong*) malloc(sizeof(ulong)*sizeAnswer);
		(*answer_host_buffer) = (ulong*) malloc(sizeof(ulong)*sizeAnswer);
		
		cudaMalloc( (void**) answer_device, sizeof(ulong)*sizeAnswer );
		cudaMalloc( (void**) answer_device_buffer, sizeof(ulong)*sizeAnswer );
	}
}

__global__ void kernelTestFilter(ulong* data, Point size, int nimages, FeatureMaskDev* fmd,int startFeature, int stepFeature, int nfeatures, ulong* answer){	

	/*
	int factorF = stepFeature/712 + 1;
	int featDe = threadIdx.x*factorF;
	int featAte = (threadIdx.x+1)*factorF;
	
	int factorI = nimages/16000 + 1;
	int imgDe =  blockIdx.x*factorI;
	int imgAte = (blockIdx.x+1)*factorI;
	*/
	
	int factorF = stepFeature/16000 + 1;
	int featDe = blockIdx.x*factorF;
	int featAte = (blockIdx.x+1)*factorF;
	
	int factorI = nimages/712 + 1;
	int imgDe =  threadIdx.x*factorI;
	int imgAte = (threadIdx.x+1)*factorI;	
	
	if(featDe < stepFeature && imgDe < nimages){
		
		if(featAte>stepFeature) featAte = stepFeature;
		if(imgAte>nimages) imgAte = nimages;
		
		for(int featId=featDe;featId<featAte;featId++){
			
			for(int imgId=imgDe;imgId<imgAte;imgId++){
				IntegralImageDev ii( &(data[24*24*imgId]) ,size);
				
				answer[featId*nimages+imgId] = ii.filter( &(fmd[featId+startFeature]) );	
			}			
		}
	}

	
}

int testFilterCuda(ulong* data_device, Point size_image, int nimages, FeatureMaskDev* fmd_device, int startFeature, int stepFeature, int nfeatures, int sizeAnswer, ulong* answer_host, ulong* answer_device){	
	printf("KERNEL CALL\n");
	kernelTestFilter<<<16000,712>>>(data_device, size_image, nimages, fmd_device, startFeature, stepFeature, nfeatures, answer_device);
	cudaMemcpy( answer_host, answer_device, sizeof(ulong)*sizeAnswer, cudaMemcpyDeviceToHost );			
	printf("END CALL\n");
	
	cudaError_t error = cudaGetLastError();
	if(error != cudaSuccess){
		// print the CUDA error message and exit
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	}	
}


void Trainer::startCuda(){
    if(Config::CLASSIFIER_CUDA){
		FacesFeatureFactory fff;
		nfeatures = fff._facesFeatures.size();
		stepFeature = 30000;
		
		sizeAnswer = ts.size()*stepFeature;    
		startFeature = 0;
		endFeature = stepFeature;
		
		size_image.x = 24;
		size_image.y = 24;
		
		prepareData(ts, &size_image, &data_host, &data_device, nfeatures, &fmd_host, &fmd_device, sizeAnswer, &answer_host, &answer_device, &answer_host_buffer, &answer_device_buffer, true);	
		
		testFilterCuda(data_device,size_image,ts.size(),fmd_device,startFeature,stepFeature,nfeatures,sizeAnswer,answer_host,answer_device);
		testFilterCuda(data_device,size_image,ts.size(),fmd_device,startFeature,stepFeature,nfeatures,sizeAnswer,answer_host_buffer,answer_device_buffer);			
		
		cuda_stage = 2;
	}
}	

void Trainer::populateAnswer(){
	answer_host = answer_host_buffer;
	answer_device = answer_device_buffer;
	
	if(cuda_stage*stepFeature>nfeatures) cuda_stage=0;
	
	testFilterCuda(data_device,size_image,ts.size(),fmd_device,cuda_stage*stepFeature,stepFeature,nfeatures,sizeAnswer,answer_host_buffer,answer_device_buffer);
	cuda_stage+=1;	
}


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

    inputInfo();
    
    startCuda();
}

void Trainer::prepareTrainer(){
    for(int i=0;i<THREADS_NUMBER;i++) _ct[i] = new ClassificationTable();        
    for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->initTable(_tsm->trs());
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
        if(r==-1) break;   
    }
    

    return cascade;
}

void* getBestFromFeature(void* params){
     printf("INIT\n");
    elem_params* ep = (elem_params*) params;

    int from = ep->factor*ep->thread_number;
    int to = ep->factor*(ep->thread_number+1);

    if(ep->final==1){
        to = ep->t->_facesFactory._facesFeatures.size()-1;
    }

     printf("OK %d %d %d\n", from, to, ep->factor);    

    TableItem partialBest;
    for(int i=from;i<to;i++){
        // printf("GO %d\n", i);
        partialBest = ep->t->_ct[ep->thread_number]->getBestTableItem( (ep->t->_facesFactory._facesFeatures)[i], ep->t->_tsm->trs() );
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
        ep[t].best = TableItem(0,0,FACE,1.0,1);
        ep[t].final = (t==THREADS_NUMBER-1?1:0);

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

    for(int i=0;i<THREADS_NUMBER;i++) _ct[i]->updateWeights(b_t,h,_tsm->trs());
    
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
