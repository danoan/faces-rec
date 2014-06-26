#include "../headers/validationSet.h"

ValidationSet::ValidationSet(std::string facesValidationDir, std::string scenesValidationDir): ImageSet(facesValidationDir, scenesValidationDir){
    getAllPathFaces(facesValidationDir, _faces);    
    getGroupScenePath(scenesValidationDir, 0, -1, _scenes);
}


int ValidationSet::resetScenesSet(int stage){
    int validation_set_size = _scenes.size();
    _scenes.clear();
    getGroupScenePath(_svd, stage, validation_set_size, _scenes);   

    return 1;
}
