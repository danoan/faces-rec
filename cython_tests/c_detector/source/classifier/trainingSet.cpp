#include "../../headers/classifier/trainingSet.h"

TrainingSet::TrainingSet(std::string facesValidationDir, std::string scenesValidationDir): ImageSet(facesValidationDir, scenesValidationDir){
    _stage = 1;

    getAllPathFaces(facesValidationDir, _faces);    
    getGroupScenePath(scenesValidationDir, _stage, -1, _scenes);
}

int TrainingSet::resetScenesSet(Classifier& cc, ValidationSet& vs){
    _stage++;
    _scenes.clear();

    std::vector<std::string>::iterator it;
    
    Point wr;
    wr.x=64;
    wr.y=64;

    Subwindow sw (0,0,wr,1,1);        

    for(register int i=vs._scenes.size()-1;i>=0;i--){
        IntegralImage ii( vs._scenes[i] );
        if( cc.isFace(ii,sw)==1 ){
            _scenes.push_back( vs._scenes[i] );
        }
    }        
    int before_size = _scenes.size();    
    printf("STAGE %d\n (FP: %d)",_stage,_scenes.size());
    getGroupScenePath(_svd, _stage, 200, _scenes);   

    if(_scenes.size()==before_size){
        printf("group_%d not found\n",_stage);
        return -1;
    }else{
        printf("NEW TS SCENE: %d\n", _scenes.size());
    }
}