#include <cstdio>
#include <string>
#include "/home/daniel/Projects/faces-rec/src_c/headers/libviola.h"

#define MAX_SHIFT_STEP 1
#define MAX_RESIZE_FACTOR 3
#define MAX_DATASET 2

int shiftStep[MAX_SHIFT_STEP] = {1};
double resizeFactor[MAX_RESIZE_FACTOR] = {1.25,1.5,2.0};
int validationSet[MAX_DATASET] = {400,800};
// std::string dataset[MAX_DATASET] = {"24x24/Instancia SUN_300_400","24x24/Instancia SUN_600_800"};
std::string dataset[MAX_DATASET] = {"24x24/Instancia SUN_8000_6000"};


double detectSimpleFaces(std::string img_dir, CascadeClassifier cl){
    std::vector<std::string> files;

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

    return (1.0*faces_count/files.size());
}

void call(){
    std::string vs_face_path, vs_scene_path;
    std::string ts_face_path, ts_scene_path;

    vs_face_path = Config::VALIDATION_FACES_TEST_PATH;
    vs_scene_path = Config::VALIDATION_SCENES_TEST_PATH;

    ts_face_path = Config::TRAINING_FACES_TEST_PATH;
    ts_scene_path = Config::TRAINING_SCENES_TEST_PATH;

    ValidationSet vs(vs_face_path,vs_scene_path);
    TrainingSet ts(ts_face_path,ts_scene_path);

    struct timeval call_stop,call_start;
    gettimeofday(&call_start,NULL);
    
    Trainer* t = new Trainer(ts,vs);
    CascadeClassifier cl = t->startTrainingCascade();        

    gettimeofday(&call_stop,NULL);
    
    ulong seg = (call_stop.tv_sec-call_start.tv_sec); 
    ulong useg = (call_stop.tv_usec-call_start.tv_usec);      

    int hipoteses=0;
    for(int i=0;i<cl._classifiers.size();i++){
        hipoteses += cl._classifiers[i]._hypothesis.size();
    }

    double det_rate = detectSimpleFaces(Config::DATASET_PATH + "/24x24/faces_validacao_analise",cl);
    double fp_rate = detectSimpleFaces(Config::DATASET_PATH + "/24x24/cenas_validacao_analise",cl);

    Logger::feat_anal->log("%d,%.2f,%dx%d,%d,%d,%d,%d,%d,%d,%d,%lu.%lu,%.2f,%.2f\n", Config::CLASSIFIER_SHIFT_STEP, Config::CLASSIFIER_RESIZE_FACTOR, Config::CLASSIFIER_SUBWINDOW_START_WIDTH, Config::CLASSIFIER_SUBWINDOW_START_HEIGHT,t->_facesFactory._facesFeatures.size(), hipoteses, cl._classifiers[0]._hypothesis[0]._fm._id, cl._classifiers[0]._hypothesis[1]._fm._id, cl._classifiers[0]._hypothesis[2]._fm._id, cl._classifiers[0]._hypothesis[3]._fm._id, cl._classifiers[0]._hypothesis[4]._fm._id, seg, useg, det_rate, fp_rate);

    delete t;
}

int main(int argc, char* argv[]){
    if(readInput(argc,argv)!=1) return 1;
    Logger::init("analysis");    

    for(int p=0;p<MAX_DATASET;p++){
        for(int r=0;r<MAX_RESIZE_FACTOR;r++){
            for(int s=0;s<MAX_SHIFT_STEP;s++){
                Config::CLASSIFIER_SHIFT_STEP = shiftStep[s];
                Config::CLASSIFIER_RESIZE_FACTOR = resizeFactor[r];
                Config::MAX_LENGHT_VALIDATION_SET = validationSet[p];

                Config::TEST_IMG_PATH = Config::DATASET_PATH + "/" + dataset[p] + "/test_images";
                Config::TRAINING_TEST_IMG_PATH = Config::DATASET_PATH + "/" + dataset[p] + "/training_images";
                Config::VALIDATION_TEST_IMG_PATH = Config::DATASET_PATH + "/" + dataset[p] + "/validation_images";                                   

                Config::TRAINING_FACES_TEST_PATH =  Config::TRAINING_TEST_IMG_PATH + "/faces";    
                Config::TRAINING_SCENES_TEST_PATH = Config::TRAINING_TEST_IMG_PATH + "/non_faces";

                Config::VALIDATION_FACES_TEST_PATH = Config::VALIDATION_TEST_IMG_PATH + "/faces";
                Config::VALIDATION_SCENES_TEST_PATH = Config::VALIDATION_TEST_IMG_PATH + "/non_faces";    

                call();   
            }
        }
    }
}

// ./bin/featureAnalysis.out -z -u -i 2000 -v 6000 -n 20 -m 0.4 -t 0.996 -a 24 -b 24 -x 40