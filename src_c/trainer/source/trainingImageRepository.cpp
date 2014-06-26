#include "../headers/trainingImageRepository.h"

void TrainingImageRepository::init(int features_number){
    /*Com 97039 features, posso armazenar 2500 imagens.Aqui faço uma aproximação.
    Eu digo que 100000 features ocupam 1MB. Logo, para saber quantas imagens podem
    ficar no buffer, basta multiplicar o espaço disponível pela razao 100.000 sobre
    o número de features atual
    */

    _max_buffer = Config::CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE*(100000*1.0/features_number);
    _features_number = features_number;
}

void TrainingImageRepository::addFace(std::string imagePath, TrainingType tit){
    if(_faces.size()<_max_buffer){
        _faces.push_back(new TrainingImage(imagePath,tit,Config::CLASSIFIER_HAS_BUFFER,_features_number)); 
    }else{
        _faces.push_back(new TrainingImage(imagePath,tit,false,_features_number)); 
    }
}

void TrainingImageRepository::addScene(std::string imagePath, TrainingType tit){
    _scenes.push_back(new TrainingImage(imagePath,tit,false,_features_number)); 
}

TrainingImage* TrainingImageRepository::get(int index){
    if(index<_faces.size()){
        return _faces[index];
    }else{
        return _scenes[ index- _faces.size() ];
    }
}