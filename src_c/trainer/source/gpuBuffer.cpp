#include "gpuBuffer.h"

GPUBuffer::GPUBuffer(int id, int nfeatures, int nimages):_id(id),_nimages(nimages){
    int size = nfeatures*nimages;
    answer_host = (ulong*) malloc(sizeof(ulong)*size);
    cudaMalloc( (void**) &answer_device, sizeof(ulong)*size );
}
