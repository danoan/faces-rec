#ifndef __TRAINING_SET__
#define __TRAINING_SET__

#include "imageSet.h"
#include "validationSet.h"

class TrainingSet:public ImageSet{
public:    
    TrainingSet():ImageSet(){};
    TrainingSet(std::string facesValidationDir, std::string scenesValidationDir);

    int resetScenesSet(ClassifierInterface& cc, ValidationSet& vs, int stage, int training_set_size);
    void storeFalsePositives(ClassifierInterface& cc,std::vector<std::string>& all_examples, std::vector<std::string>& fp_examples);
};

#endif