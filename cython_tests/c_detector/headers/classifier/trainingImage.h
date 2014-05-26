#ifndef __TRAINING_IMAGE__
#define __TRAINING_IMAGE__

#include "../feature/libfeature.h"
#include "integralImage.h"

enum TrainingType {SCENE, FACE};

class TrainingImage{
public:
    TrainingType _tt;
    IntegralImage* _ii;

    TrainingImage(std::string imagePath, TrainingType tt);
    ~TrainingImage(){
        printf("DESTROY TRAINNING\n");
        delete _ii;
    }
    unsigned long int filter(FeatureMask& fm);

};

#endif