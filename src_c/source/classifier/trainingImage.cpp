#include "../../headers/classifier/trainingImage.h"

TrainingImage::TrainingImage(std::string imagePath, TrainingType tt, bool has_buffer){
    _tt = tt;
    _ii = new IntegralImage(imagePath);
    _has_buffer = has_buffer;
    // printf("%s\n",imagePath.c_str());
}

unsigned long int TrainingImage::filter(FeatureMask& fm){
    //Check if has_buffer and pick the value for this feature mask or compute the integral image
    if(_has_buffer){
        if(_ii_buffer.size()<=fm._id){
            _ii_buffer.push_back(_ii->filter(fm));        
        }           

        return _ii_buffer[fm._id];
    }else{
        return _ii->filter(fm);
    }    
    
}