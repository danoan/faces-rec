#ifndef __TRAINING_IMAGE_REPOSITORY__
#define __TRAINING_IMAGE_REPOSITORY__

#include <vector>
#include "trainingImage.h"

class TrainingImageRepository{
public:
    static std::vector<TrainingImage*> _faces;
    static std::vector<TrainingImage*> _scenes;

    static void addFace(std::string imagePath, TrainingType tit);
    static void addScene(std::string imagePath, TrainingType tit);

    static TrainingImage* get(int index);
    static int size(){ return _faces.size() + _scenes.size(); };
    static void clearScenes(){ for(int i=_scenes.size()-1;i>=0;i--) delete _scenes[i]; _scenes.clear(); };
};

#endif