#include "../headers/trainerSetCrop.h"

TrainerSetCrop::TrainerSetCrop(std::string faces_training_path, std::string faces_validation_path, std::string scenes_training_path, std::string scenes_validation_path, int cropsPerWindow, int ncropsScenesTraining, int ncropsScenesValidation){
    _faces_training_path = faces_training_path;
    _faces_validation_path = faces_validation_path;
    _scenes_training_path = scenes_training_path;
    _scenes_validation_path = scenes_validation_path;

    _crops_per_window = cropsPerWindow;
    _ncropsScenesTraining = ncropsScenesTraining;
    _ncropsScenesValidation = ncropsScenesValidation;

    srand(time(NULL));
}

void TrainerSetCrop::init(int featuresNumber, int bufferSize){
    _trs.init(featuresNumber,bufferSize*0.8);
    _vas.init(featuresNumber,bufferSize*0.2);

    loadAndStoreFaces(_faces_training_path,_trs);
    loadAndStoreFaces(_faces_validation_path,_vas);

    loadScenes(_scenes_training_path,_tid_training);
    loadScenes(_scenes_validation_path,_tid_validation);

    addSceneCrops(_tid_training,_ncropsScenesTraining,_trs,NULL);
    addSceneCrops(_tid_validation,_ncropsScenesValidation,_vas,NULL);
}

int TrainerSetCrop::loadAndStoreFaces(std::string dir_path,TrainingSet& ts){
    std::vector<std::string> file_paths;
    int r = getAllFiles(dir_path,file_paths);
    for(int i=0;i<file_paths.size();i++){
        ts.addFace( TID(file_paths[i],false) );
    }

    return r;
}

int TrainerSetCrop::loadScenes(std::string dir_path,std::vector<TID>& vtid){
    std::vector<std::string> file_paths;
    int r = getAllFiles(dir_path,file_paths);
    for(int i=0;i<file_paths.size();i++){
        vtid.push_back( TID(file_paths[i],true) ); 
    }
}

int TrainerSetCrop::generateCrops(std::vector<TID>& vtid,int imgIndex, int ncrops, int& totalGen, void* vp ){
    int theEnd;
    totalGen = vtid[imgIndex].loadNextCrops(ncrops,theEnd,ClassifierCheckFunctions::checkData,vp);

    if(theEnd==1){
        vtid.erase(vtid.begin()+imgIndex);
        return -1;
    }

    return 1;
}

int TrainerSetCrop::addSceneCrops(std::vector<TID>& vtid,int n,TrainingSet& ts, void* vp){
    int imgs = n/_crops_per_window;

    int total=0;
    for(int i=0;i<imgs;i++){
        int img_index = rand() % vtid.size();
        int totalGen;

        if( generateCrops(vtid,img_index,_crops_per_window,totalGen,vp)==1 ){
            total+=totalGen;
            ts.addScene(vtid[img_index]);    
        }
    }

    int diff = n - total;
    while(diff>0 && vtid.size()>0){
        int img_index = rand() % vtid.size();
        int totalGen;

        if( generateCrops(vtid,img_index,diff,totalGen,vp)==1 ){
            total+=totalGen;
            ts.addScene(vtid[img_index]);    
        }

        diff = n-total;        
    }

    if(vtid.size()==0) return -1;

    return 1;
}

int TrainerSetCrop::resetSets(int stage, ClassifierInterface& cc){
    return 1;
}

bool TrainerSetCrop::checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min){
    return false;
}   