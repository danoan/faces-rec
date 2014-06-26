#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "../../feature/headers/featureMask.h"
#include "../../classifier/headers/libclassifier.h"
#include "../../trainer/headers/libtrainer.h"
#include "../../util/headers/libutil.h"


int train(){
    std::string vs_face_path, vs_scene_path;
    std::string ts_face_path, ts_scene_path;

    vs_face_path = Config::VALIDATION_FACES_PATH;
    vs_scene_path = Config::VALIDATION_SCENES_PATH;

    ts_face_path = Config::TRAINING_FACES_PATH;
    ts_scene_path = Config::TRAINING_SCENES_PATH;

    ValidationSet vs(vs_face_path,vs_scene_path);
    TrainingSet ts(ts_face_path,ts_scene_path);

    Logger::debug->log("Image Sets TS_FACES %d\nTS_SCENES %d\n",ts._faces.size(),ts._scenes.size());

    Trainer* t = new Trainer(ts,vs);
    CascadeClassifier cl = t->startTrainingCascade();    

    // cl.save(Config::CLASSIFIERS_PATH + "/classifier_final");
}

int main(int argc, char* argv[]){
    if( Config::readInput(argc,argv)!=1) return 1;
    Logger::init("trainer");
    train();
    return 0;
}
