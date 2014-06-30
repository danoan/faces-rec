#include <unistd.h>
#include <vector>
#include <string>

#include "../../util/headers/libutil.h"
#include "../../feature/headers/libfeature.h"
#include "../../classifier/headers/libclassifier.h"
#include "../../detector/headers/libdetector.h"

bool b_detectFaces = false;
std::string classifier_path;

bool b_simpleFaces = false;
std::string img_dir;

int detectSimpleFaces(std::string img_dir, std::string classifier_path){
    std::vector<std::string> files;
    CascadeClassifier cl;
    cl.load(Config::CLASSIFIERS_PATH+ "/" + classifier_path);    

    getAllFiles(img_dir,files);

    Point wr;
    wr.x=Config::ARDIS_WIDTH;
    wr.y=Config::ARDIS_HEIGHT;

    Subwindow sw (0,0,wr,1,1);
    int faces_count=0;

    for(int i=0;i<files.size();i++){
        IntegralImage ii ( files[i] );
        faces_count+= cl.isFace(ii,sw);
    }

    printf("FACES DETECTED %d/%d\n",faces_count,files.size());
}

int detectFaces(std::string classifier_path){
    printf("Detection started for: %s\n\n", classifier_path.c_str());
    CascadeClassifier cc;
    cc.load(classifier_path);

    Point ref_mask;
    ref_mask.x = Config::DETECTOR_SUBWINDOW_START_WIDTH;
    ref_mask.y = Config::DETECTOR_SUBWINDOW_START_HEIGHT;
    
    Point ardis;
    ardis.x = Config::ARDIS_WIDTH;
    ardis.y = Config::ARDIS_HEIGHT;

    Detector d(Config::DETECTOR_GENERATIONS,ardis,ref_mask,Config::DETECTOR_SHIFT_STEP);
    return d.detectFaces(&cc,Config::DATASET_PATH + Config::DETECTOR_IMG_PATH,Config::DETECTOR_DRAW);
}

int detectFaces(std::string folder_classifiers, std::string classifier_path){
    detectFaces( folder_classifiers + "/" + classifier_path );
}


int main(int argc, char* argv[]){
    if( Config::readInput(argc,argv)!= 1) return 1;
    Logger::init("detector");

    if(Config::DETECTOR_DETECT_FACES){
        detectFaces(Config::CLASSIFIERS_PATH, Config::DETECTOR_CLASSIFIER_PATH);    
    }else if(Config::DETECTOR_SIMPLE_FACES){
        detectSimpleFaces(Config::DETECTOR_IMG_DIR,Config::DETECTOR_CLASSIFIER_PATH);
    }
    
    return 0;
}
