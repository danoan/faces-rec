#include "../../headers/classifier/trainingImageRepository.h"

    
int TrainingImageRepository::computeNextRange(){
    _rangeOnMemory = _rangeOnMemory+_imageLimit;
    bool isTheEnd = _rangeOnMemory>=_imageCount;
    
    if(isTheEnd) _rangeOnMemory=0;
    // printf("%d %d %d\n",_rangeOnMemory,_imageLimit,_imageCount);

    //Only loadImages if more than one buffer is necessary. If not, all images are already loaded
    if(_imageCount>_imageLimit) loadImages();
    else _it=_trainingImages.begin();
    
    if(isTheEnd) return -1;
    else return 1;
}

void TrainingImageRepository::clearTrainingImages(){
    for(int i=_trainingImages.size();i>0;i--){
        delete _trainingImages.back();
        _trainingImages.pop_back();
    }
}

void TrainingImageRepository::loadImages(){
    // printf("LOAD %d %d %d\n",_imageLimit,_imageCount,_trainingImages.size());
    clearTrainingImages();
    for(int i=_rangeOnMemory;i<(_rangeOnMemory + _imageLimit);i++){
        if(i>=_imageCount) break;
        _trainingImages.push_back( new TrainingImage(_imagesPath[i].first, _imagesPath[i].second, false) );
    }
    _it = _trainingImages.begin();    
}

void TrainingImageRepository::addTrainingImage(std::string imagePath, TrainingType tit){
    _imagesPath[ _imageCount ] = mapItem(imagePath,tit);    
    _imageCount++;
}

TrainingImage* TrainingImageRepository::getNextImage(){
    if(_rangeOnMemory==-1){ //First time getNextImage is called
        _rangeOnMemory=0;
        loadImages();
        _it=_trainingImages.begin();
    }

    if(_it==_trainingImages.end()){    //The end of the current range has arrived
        if(_imageCount==0) return NULL; //No TrainingImage was ever added to the Repository
        if(computeNextRange()==-1) return NULL;   //It reaches the end and will start over again
    }

    return *(_it++);

}   