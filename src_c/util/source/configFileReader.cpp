#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "../headers/config.h"

int readParam(std::ifstream& ifs, std::string prefix,std::string& p){
    std::string name;
    ifs >> name;
    ifs >> p;

    p = prefix + p;

    // printf("%s: %s\n", name.c_str(),p.c_str());
}

int readConfigFile(std::string file_path){
    std::ifstream ifs(file_path.c_str());

    readParam(ifs,"",Config::PROJECT_PATH);
    readParam(ifs,Config::PROJECT_PATH, Config::LOG_PATH);
    readParam(ifs,Config::PROJECT_PATH, Config::CLASSIFIERS_PATH);
    readParam(ifs,Config::CLASSIFIERS_PATH, Config::STATES_PATH);
    readParam(ifs,Config::Config::PROJECT_PATH, Config::DATASET_PATH);
    readParam(ifs,Config::Config::DATASET_PATH, Config::ATT_FACES_PATH);

    readParam(ifs,Config::DATASET_PATH, Config::TRAINING_IMG_PATH);
    readParam(ifs,Config::DATASET_PATH, Config::VALIDATION_IMG_PATH);
    readParam(ifs,Config::DATASET_PATH, Config::TEST_IMG_PATH);

    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TEST_FACES_PATH);
    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TRAINING_FACES_PATH);
    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TRAINING_SCENES_PATH);

    readParam(ifs,Config::VALIDATION_IMG_PATH, Config::VALIDATION_FACES_PATH);
    readParam(ifs,Config::VALIDATION_IMG_PATH, Config::VALIDATION_SCENES_PATH);

    ifs.close();    
}

int main(){
    readConfigFile("config.conf");
}