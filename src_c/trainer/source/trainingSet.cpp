#include "../headers/trainingSet.h"

void TrainingSet::init(int featuresNumber, int bufferSize){
    /*Com 97039 features, posso armazenar 2500 imagens.Aqui faço uma aproximação.
    Eu digo que 100000 features ocupam 1MB. Logo, para saber quantas imagens podem
    ficar no buffer, basta multiplicar o espaço disponível pela razao 100.000 sobre
    o número de features atual
    */

    _max_buffer = bufferSize*(100000*1.0/featuresNumber);
    _features_number = featuresNumber;
}

void TrainingSet::addFace(TID tid){
    if(_faces.size()<_max_buffer){
        _faces.push_back(new TrainingImage(tid._img_path,FACE,Config::CLASSIFIER_HAS_BUFFER,_features_number)); 
    }else{
        _faces.push_back(new TrainingImage(tid._img_path,FACE,false,_features_number)); 
    }
}

void TrainingSet::addScene(TID tid,ClassifierInterface& cc){
    if(tid._crop_selector){

    }else{    
        TrainingImage* ti = new TrainingImage(tid._img_path,SCENE,false,_features_number);
        if( cc.isFace( *(ti->_ii) )==1 ){
            _scenes.push_back(ti);
        }else{
            delete ti;
        }
    }
}

void TrainingSet::addScene(TID tid){
    if(tid._crop_selector){

    }else{
        _scenes.push_back(new TrainingImage(tid._img_path,SCENE,false,_features_number));                
    }
    
}

TrainingImage* TrainingSet::get(int index){
    if(index<_faces.size()){
        return _faces[index];
    }else{
        return _scenes[ index- _faces.size() ];
    }
}
