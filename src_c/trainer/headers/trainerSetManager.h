#ifndef __TRAINER_SET_MANAGER__
#define __TRAINER_SET_MANAGER__

#include "../../classifier/headers/classifier.h"
#include "trainingSet.h"

class TrainerSetManager{
public:
    virtual void init(int featuresNumber, int bufferSize) = 0;

    virtual int resetSets(int stage, ClassifierInterface& cc) = 0;
    virtual bool checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min) = 0;

    virtual TrainingSet& trs() = 0;
    virtual TrainingSet& vas() = 0;
};

#endif