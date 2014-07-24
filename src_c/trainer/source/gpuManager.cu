#include "../headers/gpuManager.h"

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

void prepareData(TrainingSet& ts, Point* size_image, ulong** data_host, ulong** data_device, int nfeatures, FeatureMaskDev** fmd_host, FeatureMaskDev** fmd_device, bool alloc){
	int ardis_w = 24;
	int ardis_h = 24;
	int nimages = ts.size();
	int totalPixels = nimages*ardis_w*ardis_h;
	
	printf("PREPARING DATA\n");
	
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
}

__global__ void kernelFilter(ulong* data, Point size, int nimages, FeatureMaskDev* fmd,int startFeature, int stepFeature, int nfeatures, ulong* answer){	

	/*
	int factorF = stepFeature/712 + 1;
	int featDe = threadIdx.x*factorF;
	int featAte = (threadIdx.x+1)*factorF;
	
	int factorI = nimages/16000 + 1;
	int imgDe =  blockIdx.x*factorI;
	int imgAte = (blockIdx.x+1)*factorI;
	*/
	
	
	int factorF = stepFeature/gridDim.x + 1;
	int featDe = blockIdx.x*factorF;
	int featAte = (blockIdx.x+1)*factorF;
	
	int factorI = nimages/blockDim.x + 1;
	int imgDe =  threadIdx.x*factorI;
	int imgAte = (threadIdx.x+1)*factorI;	
	
	if(featDe < stepFeature && imgDe < nimages){
		
		if(featAte>stepFeature) featAte = stepFeature;
		if(imgAte>nimages) imgAte = nimages;
		
		for(int featId=featDe;featId<featAte;featId++){
			if( (featId+startFeature)>=nfeatures) break;
			for(int imgId=imgDe;imgId<imgAte;imgId++){
				IntegralImageDev ii( &(data[24*24*imgId]) ,size);
				
				answer[featId*nimages+imgId] = ii.filter( &(fmd[featId+startFeature]) );	
				//answer[threadIdx.x] = ii.filter( &(fmd[0]) );	
				
			}			
		}
	}
	/*
	answer[0] = data[0];
	answer[1] = data[1];
	answer[2] = data[2];
	answer[3] = data[3];
	*/
}



int callCUDA(ulong* data_device, Point size_image, int nimages, FeatureMaskDev* fmd_device, int startFeature, int stepFeature, int nfeatures, int sizeAnswer, ulong* answer_host, ulong* answer_device){	
	printf("KERNEL CALL %d %d %d %d %d %d %d\n",size_image.x,size_image.y,nimages,startFeature,stepFeature,nfeatures,sizeAnswer);
	kernelFilter<<<GRID_SIZE,BLOCK_SIZE>>>(data_device, size_image, nimages, fmd_device, startFeature, stepFeature, nfeatures, answer_device);
	cudaMemcpy( answer_host, answer_device, sizeof(ulong)*sizeAnswer, cudaMemcpyDeviceToHost );			
	printf("END CALL\n");
	
	cudaError_t error = cudaGetLastError();
	if(error != cudaSuccess){
		// print the CUDA error message and exit
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	}	
}


void* gpuThread(void* vp){
    GPUManager* manager = (GPUManager*) vp;

	printf("BEGINBEGUNBEGIN\n");
    while(manager->END_STAGE==0){
        if(manager->CONSUMED_BUFFER==1){
			printf("BUFFER LOAD PROCESS BEGIN\n");
            pthread_mutex_lock( &(manager->M) );
            printf("INSIDE LOCK\n");
            int from,to;
            GPUBuffer* buffer = manager->getConsumedBuffer(&from,&to);
            pthread_mutex_unlock( &(manager->M) );
            manager->fillBuffer(buffer,from,to);
            printf("BUFFER LOAD PROCESS END\n");
        }
    }
	printf("FIMFIMFIM\n");
    pthread_exit(NULL);

}

void GPUManager::wait(){
	pthread_join( threads[0], NULL );
}

GPUManager::GPUManager(TrainingSet& ts, int totalFeatures, int nimages):_total_features(totalFeatures),_nimages(nimages){	  
    _feat_per_buffer = SINGLE_BUFFER_STEP_FEATURE/GPU_BUFFER + 1;
    _max_stage = totalFeatures/SINGLE_BUFFER_STEP_FEATURE + 1;
 
 	size_image.x = 24;
	size_image.y = 24;    

    for(int i=0;i<GPU_BUFFER;i++){
        buffers[i] = new GPUBuffer(i,_feat_per_buffer, nimages);
    }
    resetManager();
    	
    prepareData(ts, &size_image, &data_host, &data_device, totalFeatures, &fmd_host, &fmd_device, true);
}

void GPUManager::resetManager(){
	_cur_stage = 0; 
	
    _buffer_chunk = 0;
    _get_filled_counter = 0;	
    
    while(!consumed_buffers.empty()) consumed_buffers.pop();
    while(!filled_buffers.empty()) filled_buffers.pop();
    
    for(int i=0;i<GPU_BUFFER;i++){
        consumed_buffers.push(buffers[i]);
    }    
    
    CONSUMED_BUFFER = true;
    END_STAGE=false;    
}

void GPUManager::resetImageData(TrainingSet& ts){
	_nimages = ts.size();
	printf("NEW SIZE %d\n",_nimages);
    
	free(data_host);
	free(fmd_host);
	
	cudaFree(data_device);
	cudaFree(fmd_device);
	
    for(int i=0;i<GPU_BUFFER;i++){
		printf("REMOVING BUFFER\n");
		delete buffers[i];
        buffers[i] = new GPUBuffer(i,_feat_per_buffer, _nimages);
    }	
    resetManager();
	
	prepareData(ts, &size_image, &data_host, &data_device, _total_features, &fmd_host, &fmd_device, true);	
}

void GPUManager::restart(){
    printf("INITIALIZING GPU MANAGER...START THREAD\n");
	pthread_create(&(threads[0]),NULL, gpuThread, this);
}

GPUBuffer* GPUManager::getConsumedBuffer(int* from, int* to){
	
    if(consumed_buffers.size()==0) return NULL;
    if(consumed_buffers.size()==1){ CONSUMED_BUFFER = false; printf("CONSUMED FALSE %d\n", CONSUMED_BUFFER); }

    GPUBuffer* b = consumed_buffers.front();
    consumed_buffers.pop();

    *from = _cur_stage*SINGLE_BUFFER_STEP_FEATURE + b->_id*_feat_per_buffer;
    *to = _cur_stage*SINGLE_BUFFER_STEP_FEATURE + (b->_id+1)*_feat_per_buffer;

	if(_buffer_chunk == GPU_BUFFER-1){
		_cur_stage = _cur_stage+1;
	}
	
	_buffer_chunk = (_buffer_chunk+1)%GPU_BUFFER;

    if(*to>_total_features) *to=_total_features;

    b->_from = *from;
    b->_to = *to;

    return b;
}

void GPUManager::bufferHasBeenConsumed(GPUBuffer* b){
	consumed_buffers.push(b);
	pthread_mutex_lock( &(M) );
	CONSUMED_BUFFER = true;
	printf("CONSUMED TRUE %d\n", CONSUMED_BUFFER);
	pthread_mutex_unlock( &(M) );
}

void GPUManager::fillBuffer(GPUBuffer* b, int from, int to){
	printf("CALL CUDA BUFFER ID: %d\n",b->_id);
	if(from>_total_features) return;
    callCUDA(data_device, size_image, _nimages, fmd_device, from, _feat_per_buffer, _total_features, b->_size, b->answer_host, b->answer_device);
    filled_buffers.push(b);
}

GPUBuffer* GPUManager::getFilledBuffer(){
    if(filled_buffers.size()==0) return NULL;

	printf("%d - %d\n",_get_filled_counter,_max_stage);

    if( _get_filled_counter==(GPU_BUFFER*_max_stage-1) ) END_STAGE = true;
    else END_STAGE = false;

    _get_filled_counter= (_get_filled_counter+1)%(GPU_BUFFER*_max_stage);

    GPUBuffer* b = filled_buffers.front();
    filled_buffers.pop();

    return b;   
}
