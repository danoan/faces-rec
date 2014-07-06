#include "../headers/trainerSetStatic.h"

TrainerSetStatic::TrainerSetStatic(std::string faces_training_path, std::string faces_validation_path, std::string scenes_training_path, std::string scenes_validation_path){
    _faces_training_path = faces_training_path;
    _faces_validation_path = faces_validation_path;
    _scenes_training_path = scenes_training_path;
    _scenes_validation_path = scenes_validation_path;
}

void TrainerSetStatic::init(int featuresNumber, int bufferSize){
    _trs.init(featuresNumber,bufferSize);
    _vas.init(featuresNumber,0);

    loadAndStoreFaces(_faces_training_path,_trs);
    loadAndStoreFaces(_faces_validation_path,_vas);

    scanGroups(_scenes_training_path,_training_group_paths);
    scanGroups(_scenes_validation_path,_validation_group_paths);

    loadAndStoreScenes(currentTrainingGroupPath(),_trs);
    loadAndStoreScenes(currentValidationGroupPath(),_vas);

    Logger::debug->log("Image Sets TS_FACES %d\nTS_SCENES %d\n",_trs._faces.size(),_trs._scenes.size());

    _theEnd = 0;    
}

void TrainerSetStatic::scanGroups(std::string groupsPath, std::vector<std::string>& container){
    std::vector<std::string> dir_paths;

    int r = getAllDirectories(groupsPath,dir_paths);
    for(int i=0;i<dir_paths.size();i++){
        char group_name[10];
        sprintf(group_name,"group_%d",i);
        container.push_back( groupsPath + "/" + std::string(group_name) + "/crop" );
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

    for(int i=0;i<container.size();i++){
        dir_paths.clear();
        r = getAllDirectories(container[i],dir_paths);
        container[i] = dir_paths[0];
    }


    if(r==-1){
        printf("Erro leitura de diretorios de Grupos\n");
        exit(1);
    } 
}

std::string TrainerSetStatic::currentTrainingGroupPath(){
    return _training_group_paths[_training_group_number];
}

std::string TrainerSetStatic::currentValidationGroupPath(){
    return _validation_group_paths[_validation_group_number];
}

int TrainerSetStatic::loadAndStoreFaces(std::string dir_path,TrainingSet& ts){
    std::vector<std::string> file_paths;
    int r = getAllFiles(dir_path,file_paths);
    for(int i=0;i<file_paths.size();i++){
        ts.addFace( TID(file_paths[i],false) );
    }

    return r;
}

int TrainerSetStatic::loadAndStoreScenes(std::string dir_path,TrainingSet& ts, ClassifierInterface& cc){
    std::vector<std::string> file_paths;
    int r = getAllFiles(dir_path,file_paths);
    for(int i=0;i<file_paths.size();i++){        
        ts.addScene( TID(file_paths[i],false), cc);
    }

    return r;
}

int TrainerSetStatic::loadAndStoreScenes(std::string dir_path,TrainingSet& ts){
    std::vector<std::string> file_paths;
    int r = getAllFiles(dir_path,file_paths);
    for(int i=0;i<file_paths.size();i++){
        ts.addScene( TID(file_paths[i],false));
    }

    return r;
}

void TrainerSetStatic::storeFalsePositives(ClassifierInterface& cc, std::vector<TrainingImage*>& all_examples, std::vector<TrainingImage*>& fp_examples){
    for(register int i=all_examples.size()-1;i>=0;i--){
        TrainingImage* ti = all_examples[i];
        if( cc.isFace( *(ti->_ii) )==1 ){
            fp_examples.push_back( new TrainingImage(ti->_img_path, ti->_tt, ti->_has_buffer, ti->_features_number)  );
        }
    }            
}

int TrainerSetStatic::resetSets(int stage, ClassifierInterface& cc){
    std::vector<TrainingImage*> all_fp;

    storeFalsePositives(cc,_vas._scenes,all_fp);
    storeFalsePositives(cc,_trs._scenes,all_fp);

    Logger::debug->log("(FP: %d/%d)\n",all_fp.size(),_trs._scenes.size()+_vas._scenes.size());

    _trs.clearScenes();
    _vas.clearScenes(); //Ficar de Olho para ver se nao perde a referencia em all_fp

    _trs._scenes = all_fp;
    int r;
    while(_trs._scenes.size()<Config::CLASSIFIER_SCENES_TRAINING_SET_SIZE){
        _training_group_number++;
        if(_training_group_number>=_training_group_paths.size()){
            r=-1;
        }else{
            r = loadAndStoreScenes(currentTrainingGroupPath(),_trs,cc);
        }        

        if(r==-1){
            _theEnd=1;
            printf("REACH END OF THE GROUP\n");
            break;            
        }
    }

    loadAndStoreScenes(currentValidationGroupPath(),_vas);
    return 1;
}

bool TrainerSetStatic::checkClassifier(ClassifierInterface& cc, double* ac, double* fi, double* di, double max_fp_rate, double min_det_rate, double ac_start,double ac_step, double ac_min){
    std::vector<TrainingImage*>::iterator it;

    double rate_fp;
    double rate_det;

    *ac = ac_start;
    while( *ac>=ac_min){
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

        *fi = rate_fp;
        *di = rate_det;

        if(rate_fp>max_fp_rate) return false;    //I have to put more features in the classifier
        if(rate_det<min_det_rate) continue;


        return true;
    }
    
    return false;
}
