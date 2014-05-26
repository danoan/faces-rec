#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <dirent.h>



#include "../headers/classifier/cascadeClassifier.h"
#include "../headers/classifier/validationSet.h"
#include "../headers/classifier/trainingSet.h"
#include "../headers/classifier/trainer.h"
#include "../headers/config.h"

#include "../headers/detector/detector.h"

int detectFaces(std::string img_dir, CascadeClassifier cl){
    DIR* dir;
    struct dirent *ent;

    Point wr;
    wr.x=64;
    wr.y=64;

    Subwindow sw (0,0,wr,1,1);
    int faces_count=0;
    //It must have ONLY files, no directories
    if( (dir = opendir(img_dir.c_str()) ) != NULL){        
        while( (ent=readdir(dir))!=NULL ){
            if( strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 ) continue;


            IntegralImage ii ( (img_dir + "/" + ent->d_name) );
            faces_count+= cl.isFace(ii,sw);
        }
        closedir(dir);
    }else{
        printf("Directory could not be opened\n");
        return -1;
    }

    printf("FACES DETECTED %d\n",faces_count);
}



int main(int argc, char* argv[]){
    ValidationSet vs(Config::VALIDATION_FACES_TEST_PATH, Config::VALIDATION_SCENES_TEST_PATH);
    TrainingSet ts(Config::TRAINING_FACES_TEST_PATH, Config::TRAINING_SCENES_TEST_PATH);

    printf("Image Sets VS_FACES %d VS_SCENES %d  TS_FACES %d  TS_SCENES %d\n",vs._faces.size(),vs._scenes.size(),ts._faces.size(),ts._scenes.size());

    Trainer* t = new Trainer(ts,vs);
    CascadeClassifier cl = t->startTrainingCascade();    


    detectFaces(Config::VALIDATION_FACES_TEST_PATH,cl);
    detectFaces(Config::VALIDATION_SCENES_TEST_PATH+"/group_1/crop",cl);

    return 0;
}

// ImageSet is;
// std::vector<std::string> _scenes;
// is.getGroupScenePath(Config::VALIDATION_SCENES_TEST_PATH, 1, 50, _scenes);   

// printf("%d\n",_scenes.size());


// int main(int argc, char* argv[]){
//     std::string train_faces_dir = Config::TRAINING_FACES_PATH;
//     std::string train_scene_dir = Config::TRAINING_SCENES_PATH;

//     Trainer* t = new Trainer();

//     addTrainingImages(train_faces_dir,t,FACE);
//     addTrainingImages(train_scene_dir,t,SCENE);

//     startClock();
//     Classifier c = t->startTraining();
//     stopClock("Training");


//     detectFaces(Config::TRAINING_FACES_PATH,c);
//     detectFaces(Config::TRAINING_SCENES_PATH,c);
//     // IntegralImage ii("/home/daniel/Projects/faces-rec/dataset/training_images/test/Aaron_Peirsol_0003.pgm");

//     return 0;
// }

// seta diretorio das faces de teste
// seta diretiorio das faces de validacao

// seta diretorio das cenas de teste     (38 grupos de 10.000)
// seta diretorio das cenas de validacao (2 grupos de 10.000)

// seta taxa maxima de falsos positios
// seta taxa minimia de deteccao

