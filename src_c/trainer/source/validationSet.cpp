#include "../headers/validationSet.h"

ValidationSet::ValidationSet(std::string facesValidationDir, std::string scenesValidationDir): ImageSet(facesValidationDir, scenesValidationDir){
    getAllPathFaces(facesValidationDir, _faces);    
    getGroupScenePath(scenesValidationDir, 0, -1, _scenes);
}


int ValidationSet::resetScenesSet(){
    int validation_set_size = _scenes.size();
    _scenes.clear();
    return getGroupScenePath(_svd, _group, validation_set_size, _scenes);   
}
