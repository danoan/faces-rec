#include "../headers/trainingImage.h"

TrainingImage::TrainingImage(std::string imagePath, TrainingType tt, bool has_buffer,int features_number){
    _tt = tt;
    _ii = new IntegralImage(imagePath);
    _has_buffer = has_buffer;
    // printf("%s\n",imagePath.c_str());

    if(_has_buffer){   
        for(int i=0;i<features_number;i++){
          _ii_buffer.push_back( MAX_FEATURE_VALUE );  
        } 
    }
}

unsigned long int TrainingImage::filter(FeatureMask& fm){
    //Check if CLASSIFIER_HAS_BUFFER and pick the value for this feature mask or compute the integral image
    if(_has_buffer){
        if(_ii_buffer[fm._id] == MAX_FEATURE_VALUE){
            _ii_buffer[fm._id] = _ii->filter(fm);        
        }           

        return _ii_buffer[fm._id];
    }else{
        return _ii->filter(fm);
    }    
    
}   