#ifndef __CPU_MANAGER__
#define __CPU_MANAGER__

#include "gpuManager.h"
#include "tableItem.h"
#include "trainer.h"
#include "cudaDefs.h"
#include <pthread.h>

class Trainer;

#define CPU_THREADS Config::CUDA_CPU_THREADS

typedef struct{
    Trainer* t;
    TableItem best;
    ulong* answer_host;

    int from;
    int to;
    int thread_number;    
    int startFeature;
} elem_params;

class CPUManager{
private:
    pthread_t* _threads;

public:
    CPUManager();

    TableItem consumeBuffer(GPUBuffer* b, GPUManager* gpum, Trainer* t);
    void createThreads(GPUBuffer* b, Trainer* t, elem_params* ep);
};

#endif
