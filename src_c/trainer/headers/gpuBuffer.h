#ifndef __GPU_BUFFER__
#define __GPU_BUFFER__

#include <cstdio>
#include "../../util/headers/basic.h"

class GPUBuffer{
public:    
    ulong* answer_host;
    ulong* answer_device;

    int _id;
    int _size;
    int _from, _to;

    GPUBuffer(int id, int nfeatures, int nimages);
    ~GPUBuffer();
};

#endif
