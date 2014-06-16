#include <unistd.h>
#include <vector>
#include <string>

#include "../headers/basic.h"
#include "../headers/config.h"
#include "../headers/classifier/cascadeClassifier.h"
#include "../headers/classifier/directory.h"
#include "../headers/detector/detector.h"

bool b_detectFaces = false;
std::string classifier_path;

bool b_testClassifiers = false;
std::string classifier_folder_path;

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

void detectFaces(std::string classifier_path){
    printf("Detection started for: %s\n\n", classifier_path.c_str());
    CascadeClassifier cc;
    cc.load(classifier_path);

    Point wr;
    wr.x = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    wr.y = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;
    
    Point ardis;
    ardis.x = Config::ARDIS_WIDTH;
    ardis.y = Config::ARDIS_HEIGHT;

    Detector d(Config::DETECTOR_GENERATIONS,wr,ardis,Config::CLASSIFIER_SHIFT_STEP);
    d.detectFaces(&cc,Config::DATASET_PATH+"/seinfeld.pgm");
}

void detectFaces(std::string folder_classifiers, std::string classifier_path){
    detectFaces( folder_classifiers + "/" + classifier_path );
}

void testClassifiers(std::string classifier_folder_path){
    std::vector<std::string> files;
    classifier_folder_path = Config::CLASSIFIERS_PATH + "/" + classifier_folder_path;
    getAllFiles(classifier_folder_path,files);
    for(register int i=0;i<files.size();i++){
        detectFaces(files[i]);
    }
}

int readInput(int argc, char* argv[]){
    char* options = "s:w:h:g:a:b:";
    int c = 0;
    while(1){
        c=getopt(argc,argv,options);
        if(c<0) break;
        switch(c){
            case 's':
                Config::CLASSIFIER_SHIFT_STEP = atoi(optarg);
                break;
            case 'w':
                Config::CLASSIFIER_SUBWINDOW_START_WIDTH = atoi(optarg);
                break;
            case 'h':
                Config::CLASSIFIER_SUBWINDOW_START_HEIGHT = atoi(optarg);
                break;
            case 'g':
                Config::DETECTOR_GENERATIONS = atoi(optarg);
                break;
            case 'a':
                b_detectFaces = true;
                b_testClassifiers = false;
                classifier_path = std::string(optarg);
                break;                
            case 'b':
                b_detectFaces = false;
                b_testClassifiers = true;
                classifier_folder_path = std::string(optarg);
                break;                                
        }    
    }

    return 1;    
}

int main(int argc, char* argv[]){
    if( readInput(argc,argv)!= 1) return 1;
    Logger::init("detector");

    if(b_detectFaces){
        detectFaces(Config::CLASSIFIERS_PATH, classifier_path);    
    }else if(b_testClassifiers){
        testClassifiers(classifier_folder_path);
    }
    
    return 0;
}
