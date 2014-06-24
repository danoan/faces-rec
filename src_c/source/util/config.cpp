#include "../../headers/util/config.h"

int Config::CLASSIFIER_SHIFT_STEP = 3;
double Config::CLASSIFIER_RESIZE_FACTOR = 1.25;
int Config::CLASSIFIER_SUBWINDOW_START_WIDTH = 6;
int Config::CLASSIFIER_SUBWINDOW_START_HEIGHT = 6;

double Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE = 1e-14;
double Config::CLASSIFIER_FINAL_DETECTION_RATE = 0.9;

double Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE = 0.7;
double Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE = 0.946;

int Config::CLASSIFIER_MAX_STAGES = 38;
double Config::CLASSIFIER_BETA_MIN_VALUE = 1e-8;

int Config::MAX_LENGHT_VALIDATION_SET = 6000;
int Config::MAX_HYPOTHESIS_PER_STAGE = 300;

int Config::ARDIS_WIDTH = 64;
int Config::ARDIS_HEIGHT = 64;

int Config::DETECTOR_GENERATIONS = 3;

int Config::INTEGRAL_IMAGE_BUFFER_SIZE = 2000;
bool Config::HAS_BUFFER = false;

bool Config::TEST_EXECUTION = false;

// std::string Config::PROJECT_PATH = "/home/users/danielantunes/Documentos/faces-rec";
std::string Config::PROJECT_PATH = "/home/daniel/Projects/faces-rec";

std::string Config::LOG_PATH = PROJECT_PATH + "/log";

std::string Config::DATASET_PATH =  PROJECT_PATH + "/dataset";

std::string Config::ATT_FACES_PATH = DATASET_PATH + "/att_faces";  

std::string Config::TRAINING_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN/training_images";
std::string Config::VALIDATION_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN/validation_images";

std::string Config::TEST_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN_2400_3200/test_images";
std::string Config::TRAINING_TEST_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN_2400_3200/training_images";
std::string Config::VALIDATION_TEST_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN_2400_3200/validation_images";



std::string Config::TEST_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
std::string Config::TRAINING_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
std::string Config::TRAINING_SCENES_PATH = TRAINING_IMG_PATH + "/non_faces";

std::string Config::VALIDATION_FACES_PATH = VALIDATION_IMG_PATH + "/faces";
std::string Config::VALIDATION_SCENES_PATH = VALIDATION_IMG_PATH + "/non_faces";    

std::string Config::TRAINING_FACES_TEST_PATH =  TRAINING_TEST_IMG_PATH + "/faces";    
std::string Config::TRAINING_SCENES_TEST_PATH = TRAINING_TEST_IMG_PATH + "/non_faces";

std::string Config::VALIDATION_FACES_TEST_PATH = VALIDATION_TEST_IMG_PATH + "/faces";
std::string Config::VALIDATION_SCENES_TEST_PATH = VALIDATION_TEST_IMG_PATH + "/non_faces";        

std::string Config::GRAPH_PATH = PROJECT_PATH + "/resultados/graficos";

std::string Config::CLASSIFIERS_PATH = PROJECT_PATH + "/src_c/classifiers";
std::string Config::CLASSIFIERS_3500_PATH = CLASSIFIERS_PATH + "/3500";
std::string Config::CLASSIFIERS_10000_PATH = CLASSIFIERS_PATH + "/10000";

std::string Config::STATES_PATH = CLASSIFIERS_PATH + "/states";
std::string Config::RESULTSET_PATH = PROJECT_PATH + "/resultset";

int readInput(int argc, char* argv[]){
    char* options = "s:r:w:h:f:d:m:t:n:v:a:b:i:uzx:p:";
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
            case 'u':   //USA BUFFER
                Config::HAS_BUFFER = true;
                break;           
            case 'z':   //USA BANCO DE TESTE
                Config::TEST_EXECUTION = true;
                break;                           
            case 'x':
                Config::MAX_HYPOTHESIS_PER_STAGE = atoi(optarg);
                break;        
            case 'p':                
                Config::TEST_IMG_PATH = Config::DATASET_PATH + "/" + optarg + "/test_images";
                Config::TRAINING_TEST_IMG_PATH = Config::DATASET_PATH + "/" + optarg + "/training_images";
                Config::VALIDATION_TEST_IMG_PATH = Config::DATASET_PATH + "/" + optarg + "/validation_images";                                   

                Config::TRAINING_FACES_TEST_PATH =  Config::TRAINING_TEST_IMG_PATH + "/faces";    
                Config::TRAINING_SCENES_TEST_PATH = Config::TRAINING_TEST_IMG_PATH + "/non_faces";

                Config::VALIDATION_FACES_TEST_PATH = Config::VALIDATION_TEST_IMG_PATH + "/faces";
                Config::VALIDATION_SCENES_TEST_PATH = Config::VALIDATION_TEST_IMG_PATH + "/non_faces";                  
                break;
        }
    }

    return 1;
}
