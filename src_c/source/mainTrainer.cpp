#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "../headers/classifier/cascadeClassifier.h"
#include "../headers/classifier/validationSet.h"
#include "../headers/classifier/trainingSet.h"
#include "../headers/classifier/trainer.h"
#include "../headers/classifier/directory.h"
#include "../headers/config.h"
#include "../headers/basic.h"


#include "../headers/detector/detector.h"

int detectFaces(std::string img_dir, CascadeClassifier cl){
    std::vector<std::string> files;

    getAllFiles(img_dir,files);

    Point wr;
    wr.x=64;
    wr.y=64;

    Subwindow sw (0,0,wr,1,1);
    int faces_count=0;

    for(int i=0;i<files.size();i++){
        IntegralImage ii ( files[i] );
        faces_count+= cl.isFace(ii,sw);
    }

    Logger::debug->log("FACES DETECTED %d/%d\n",faces_count,files.size());
}

int readInput(int argc, char* argv[]){
    char* options = "s:r:w:h:f:d:m:t:n:v:a:b:";
    int c=0;
    while(1){
        c = getopt(argc,argv,options);
        if(c<0) break;
        switch(c){
            case 's':   //SHIFT_STEP
                Config::CLASSIFIER_SHIFT_STEP = atoi(optarg);
                printf("%d\n",Config::CLASSIFIER_SHIFT_STEP);
                break;
            case 'r':   //RESIZE FACTOR
                Config::CLASSIFIER_RESIZE_FACTOR = atof(optarg);
                break;
            case 'w':   //START WIDTH
                Config::CLASSIFIER_SUBWINDOW_START_WIDTH = atoi(optarg);
                break;               
            case 'h':   //START HEIGHT
                Config::CLASSIFIER_SUBWINDOW_START_HEIGHT = atoi(optarg);
                break;                
            case 'f':   //FINAL FALSE POSITIVE RATE
                Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE = atof(optarg);
                break;                 
            case 'd':   //FINAL DETECTION RATE
                Config::CLASSIFIER_FINAL_DETECTION_RATE = atof(optarg);
                break;
            case 'm':   //MAX STAGE FALSE POSITIVE RATE
                Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE = atof(optarg);
                break;                
            case 't':   //MIN STAGE DETECTION RATE
                Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE = atof(optarg);
                break;              
            case 'n':   //MAX STAGES
                Config::CLASSIFIER_MAX_STAGES = atoi(optarg);
                break;              
            case 'v':   //MAX VALIDATION SET LENGTH
                Config::MAX_LENGHT_VALIDATION_SET = atoi(optarg);
                break;                    
            case 'a':   //ARDIS_WIDTH
                Config::ARDIS_WIDTH = atoi(optarg);
                break;                             
            case 'b':   //ARDIS_HEIGHT
                Config::ARDIS_HEIGHT = atoi(optarg);
                break;                             

        }
    }

    return 1;
}

int main(int argc, char* argv[]){
    if(readInput(argc,argv)!=1) return 0;

    ValidationSet vs(Config::VALIDATION_FACES_PATH, Config::VALIDATION_SCENES_PATH);
    TrainingSet ts(Config::TRAINING_FACES_PATH, Config::TRAINING_SCENES_PATH);

    Logger::debug->log("Image Sets TS_FACES %d\nTS_SCENES %d\n",ts._faces.size(),ts._scenes.size());

    Trainer* t = new Trainer(ts,vs);
    CascadeClassifier cl = t->startTrainingCascade();    

    // detectFaces(Config::VALIDATION_FACES_PATH,cl);
    // detectFaces(Config::VALIDATION_SCENES_PATH,cl);

    return 0;
}

/*
64,64 (6,6) 3 = 97039
64,64 (8,8) 2 = 146065
*/