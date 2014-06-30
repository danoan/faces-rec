#include "../headers/trainingImageDescriptor.h"

TID::TID(std::string img_path, bool crop_selector): _img_path(img_path), _crop_selector(crop_selector){
    if(_crop_selector){
        getSize(_img_path.c_str(), &_size);
    }
}

int TID::loadNextCrops(int n){
    return n;
}