#ifndef __TRAINING_SET__
#define __TRAINING_SET__

#include "imageSet.h"
#include "validationSet.h"

class TrainingSet:public ImageSet{
public:    
    TrainingSet():ImageSet(){};
    TrainingSet(std::string facesValidationDir, std::string scenesValidationDir);

    int resetScenesSet(Classifier& cc, ValidationSet& vs, int stage);
};

#endif