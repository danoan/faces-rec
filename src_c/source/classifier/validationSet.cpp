#include "../../headers/classifier/validationSet.h"

ValidationSet::ValidationSet(std::string facesValidationDir, std::string scenesValidationDir): ImageSet(facesValidationDir, scenesValidationDir){
    getAllPathFaces(facesValidationDir, _faces);    
    getGroupScenePath(scenesValidationDir, 0, -1, _scenes);
}


int ValidationSet::resetScenesSet(int stage){
    _scenes.clear();
    getGroupScenePath(_svd, stage, Config::MAX_LENGHT_VALIDATION_SET, _scenes);   

    return 1;
}
