#ifndef __TRAINING_SET__
#define __TRAINING_SET__

#include <vector>
#include "trainingImage.h"
#include "trainingImageDescriptor.h"
#include "../../classifier/headers/classifier.h"

class TrainingSet{
private:
    int _features_number;
    int _max_buffer;
public:    
    TrainingSet(){};    
    void init(int featuresNumber, int bufferSize);

    std::vector<TrainingImage*> _faces;
    std::vector<TrainingImage*> _scenes;

    void addFace(TID tid);
    void addScene(TID tid);
    void addScene(TID tid,ClassifierInterface& cc);

    inline TrainingImage* get(int index){
        if(index<_faces.size()){
            return _faces[index];
        }else{
            return _scenes[ index- _faces.size() ];
        }        
    };
    inline int size(){ return _faces.size() + _scenes.size(); };
    
    void clearScenes(){ for(int i=_scenes.size()-1;i>=0;i--) delete _scenes[i]; _scenes.clear(); };
    void clearFaces(){ for(int i=_faces.size()-1;i>=0;i--) delete _faces[i]; _faces.clear(); };
    void clear(){clearScenes();clearFaces();printf("%d %d\n",_faces.size(),_scenes.size());};
};

#endif