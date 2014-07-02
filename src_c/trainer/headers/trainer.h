#ifndef __TRAINER__
#define __TRAINER__

#include <string>
#include <pthread.h>
#include "trainingImageDescriptor.h"
#include "trainingImage.h"
#include "classificationTable.h"
#include "trainingSet.h"
#include "trainerSetStatic.h"

#include "../../classifier/headers/libclassifier.h"
#include "../../feature/headers/libfeature.h"
#include "../../util/headers/libutil.h"

#define THREADS_NUMBER 4

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

    int _stage_number = 0;
    int _feature_number = 0;

    int _firstStagesMaxFeature[5] = {2,5,10,25,50};

    Point _ardis;    

    pthread_t _threads[THREADS_NUMBER];

    void inputInfo();
    ulong addFeatureMasks(FMF factory);

public:
    ClassificationTable* _ct[THREADS_NUMBER];
    FacesFeatureFactory _facesFactory;
    TrainerSetManager* _tsm;

    Trainer(TrainerSetManager* tsm);

    void prepareTrainer();
    void endTrainer(){ for(int i=0;i<THREADS_NUMBER;i++) delete _ct[i];};
    
    Classifier startTraining();
    CascadeClassifier startTrainingCascade();
    void keepTraining(Classifier& cl);

    bool checkClassifier(Classifier& cc, double* ac, double* fi, double* di);
    bool firstStagesCheckClassifier(Classifier& cc, double* ac, double* fi, double* di, int stage, int featureNumber);

    double final_fp_rate(){return _final_fp_rate;};
    double final_det_rate(){return _final_det_rate;};
};

typedef struct{
    Trainer* t;
    int thread_number;
    int factor;
    TableItem best;
    int final;
} elem_params;

void* getBestFromFeature(void* params);


#endif