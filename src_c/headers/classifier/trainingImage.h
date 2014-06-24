#ifndef __TRAINING_IMAGE__
#define __TRAINING_IMAGE__

#include <vector>

#include "../feature/libfeature.h"
#include "integralImage.h"

#define MAX_FEATURE_VALUE 1048576 //64*64*256 Para um ardis de 64x64

enum TrainingType {SCENE, FACE};

class TrainingImage{
public:
    TrainingType _tt;
    IntegralImage* _ii;

    bool _has_buffer;
    std::vector<ulong> _ii_buffer;  //if has_buffer is true, it puts the integral values here

    TrainingImage(std::string imagePath, TrainingType tt, bool has_buffer, int features_number);
    ~TrainingImage(){
        // printf("DESTROY TRAINNING\n");
        delete _ii;
    }
    unsigned long int filter(FeatureMask& fm);

};

#endif