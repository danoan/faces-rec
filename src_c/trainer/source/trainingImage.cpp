#include "../headers/trainingImage.h"

TrainingImage::TrainingImage(std::string imagePath, TrainingType tt, bool has_buffer,int features_number):_img_path(imagePath),_tt(tt),_has_buffer(has_buffer){
    _ii = new IntegralImage(imagePath);
    // printf("%s\n",imagePath.c_str());

    if(_has_buffer){   
        for(int i=0;i<features_number;i++){ 
          _ii_buffer.push_back( MAX_FEATURE_VALUE );  
        } 
    }
}

TrainingImage::TrainingImage(ulong** data, Point size, TrainingType tt):_tt(tt),_has_buffer(false){
    _ii = new IntegralImage(data,size);
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