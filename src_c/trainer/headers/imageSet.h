#ifndef __IMAGE_SET__
#define __IMAGE_SET__

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <vector>
#include "../../util/headers/directory.h"
#include "../../classifier/headers/classifier.h"

class ImageSet{    
public:    
    std::string _fvd, _svd;
    std::vector<std::string> _faces;
    std::vector<std::string> _scenes;

    ImageSet(){};
    ImageSet(std::string facesValidationDir, std::string scenesValidationDir):_fvd(facesValidationDir), _svd(scenesValidationDir){};

    void getAllGroups(std::string path, std::vector<std::string>& groups);
    void getGroupScenePath(std::string img_dir, int stage, int n, std::vector<std::string>& paths);

    void getAllGroupsScenePath(std::string img_dir, std::vector<std::string>& paths);
    int getAllPathFaces(std::string img_dir, std::vector<std::string>& paths);    
};

#endif
