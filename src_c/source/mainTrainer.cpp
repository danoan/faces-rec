#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "../headers/feature/featureMask.h"

#include "../headers/classifier/cascadeClassifier.h"
#include "../headers/classifier/validationSet.h"
#include "../headers/classifier/trainingSet.h"
#include "../headers/classifier/trainer.h"
#include "../headers/classifier/directory.h"
#include "../headers/util/config.h"
#include "../headers/util/basic.h"

int train(){
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

    // cl.save(Config::CLASSIFIERS_PATH + "/classifier_final");
}

int main(int argc, char* argv[]){
    if(readInput(argc,argv)!=1) return 1;
    Logger::init("trainer");
    train();
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
    p: Dataset folder name

*/
