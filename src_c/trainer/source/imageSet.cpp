#include "../headers/imageSet.h"

int ImageSet::getAllGroups(std::string path, std::vector<std::string>& groups){
    int r = getAllDirectories(path, groups);
    for(register int i=0;i<groups.size();i++){
        groups[i] = groups[i] + "/crop";
    }

    return r;
}



int ImageSet::getGroupScenePath(std::string img_dir, int stage, int n, std::vector<std::string>& paths){
    char group_name[10];
    sprintf(group_name,"group_%d",stage);
    img_dir = img_dir + "/" + std::string(group_name) + "/crop";

    return getFiles(img_dir,n,paths);
}

int ImageSet::getGroupScenePath(std::string img_dir, int stage, int n, std::vector<TID>& tids){
    char group_name[10];
    sprintf(group_name,"group_%d",stage);
    img_dir = img_dir + "/" + std::string(group_name) + "/crop";

    std::vector<std::string> paths;
    int r = getFiles(img_dir,n,paths);

    for(int i=0;i<paths.size();i++){
        tids.push_back(TID(paths[i],false));
    }

    return r;
}




int ImageSet::getAllGroupsScenePath(std::string img_dir, std::vector<std::string>& paths){
    std::vector<std::string> groups;
    int r = getAllGroups(img_dir,groups);
    for(register int i=0;i<groups.size();i++){
        r = getAllFiles(groups[i],paths);
        if(r==-1) return r;
    }

    return r;
}

int ImageSet::getAllGroupsScenePath(std::string img_dir, std::vector<TID>& tids){
    std::vector<std::string> groups;
    int r = getAllGroups(img_dir,groups);
    for(register int i=0;i<groups.size();i++){
        std::vector<std::string> paths;

        r = getAllFiles(groups[i],paths);
        if(r==-1) return r;

        for(int j=0;j<paths.size();j++){
            tids.push_back(TID(paths[j],false));
        }
    }

    return r;
}




int ImageSet::getAllPathFaces(std::string img_dir, std::vector<std::string>& paths){
    return getAllFiles(img_dir,paths);
}

int ImageSet::getAllPathFaces(std::string img_dir, std::vector<TID>& tids){
    std::vector<std::string> paths;
    int r = getAllFiles(img_dir,paths);

    if(r!=-1){
        for(int i=0;i<paths.size();i++){
            tids.push_back(TID(paths[i],false));
        }
    }

    return r;
}