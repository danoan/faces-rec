#ifndef __VALIDATION_SET__
#define __VALIDATION_SET__

#include "imageSet.h"

class ValidationSet:public ImageSet{
public:    
    ValidationSet():ImageSet(){};
    ValidationSet(std::string facesValidationDir, std::string scenesValidationDir);

    int resetScenesSet(int stage);
};

#endif