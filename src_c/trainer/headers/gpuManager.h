#ifndef __GPU_MANAGER__
#define __GPU_MANAGER__

#include <cstdlib>
#include <pthread.h>
#include <queue>
#include "../../feature/headers/featureMask.h"
#include "gpuBuffer.h"
#include "trainingSet.h"
#include "cudaDefs.h"

#define GPU_BUFFER 4
#define SINGLE_BUFFER_STEP_FEATURE 30000
#define BLOCK_SIZE 106
#define GRID_SIZE 16000

class GPUManager{
private:
    int _total_features;
    int _nimages;

    int _cur_stage;
    int _max_stage;
    int _feat_per_buffer;
    int _buffer_chunk;

    int _get_filled_counter;
public:

    GPUManager(TrainingSet& ts,int totalFeatures, int nimages);
    GPUBuffer* getFilledBuffer();
    void fillBuffer(GPUBuffer* b, int from, int to);
    GPUBuffer* getConsumedBuffer(int* from, int* to);   
    void resetImageData(TrainingSet& ts); 
    void resetManager();
    void bufferHasBeenConsumed(GPUBuffer* b);
    void wait();
    void restart();

    FeatureMaskDev* fmd_host;
    FeatureMaskDev* fmd_device;

    GPUBuffer* buffers[GPU_BUFFER];
    std::queue<GPUBuffer*> consumed_buffers;
    std::queue<GPUBuffer*> filled_buffers;

    ulong* data_host;
    ulong* data_device;
    
    Point size_image;
    
    pthread_t threads[2];

    volatile bool CONSUMED_BUFFER;
    volatile bool END_STAGE;
    pthread_mutex_t M;
};

#endif
