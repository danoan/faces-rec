#ifndef __TRAINING_IMAGE_REPOSITORY__
#define __TRAINING_IMAGE_REPOSITORY__

#include <vector>
#include "trainingImage.h"
#include "trainingImageDescriptor.h"

class TrainingImageRepository{
private:
    int _max_buffer;
    int _features_number;
public:
    TrainingImageRepository(){};
    void init(int features_number);

    std::vector<TrainingImage*> _faces;
    std::vector<TrainingImage*> _scenes;

    void addFace(TID tid, TrainingType tit);
    void addScene(TID tid, TrainingType tit);

    TrainingImage* get(int index);
    int size(){ return _faces.size() + _scenes.size(); };
    
    void clearScenes(){ for(int i=_scenes.size()-1;i>=0;i--) delete _scenes[i]; _scenes.clear(); };
    void clearFaces(){ for(int i=_faces.size()-1;i>=0;i--) delete _faces[i]; _faces.clear(); };
    void clear(){clearScenes();clearFaces();printf("%d %d\n",_faces.size(),_scenes.size());};
};

#endif