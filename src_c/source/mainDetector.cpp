#include <unistd.h>
#include <vector>
#include <string>

#include "../headers/basic.h"
#include "../headers/config.h"
#include "../headers/classifier/integralImage.h"
#include "../headers/feature/libfeature.h"
#include "../headers/classifier/cascadeClassifier.h"
#include "../headers/classifier/directory.h"
#include "../headers/detector/detector.h"

bool b_detectFaces = false;
std::string classifier_path;

bool b_testClassifiers = false;
std::string classifier_folder_path;

bool b_simpleFaces = false;
std::string img_dir;

std::vector<FeatureMask> _featureMasks;

ulong addFeatureMasks(FMF factory){
    ulong c=0;

    while(factory.hasNext()==1){
        _featureMasks.push_back( factory.next() );
        c++;
    }

    return c;
}

void teste(){
    std::string img1 = "/home/daniel/Projects/faces-rec/dataset/seinfeld.pgm";  //(588,285) - 
    std::string img2 = "/home/daniel/Projects/faces-rec/dataset/seinfeld2.pgm"; //(360,221) - corte 64 Topo; Corte 100 esquerda; Corte 128 direita
    std::string classifier_path = "/home/daniel/Projects/faces-rec/src_c/classifiers/state/classifier_5400_2_21";

    CascadeClassifier cl;
    cl.load(classifier_path);        

    Point wr;
    wr.x=64;
    wr.y=64;

    IntegralImage i1(img1);
    IntegralImage i2(img2);

    Point _ardis;
    _ardis.x = Config::ARDIS_WIDTH; _ardis.y = Config::ARDIS_HEIGHT;

    int _shift_w = Config::CLASSIFIER_SHIFT_STEP;
    int _shift_h = Config::CLASSIFIER_SHIFT_STEP;

    double _resize_factor = Config::CLASSIFIER_RESIZE_FACTOR;
    int _start_w = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    int _start_h = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;    

    double _final_fp_rate = Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE;
    double _final_det_rate = Config::CLASSIFIER_FINAL_DETECTION_RATE;

    double _max_fp_rate = Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE;
    double _min_det_rate = Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE;       

    MaskTwoHorizontalFactory m2hf(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    MaskTwoVerticalFactory m2vf(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    MaskThreeHorizontalFactory m3hf(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    MaskThreeVerticalFactory m3vf(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);    
    MaskDiagonalFactory mdf(_ardis,_shift_w,_shift_h,_resize_factor,_start_w,_start_h);        

    addFeatureMasks(m2hf);
    addFeatureMasks(m2vf);
    addFeatureMasks(m3hf);
    addFeatureMasks(m3vf);
    addFeatureMasks(mdf);    

    for(int m=0;m<_featureMasks.size();m++){

        for(int i=0;i<(221-64);i++){
            for(int j=0;j<(360-64);j++){
                Subwindow sw1 (j+100,64+i,wr,1,1);    
                Subwindow sw2 (j,i,wr,1,1);                            

                _featureMasks[m].adjustMask(sw1);
                ulong f1 = i1.filter(_featureMasks[m]);

                Point l1 = _featureMasks[m]._location;

                _featureMasks[m].adjustMask(sw2);
                ulong f2 = i2.filter(_featureMasks[m]);

                Point l2 = _featureMasks[m]._location;                

                if(f1!=f2){
                    printf("ERRO. %lu %lu %d %d\n");
                    printf("LOC (%d, %d) - (%d, %d)\n", l1.x,l1.y, l2.x,l2.y);
                }
            }
        }
    }
    
}

int detectSimpleFaces(std::string img_dir, std::string classifier_path){
    std::vector<std::string> files;
    CascadeClassifier cl;
    cl.load(classifier_path);    

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

    printf("FACES DETECTED %d/%d\n",faces_count,files.size());
}

void detectFaces(std::string classifier_path){
    printf("Detection started for: %s\n\n", classifier_path.c_str());
    CascadeClassifier cc;
    cc.load(classifier_path);

    Point ref_mask;
    ref_mask.x = Config::CLASSIFIER_SUBWINDOW_START_WIDTH;
    ref_mask.y = Config::CLASSIFIER_SUBWINDOW_START_HEIGHT;
    
    Point ardis;
    ardis.x = Config::ARDIS_WIDTH;
    ardis.y = Config::ARDIS_HEIGHT;

    Detector d(Config::DETECTOR_GENERATIONS,ardis,ref_mask,Config::CLASSIFIER_SHIFT_STEP);
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
    char* options = "s:w:h:g:a:b:c:d:";
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
                b_simpleFaces = false;
                classifier_path = std::string(optarg);
                break;                
            case 'b':
                b_detectFaces = false;
                b_testClassifiers = true;
                b_simpleFaces = false;
                classifier_folder_path = std::string(optarg);
                break;         
            case 'c':
                b_detectFaces = false;
                b_testClassifiers = false;
                b_simpleFaces = true;

                img_dir = std::string(optarg);
                break;    
            case 'd':
                classifier_path = std::string(optarg);
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
    }else{
        detectSimpleFaces(img_dir,classifier_path);
    }
    
    return 0;
}
