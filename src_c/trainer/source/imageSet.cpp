#include "../headers/imageSet.h"

void ImageSet::getAllGroups(std::string path, std::vector<std::string>& groups){
    getAllDirectories(path, groups);
    for(register int i=0;i<groups.size();i++){
        groups[i] = groups[i] + "/crop";
    }
}

void ImageSet::getGroupScenePath(std::string img_dir, int stage, int n, std::vector<std::string>& paths){
    char group_name[10];
    sprintf(group_name,"group_%d",stage);
    img_dir = img_dir + "/" + std::string(group_name) + "/crop";

    getFiles(img_dir,n,paths);
}

void ImageSet::getAllGroupsScenePath(std::string img_dir, std::vector<std::string>& paths){
    std::vector<std::string> groups;
    getAllGroups(img_dir,groups);
    for(register int i=0;i<groups.size();i++){
        getAllFiles(groups[i],paths);
    }
}


int ImageSet::getAllPathFaces(std::string img_dir, std::vector<std::string>& paths){
    return getAllFiles(img_dir,paths);
}