#include "../../headers/classifier/trainingSet.h"

TrainingSet::TrainingSet(std::string facesTrainingDir, std::string scenesTrainingDir): ImageSet(facesTrainingDir, scenesTrainingDir){
    getAllPathFaces(facesTrainingDir, _faces);    
    getGroupScenePath(scenesTrainingDir, 0, -1, _scenes);
}

int TrainingSet::resetScenesSet(Classifier& cc, ValidationSet& vs, int stage){
    _scenes.clear();

    std::vector<std::string>::iterator it;
    
    for(register int i=vs._scenes.size()-1;i>=0;i--){
        IntegralImage ii( vs._scenes[i] );
        if( cc.isFace(ii)==1 ){
            _scenes.push_back( vs._scenes[i] );
        }
    }        
    int before_size = _scenes.size();    
    Logger::debug->log("STAGE %d\n (FP: %d/%d)\n",stage,_scenes.size(),vs._scenes.size());

    getGroupScenePath(_svd, stage, Config::MAX_LENGHT_VALIDATION_SET, _scenes);   

    if(_scenes.size()==before_size){
        printf("group_%d not found\n",stage);
        return -1;
    }else{
        printf("NEW TS SCENE: %d\n", _scenes.size());
    }

    return 1;
}