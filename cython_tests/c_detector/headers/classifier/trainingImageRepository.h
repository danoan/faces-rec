#ifndef __TRAINING_IMAGE_REPOSITORY__
#define __TRAINING_IMAGE_REPOSITORY__

#include <string>
#include <vector>
#include <map>

#include "trainingImage.h"

typedef std::pair<std::string,TrainingType> mapItem;

class TrainingImageRepository{
private:
    int _imageLimit;    //How many images load on memory
    int _imageCount;    //Key of image in the map. This key represents its position on _trainingImages
    int _rangeOnMemory; //It represents the multiple of _imageLimit that corresponds the 1st position in _trainingImages
    std::vector<TrainingImage*>::iterator _it;    

    std::map<int,mapItem> _imagesPath;
    std::vector<TrainingImage*> _trainingImages;

    int computeNextRange();
    void clearTrainingImages();
public:
    TrainingImageRepository():_imageLimit(24000), _imageCount(0), _rangeOnMemory(-1){};

    void addTrainingImage(std::string imagePath, TrainingType tit);
    void loadImages();
    TrainingImage* getNextImage();
};
#endif