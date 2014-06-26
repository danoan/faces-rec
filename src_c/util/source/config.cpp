#include "../headers/config.h"

//LIVRE:    y,z

std::string Config::CONFIG_FILE;

int Config::ARDIS_WIDTH = 64;   //"a"
int Config::ARDIS_HEIGHT = 64;  //"b"

int Config::CLASSIFIER_SHIFT_STEP = 3;  //"s"
double Config::CLASSIFIER_RESIZE_FACTOR = 1.25; //"r"
int Config::CLASSIFIER_SUBWINDOW_START_WIDTH = 6;   //"w"
int Config::CLASSIFIER_SUBWINDOW_START_HEIGHT = 6;  //"h"

double Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE = 1e-14;    //"f"
double Config::CLASSIFIER_FINAL_DETECTION_RATE = 0.9;           //"d"

double Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE = 0.7;  //"m"
double Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE = 0.946;     //"t"

int Config::CLASSIFIER_MAX_STAGES = 38; //"n"
double Config::CLASSIFIER_BETA_MIN_VALUE = 1e-8;    //"NONE"

int Config::CLASSIFIER_MAX_HYPOTHESIS_PER_STAGE = 300;      //"x"

int Config::CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE = 2000;   //"i"
bool Config::CLASSIFIER_HAS_BUFFER = false;                 //"u"


int Config::DETECTOR_GENERATIONS = 3;   //"g"
int Config::DETECTOR_SHIFT_STEP = 3;    //"j"

int Config::DETECTOR_SUBWINDOW_START_WIDTH = 6;     //"k"
int Config::DETECTOR_SUBWINDOW_START_HEIGHT = 6;    //"l"

int Config::VIEWER_FEATURE_ID;  //e

std::string Config::DETECTOR_CLASSIFIER_PATH;   //p
std::string Config::DETECTOR_IMG_DIR;   
bool Config::DETECTOR_DETECT_FACES;     //q
bool Config::DETECTOR_SIMPLE_FACES;     //o
int Config::DETECTOR_DRAW = 0;             //v

// std::string Config::PROJECT_PATH = "/home/users/danielantunes/Documentos/faces-rec";
std::string Config::PROJECT_PATH = "/home/daniel/Projects/faces-rec";
std::string Config::LOG_PATH = PROJECT_PATH + "/log";
std::string Config::CLASSIFIERS_PATH = PROJECT_PATH + "/classifiers";
std::string Config::STATES_PATH = CLASSIFIERS_PATH + "/states";
std::string Config::DATASET_PATH =  PROJECT_PATH + "/dataset";
std::string Config::ATT_FACES_PATH = DATASET_PATH + "/att_faces";  

std::string Config::TRAINING_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN/training_images";
std::string Config::VALIDATION_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN/validation_images";
std::string Config::TEST_IMG_PATH = DATASET_PATH + "/64x64/Instancia SUN/test_images";

std::string Config::TEST_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
std::string Config::TRAINING_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
std::string Config::TRAINING_SCENES_PATH = TRAINING_IMG_PATH + "/non_faces";

std::string Config::VALIDATION_FACES_PATH = VALIDATION_IMG_PATH + "/faces";
std::string Config::VALIDATION_SCENES_PATH = VALIDATION_IMG_PATH + "/non_faces";    


int Config::readInput(int argc, char* argv[]){
    char* options = "a:b:s:r:w:h:f:d:m:t:n:x:i:ug:j:k:l:c:e:p:o:q:v?";
    int c=0;
    while(1){
        c = getopt(argc,argv,options);
        if(c<0) break;
        switch(c){
            case 'c':   //CONFIGURATION FILE
                Config::CONFIG_FILE = std::string(optarg);
                Config::readConfigFile(Config::CONFIG_FILE);
                break;
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
            case 'a':   //ARDIS_WIDTH
                Config::ARDIS_WIDTH = atoi(optarg);
                break;                             
            case 'b':   //ARDIS_HEIGHT
                Config::ARDIS_HEIGHT = atoi(optarg);
                break;                             
            case 'i':   //II Buffer Size
                Config::CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE = atoi(optarg);
                break;     
            case 'u':   //USA BUFFER
                Config::CLASSIFIER_HAS_BUFFER = true;
                break;           
            case 'x':   //MAXIMOS DE ESTAGIOS POR HIPOTESE
                Config::CLASSIFIER_MAX_HYPOTHESIS_PER_STAGE = atoi(optarg);
                break;        
            case 'g':
                Config::DETECTOR_GENERATIONS = atoi(optarg);
                break;
            case 'j':
                Config::DETECTOR_SHIFT_STEP = atoi(optarg);
                break;
            case 'k':
                Config::DETECTOR_SUBWINDOW_START_WIDTH = atoi(optarg);
                break;
            case 'l':
                Config::DETECTOR_SUBWINDOW_START_HEIGHT = atoi(optarg);
                break;
            case 'e':
                Config::VIEWER_FEATURE_ID = atoi(optarg);
                break;
            case 'p':
                Config::DETECTOR_CLASSIFIER_PATH = std::string(optarg);
                break;                     
            case 'o':
                Config::DETECTOR_DETECT_FACES = true;
                Config::DETECTOR_SIMPLE_FACES = false;
                Config::DETECTOR_CLASSIFIER_PATH = std::string(optarg);
                break;                       
            case 'q':
                Config::DETECTOR_DETECT_FACES = false;
                Config::DETECTOR_SIMPLE_FACES = true;

                Config::DETECTOR_IMG_DIR = std::string(optarg);
                break;                      
            case 'v':
                Config::DETECTOR_DRAW = 1;
                break;                   
            case '?':
                printf("-c Config File\n");
                printf("-s Shift Step\n");
                printf("-r Resize Factor\n");
                printf("-w Subwindow Start Width\n");
                printf("-h Subwindow Start Height\n");

                printf("-f Final False Positive Rate\n");
                printf("-d Final Detection Rate\n");
                printf("-m Max Stage False Positive Rate\n");
                printf("-t Min Stage Detection Rate\n");
                printf("-n Max Stages\n");
                printf("-x Max Hypothesis per Stage \n");

                printf("-a Ardis Width \n");
                printf("-b Ardis Height \n");
                printf("-i Buffer Size \n");
                printf("-u Usa Buffer \n");

                printf("-g Detector Generations \n");
                printf("-j Detector Shift Step \n");
                printf("-k Detector Subwindow Start Width \n");
                printf("-l Detector Subwindow Start Height \n");

                printf("-e Viewer Feature Id \n");

                printf("-p Detector Classifier Path \n");
                printf("-o Detector Detect Faces \n");
                printf("-q Detector Simple Faces \n");
                printf("-v Detector Draw Rectangles \n");
                return -1;
                
                break;
        }
    }

    return 1;
}

int Config::readParam(std::ifstream& ifs, std::string prefix,std::string& p){
    std::string name;
    ifs >> name;
    ifs >> p;

    p = prefix + p;

    // printf("%s: %s\n", name.c_str(),p.c_str());
}

int Config::readConfigFile(std::string file_path){
    std::ifstream ifs(file_path.c_str());

    readParam(ifs,"",Config::PROJECT_PATH);
    readParam(ifs,Config::PROJECT_PATH, Config::LOG_PATH);
    readParam(ifs,Config::PROJECT_PATH, Config::CLASSIFIERS_PATH);
    readParam(ifs,Config::CLASSIFIERS_PATH, Config::STATES_PATH);
    readParam(ifs,Config::Config::PROJECT_PATH, Config::DATASET_PATH);
    readParam(ifs,Config::Config::DATASET_PATH, Config::ATT_FACES_PATH);

    readParam(ifs,Config::DATASET_PATH, Config::TRAINING_IMG_PATH);
    readParam(ifs,Config::DATASET_PATH, Config::VALIDATION_IMG_PATH);
    readParam(ifs,Config::DATASET_PATH, Config::TEST_IMG_PATH);

    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TEST_FACES_PATH);
    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TRAINING_FACES_PATH);
    readParam(ifs,Config::TRAINING_IMG_PATH, Config::TRAINING_SCENES_PATH);

    readParam(ifs,Config::VALIDATION_IMG_PATH, Config::VALIDATION_FACES_PATH);
    readParam(ifs,Config::VALIDATION_IMG_PATH, Config::VALIDATION_SCENES_PATH);

    ifs.close();    
}
