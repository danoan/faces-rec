#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "../../feature/headers/featureMask.h"
#include "../../classifier/headers/libclassifier.h"
#include "../../trainer/headers/libtrainer.h"
#include "../../util/headers/libutil.h"


int train(){
    TrainerSetManager* tsm = new TrainerSetStatic(Config::TRAINING_FACES_PATH,Config::VALIDATION_FACES_PATH,Config::TRAINING_SCENES_PATH,Config::VALIDATION_SCENES_PATH);

    Trainer* t = new Trainer(tsm);
    CascadeClassifier cl = t->startTrainingCascade();    

    // cl.save(Config::CLASSIFIERS_PATH + "/classifier_final");
}

int main(int argc, char* argv[]){
    if( Config::readInput(argc,argv)!=1) return 1;
    Logger::init("trainer");
    train();
    return 0;
}
