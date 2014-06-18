#include "../../headers/classifier/trainingImageRepository.h"

std::vector<TrainingImage*> TrainingImageRepository::_faces;
std::vector<TrainingImage*> TrainingImageRepository::_scenes;

void TrainingImageRepository::addFace(std::string imagePath, TrainingType tit){
    if(_faces.size()<Config::INTEGRAL_IMAGE_BUFFER_SIZE){
        _faces.push_back(new TrainingImage(imagePath,tit,Config::HAS_BUFFER)); 
    }else{
        _faces.push_back(new TrainingImage(imagePath,tit,false)); 
    }
}

void TrainingImageRepository::addScene(std::string imagePath, TrainingType tit){
    _scenes.push_back(new TrainingImage(imagePath,tit,false)); 
}

TrainingImage* TrainingImageRepository::get(int index){
    if(index<_faces.size()){
        return _faces[index];
    }else{
        return _scenes[ index- _faces.size() ];
    }
}