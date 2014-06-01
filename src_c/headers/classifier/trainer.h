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
#include "../config.h"

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

    Point _ardis;

    MaskTwoHorizontalFactory _m2hf;
    MaskTwoVerticalFactory _m2vf;
    MaskThreeHorizontalFactory _m3hf;
    MaskThreeVerticalFactory _m3vf;
    MaskDiagonalFactory _mdf;

    TrainingSet _ts;
    ValidationSet _vs;

    void inputInfo();

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