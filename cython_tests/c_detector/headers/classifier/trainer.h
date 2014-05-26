#ifndef __TRAINER__
#define __TRAINER__

#include <string>
#include "trainingImage.h"
#include "classificationTable.h"
#include "cascadeClassifier.h"
#include "classifier.h"
#include "trainingSet.h"
#include "validationSet.h"
#include "../feature/libfeature.h"
#include "../basic.h"

class Trainer{
private:
    Point _ardis;
    int _shift_w = 1;
    int _shift_h = 1;
    double _resize_factor = 1.25;
    int _start_w = 8;
    int _start_h = 8;    

    int _stage_number = 0;
    int _feature_number = 0;

    double _final_fp_rate = 1e-4;
    double _final_det_rate = 0.9;

    double _max_fp_rate = 0.4;
    double _min_det_rate = 0.998;

    MaskTwoHorizontalFactory _m2hf;
    MaskTwoVerticalFactory _m2vf;
    MaskThreeHorizontalFactory _m3hf;
    MaskThreeVerticalFactory _m3vf;
    MaskDiagonalFactory _mdf;

    TrainingSet _ts;
    ValidationSet _vs;

public:
    ClassificationTable* _ct;

    Trainer(TrainingSet& ts, ValidationSet& vs);

    void addTrainingImage(std::string imagePath, TrainingType imageType );
    void getBestFromFeature(TableItem& theBest, FMF& factory);

    void prepareTrainer();
    Classifier startTraining();
    CascadeClassifier startTrainingCascade();
    void keepTraining(Classifier& cl);

    bool checkClassifier(Classifier& cc, double* ac, double* fi, double* di);
};

#endif