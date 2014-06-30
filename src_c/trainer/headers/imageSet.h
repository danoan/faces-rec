#ifndef __IMAGE_SET__
#define __IMAGE_SET__

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <vector>
#include "../../util/headers/directory.h"
#include "../../classifier/headers/classifier.h"
#include "trainingImageDescriptor.h"

class ImageSet{    
public:    
    std::string _fvd, _svd;
    std::vector<TID> _faces;
    std::vector<TID> _scenes;

    int _group;

    ImageSet(){};
    ImageSet(std::string facesValidationDir, std::string scenesValidationDir):_fvd(facesValidationDir), _svd(scenesValidationDir), _group(0){};

    int getAllGroups(std::string path, std::vector<std::string>& groups);
    
    int getGroupScenePath(std::string img_dir, int stage, int n, std::vector<std::string>& paths);
    int getGroupScenePath(std::string img_dir, int stage, int n, std::vector<TID>& tids);

    int getAllGroupsScenePath(std::string img_dir, std::vector<std::string>& paths);
    int getAllGroupsScenePath(std::string img_dir, std::vector<TID>& tids);

    int getAllPathFaces(std::string img_dir, std::vector<std::string>& paths);    
    int getAllPathFaces(std::string img_dir, std::vector<TID>& tids);    
};

#endif
