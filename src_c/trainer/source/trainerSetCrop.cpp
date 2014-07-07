#include "../headers/trainerSetCrop.h"

TrainerSetCrop::TrainerSetCrop(std::string faces_training_path, std::string faces_validation_path, std::string scenes_training_path, std::string scenes_validation_path, int nFacesTraining, int nFacesValidation, int ncropsScenesTraining, int ncropsScenesValidation){
    _faces_training_path = faces_training_path;
    _faces_validation_path = faces_validation_path;
    _scenes_training_path = scenes_training_path;
    _scenes_validation_path = scenes_validation_path;

    _nFacesTraining = nFacesTraining;
    _nFacesValidation = nFacesValidation;
    _ncropsScenesTraining = ncropsScenesTraining;
    _ncropsScenesValidation = ncropsScenesValidation;

    _totalRead = 0;

    srand(time(NULL));
}

void TrainerSetCrop::init(int featuresNumber, int bufferSize){
    _trs.init(featuresNumber,bufferSize*0.8);
    _vas.init(featuresNumber,bufferSize*0.2);

    loadAndStoreFaces(_faces_training_path,_nFacesTraining,_trs);
    loadAndStoreFaces(_faces_validation_path,_nFacesValidation,_vas);

    printf("Load Scenes %s \n %s\n", _scenes_training_path.c_str(), _scenes_validation_path.c_str());
    loadScenes(_scenes_training_path,_tid_training);
    loadScenes(_scenes_validation_path,_tid_validation);

    printf("AddScenesCrop\n");
    addSceneCrops(_tid_training,_buffer_training,_ncropsScenesTraining,_trs,NULL);
    addSceneCrops(_tid_validation,_buffer_validation,_ncropsScenesValidation,_vas,NULL);
}

int TrainerSetCrop::loadAndStoreFaces(std::string dir_path,int n,TrainingSet& ts){
    std::vector<std::string> file_paths;
    int r = getFiles(dir_path,n,file_paths);
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

int TrainerSetCrop::chargeBuffer(std::vector<TID>& vtid, std::vector<TID>& buffer, int n){
    int buffer_size=0;    

    if(vtid.size()==0) return -1;
    while(buffer_size<n){
        if(vtid.size()==0) break;

        int img_index = rand() % vtid.size();
        buffer.push_back( vtid[img_index] );

        vtid.erase( vtid.begin() + img_index );
        buffer_size+=1;
    }

    return 1;
}

int TrainerSetCrop::generateCrops(std::vector<TID>& vtid,int imgIndex, int ncrops, int& totalGen, void* vp ){
    int theEnd;
    int windowsRead;
    totalGen = vtid[imgIndex].loadNextCrops(ncrops,windowsRead,theEnd,ClassifierCheckFunctions::checkData,vp);

    _totalRead+=windowsRead;

    if(theEnd==1){
        vtid.erase(vtid.begin()+imgIndex);
        return -1;
    }

    return 1;
}

int TrainerSetCrop::addSceneCrops(std::vector<TID>& vtid,std::vector<TID>& buffer, int n,TrainingSet& ts, void* vp){
    printf("BEFORE DIFF:: Scenes %d N %d TOTAL READ: %d \n",ts._scenes.size(),n,_totalRead);

    int total = 0;
    int diff = n - total;
    while(diff>0){

        if(buffer.size()==0){
            if(chargeBuffer(vtid,buffer, Config::CLASSIFIER_SET_CROP_BUFFER_SIZE)==-1){
                printf("AFTER DIFF:: Total Crops %d Scenes %d N %d Diff %d \n",total,ts._scenes.size(),n,n-total);        
                return -1;  
            }
        }

        int img_index = rand() % buffer.size();
        int totalGen;
        int wantGen;
        if(Config::CLASSIFIER_SET_CROP_ELEMENTS_PER_WINDOW<diff) wantGen = Config::CLASSIFIER_SET_CROP_ELEMENTS_PER_WINDOW;
        else wantGen = diff;

        if( generateCrops(buffer,img_index,wantGen,totalGen,vp)==1 ){
            total+=totalGen;
            ts.addScene(buffer[img_index]);    
        }

        printf("IMG: %d TOTAL: %d\n",img_index,totalGen);

        diff = n-total;        
    }

    printf("AFTER DIFF:: Total Crops %d Scenes %d N %d Diff %d \n",total,ts._scenes.size(),n,n-total);

    return 1;
}

void TrainerSetCrop::storeFalsePositives(ClassifierInterface& cc, std::vector<TrainingImage*>& all_examples, std::vector<TrainingImage*>& fp_examples){
    for(register int i=all_examples.size()-1;i>=0;i--){
        TrainingImage* ti = all_examples[i];
        if( cc.isFace( *(ti->_ii) )==1 ){
            fp_examples.push_back( new TrainingImage(ti->_ii->_data, ti->_ii->_size, ti->_tt)  );
        }
    }
}

int TrainerSetCrop::resetSets(int stage, ClassifierInterface& cc){
    printf("RESET SETS\n");
    std::vector<TrainingImage*> all_fp;

    if(_theEnd==1) return -1;

    storeFalsePositives(cc,_trs._scenes,all_fp);
    storeFalsePositives(cc,_vas._scenes,all_fp);

    Logger::debug->log("(FP: %d/%d)\n",all_fp.size(),_trs._scenes.size()+_vas._scenes.size());

    _trs.clearScenes();
    _vas.clearScenes(); //Ficar de Olho para ver se nao perde a referencia em all_fp

    _trs._scenes = all_fp;
    int r;

    if( addSceneCrops(_tid_training,_buffer_training,_ncropsScenesTraining - all_fp.size(),_trs,&cc) == -1){
        _theEnd = 1;
        printf("REACH END OF THE GROUP\n");
    }

    addSceneCrops(_tid_validation,_buffer_validation,_ncropsScenesValidation,_vas,NULL);
    printf("VALIDATION: %d\n",_vas.size());
    return 1;
}

bool TrainerSetCrop::checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min){
    std::vector<TrainingImage*>::iterator it;

    double rate_fp = 1.0;
    double rate_det = 0.0;

    *ac = ac_start;
    while( *ac>=ac_min){
        *fi = rate_fp;
        *di = rate_det;

        *ac-=ac_step;
        rate_fp=0;

        for(it=_vas._scenes.begin();it!=_vas._scenes.end();it++){
            rate_fp += cc.isFace( *( (*it)->_ii),*ac);
        }

        rate_fp = rate_fp/_vas._scenes.size();
        Logger::debug->log("RATE FP: %.4f\n",rate_fp);


        rate_det=0;
        for(it=_vas._faces.begin();it!=_vas._faces.end();it++){
            rate_det += cc.isFace(  *( (*it)->_ii),*ac);
        }    

        rate_det = rate_det/_vas._faces.size();
        Logger::debug->log("RATE DET: %.4f (MIN:%.4f)\n",rate_det,min_det_rate);

        if(rate_fp>max_fp_rate) return false;    //I have to put more features in the classifier
        if(rate_det<min_det_rate) continue;

        *fi = rate_fp;
        *di = rate_det;

        return true;
    }
    
    return false;
}   
