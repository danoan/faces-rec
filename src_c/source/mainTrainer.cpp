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

#include "../headers/feature/maskTwoHorizontalFactory.h"

#include "../headers/detector/detector.h"

int detectSimpleFaces(std::string img_dir, CascadeClassifier cl){
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
    char* options = "s:r:w:h:f:d:m:t:n:v:a:b:i:uzx:";
    int c=0;
    while(1){
        c = getopt(argc,argv,options);
        if(c<0) break;
        switch(c){
            case 's':   //SHIFT_STEP
                Config::CLASSIFIER_SHIFT_STEP = atoi(optarg);
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
            case 'i':   //II Buffer Size
                Config::INTEGRAL_IMAGE_BUFFER_SIZE = atoi(optarg);
                break;     
            case 'u':
                Config::HAS_BUFFER = true;
                break;           
            case 'z':
                Config::TEST_EXECUTION = true;
                break;                           
            case 'x':
                Config::MAX_HYPOTHESIS_PER_STAGE = atoi(optarg);
                break;                                           

        }
    }

    return 1;
}

int train(int argc, char* argv[]){
    if(readInput(argc,argv)!=1) return 0;

    std::string vs_face_path, vs_scene_path;
    std::string ts_face_path, ts_scene_path;

    if(Config::TEST_EXECUTION){
        vs_face_path = Config::VALIDATION_FACES_TEST_PATH;
        vs_scene_path = Config::VALIDATION_SCENES_TEST_PATH;

        ts_face_path = Config::TRAINING_FACES_TEST_PATH;
        ts_scene_path = Config::TRAINING_SCENES_TEST_PATH;
    }else{
        vs_face_path = Config::VALIDATION_FACES_PATH;
        vs_scene_path = Config::VALIDATION_SCENES_PATH;

        ts_face_path = Config::TRAINING_FACES_PATH;
        ts_scene_path = Config::TRAINING_SCENES_PATH;
    }

    ValidationSet vs(vs_face_path,vs_scene_path);
    TrainingSet ts(ts_face_path,ts_scene_path);

    Logger::debug->log("Image Sets TS_FACES %d\nTS_SCENES %d\n",ts._faces.size(),ts._scenes.size());

    Trainer* t = new Trainer(ts,vs);
    CascadeClassifier cl = t->startTrainingCascade();    

    cl.save(Config::CLASSIFIERS_PATH + "/classifier_final");

    detectSimpleFaces(Config::TEST_FACES_PATH,cl);
    // detectSimpleFaces(Config::VALIDATION_SCENES_PATH,cl);
}

void detectFaces(){
    CascadeClassifier cc;
    cc.load(Config::CLASSIFIERS_PATH + "/classifier_final");

    Point wr;
    wr.x = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    wr.y = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;
    
    Point ardis;
    ardis.x = Config::ARDIS_WIDTH;
    ardis.y = Config::ARDIS_HEIGHT;

    Detector d(3,wr,ardis,Config::CLASSIFIER_SHIFT_STEP);
    d.detectFaces(&cc,Config::DATASET_PATH+"/seinfeld.pgm");
}

int main(int argc, char* argv[]){
    // train(argc,argv);
    detectFaces();
    return 0;
}

/*
64,64 (6,6) 3 = 97039
64,64 (8,8) 2 = 146065
*/

/*
    ./a.out -a 64 -b 64 -s 3 -r 1.25 -w 6 -h 6 -i 1000 -n 3 -u -z

    u: Use estrategia de buffer
    z: Use o banco de teste
    i: Buffer Size
    n: Max Stages
    v: Max Length Validation Set
    m: Stage Max FP
    t: Stage Min Det

*/
