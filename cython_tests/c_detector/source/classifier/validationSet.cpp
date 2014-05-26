#include "../../headers/classifier/validationSet.h"

ValidationSet::ValidationSet(std::string facesValidationDir, std::string scenesValidationDir): ImageSet(facesValidationDir, scenesValidationDir){
    getAllPathFaces(facesValidationDir, _faces);    
    getAllGroupsScenePath(scenesValidationDir, _scenes);
}

