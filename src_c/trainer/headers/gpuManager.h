#ifndef __GPU_MANAGER__
#define __GPU_MANAGER__

#include <cstdlib>
#include <pthread.h>
#include <queue>
#include "../../feature/headers/featureMask.h"
#include "gpuBuffer.h"
#include "trainingSet.h"
#include "cudaDefs.h"

#define GPU_BUFFER Config::CUDA_BUFFER
#define SINGLE_BUFFER_STEP_FEATURE Config::CUDA_FEATURE_COVER_SIZE
#define BLOCK_SIZE Config::CUDA_BLOCK_SIZE
#define GRID_SIZE Config::CUDA_GRID_SIZE

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

    GPUBuffer** buffers;
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
