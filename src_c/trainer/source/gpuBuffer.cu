#include "../headers/gpuBuffer.h"

GPUBuffer::GPUBuffer(int id, int nfeatures, int nimages):_id(id){
    _size = nfeatures*nimages;
    answer_host = (ulong*) malloc(sizeof(ulong)*_size);
    cudaMalloc( (void**) &answer_device, sizeof(ulong)*_size );
}

GPUBuffer::~GPUBuffer(){
	Logger::cuda->log("*** \n FREE BUFFER \n *** \n");
	free(answer_host);
	cudaFree(answer_device);
}
