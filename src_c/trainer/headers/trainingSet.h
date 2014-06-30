#ifndef __TRAINING_SET__
#define __TRAINING_SET__

#include "imageSet.h"
#include "validationSet.h"

class TrainingSet:public ImageSet{
public:    
    int _theEnd;
    TrainingSet():ImageSet(){};
    TrainingSet(std::string facesValidationDir, std::string scenesValidationDir);

    int resetScenesSet(ClassifierInterface& cc, ValidationSet& vs);
    void storeFalsePositives(ClassifierInterface& cc,std::vector<TID>& all_examples, std::vector<TID>& fp_examples);
};

#endif