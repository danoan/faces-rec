#ifndef __TRAINER__
#define __TRAINER__

#include <string>
#include <pthread.h>
#include "trainingImageDescriptor.h"
#include "trainingImage.h"
#include "classificationTable.h"
#include "trainingSet.h"
#include "trainerSetStatic.h"
#include "gpuManager.h"
#include "cpuManager.h"

#include "../../classifier/headers/libclassifier.h"
#include "../../feature/headers/libfeature.h"
#include "../../util/headers/libutil.h"

class CPUManager;
#define CPU_THREADS 5

//#define THREADS_NUMBER 1

class Trainer{
private:
    int _shift_w;
    int _shift_h;

    double _resize_factor;
    int _start_w;
    int _start_h;

    double _final_fp_rate;
    double _final_det_rate;

    double _max_fp_rate;
    double _min_det_rate;

    int _stage_number;
    int _feature_number;

    int _firstStagesMaxFeature[5];
    
    GPUManager* gpuManager;
    CPUManager* cpuManager;

    Point _ardis;    

    void inputInfo();
    ulong addFeatureMasks(FMF factory);


public:
    ClassificationTable* _ct[CPU_THREADS];
    FacesFeatureFactory _facesFactory;
    TrainerSetManager* _tsm;

    Trainer(TrainerSetManager* tsm);

    void prepareTrainer();
    void endTrainer(){ for(int i=0;i<CPU_THREADS;i++) delete _ct[i];};
    
    Classifier startTraining();
    CascadeClassifier startTrainingCascade();
    void keepTraining(Classifier& cl);

    bool checkClassifier(Classifier& cc, double* ac, double* fi, double* di);
    bool firstStagesCheckClassifier(Classifier& cc, double* ac, double* fi, double* di, int stage, int featureNumber);

    double final_fp_rate(){return _final_fp_rate;};
    double final_det_rate(){return _final_det_rate;};
};

void* getBestFromFeature(void* params);


#endif
