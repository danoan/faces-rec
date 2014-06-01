#include "../../headers/classifier/trainingImage.h"

TrainingImage::TrainingImage(std::string imagePath, TrainingType tt){
    _tt = tt;
    _ii = new IntegralImage(imagePath);
    // printf("%s\n",imagePath.c_str());
}

unsigned long int TrainingImage::filter(FeatureMask& fm){
    return _ii->filter(fm);
}