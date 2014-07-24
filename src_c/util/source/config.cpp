#include "../headers/config.h"

//LIVRE:  g,p,o,q,z

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

int Config::CLASSIFIER_FACES_TRAINING_SET_SIZE = 300; //"j"
int Config::CLASSIFIER_SCENES_TRAINING_SET_SIZE = 400; //"k"
int Config::CLASSIFIER_FACES_VALIDATION_SET_SIZE = 100;	//v
int Config::CLASSIFIER_SCENES_VALIDATION_SET_SIZE = 200;	//y
int Config::CLASSIFIER_SCENES_CROPS_FIRST_CLASSIFIER = 10000; //l

int Config::CLASSIFIER_MAX_HYPOTHESIS_PER_STAGE = 300;      //"x"

int Config::CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE = 2000;   //"i"
bool Config::CLASSIFIER_HAS_BUFFER = false;                 //"u"

int Config::CLASSIFIER_SET_CROP_BUFFER_SIZE = 10;
int Config::CLASSIFIER_SET_CROP_ELEMENTS_PER_WINDOW = 50;

bool Config::CLASSIFIER_CUDA = false;	//g


int Config::DETECTOR_GENERATIONS = 3;   //"G"
int Config::DETECTOR_SHIFT_STEP = 3;    //"S"

int Config::DETECTOR_SUBWINDOW_START_WIDTH = 6;     //"W"
int Config::DETECTOR_SUBWINDOW_START_HEIGHT = 6;    //"H"

std::string Config::DETECTOR_CLASSIFIER_PATH;   //P
std::string Config::DETECTOR_IMG_DIR;   
std::string Config::DETECTOR_IMG_PATH;  //Z
bool Config::DETECTOR_DETECT_FACES;     //D
bool Config::DETECTOR_SIMPLE_FACES;     //F
int Config::DETECTOR_DRAW = 0;             //R
double Config::DETECTOR_SCAN_WINDOW_FACTOR = 0.0;             //C

int Config::VIEWER_FEATURE_ID;  //e


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
    char* options = "c:a:b:s:r:w:h:f:d:m:t:n:x:i:ue:j:k:v:y:l:gG:S:W:H:P:O:Q:RC:Z:?";
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
            case 'j':	//NUMERO DE FACES NO CONJUNTO DE TREINAMENTO
                Config::CLASSIFIER_FACES_TRAINING_SET_SIZE = atoi(optarg);
                break;                                                            
            case 'k':	//NUMERO DE SCENE CROPS NO CONJUNTO DE TREINAMENTO
                Config::CLASSIFIER_SCENES_TRAINING_SET_SIZE = atoi(optarg);
                break;     
            case 'v':	//NUMERO DE FACES NO CONJUNTO DE VALIDACAO
                Config::CLASSIFIER_FACES_VALIDATION_SET_SIZE = atoi(optarg);
                break;     
            case 'y':	//NUMERO DE SCENE CROPS NO CONJUNTO DE VALIDACAO
                Config::CLASSIFIER_SCENES_VALIDATION_SET_SIZE = atoi(optarg);
                break;
            case 'l':	//NUMERO DE SCENE CROPS NO CONJUNTO DE TREINAMENTO DO PRIMEIRO CLASSIFICADOR (COSTUMA SER ALTO: 10000)
                CLASSIFIER_SCENES_CROPS_FIRST_CLASSIFIER = atoi(optarg);
                break;                                                                                                                   
            case 'e':
                Config::VIEWER_FEATURE_ID = atoi(optarg);
                break;                
            case 'g':
                Config::CLASSIFIER_CUDA = true;
                break;                                
            case 'G':
                Config::DETECTOR_GENERATIONS = atoi(optarg);
                break;
            case 'S':
                Config::DETECTOR_SHIFT_STEP = atoi(optarg);
                break;
            case 'W':
                Config::DETECTOR_SUBWINDOW_START_WIDTH = atoi(optarg);
                break;
            case 'H':
                Config::DETECTOR_SUBWINDOW_START_HEIGHT = atoi(optarg);
                break;
            case 'P':
                Config::DETECTOR_CLASSIFIER_PATH = std::string(optarg);
                break;                     
            case 'O':
                Config::DETECTOR_DETECT_FACES = true;
                Config::DETECTOR_SIMPLE_FACES = false;
                Config::DETECTOR_CLASSIFIER_PATH = std::string(optarg);
                break;                       
            case 'Q':
                Config::DETECTOR_DETECT_FACES = false;
                Config::DETECTOR_SIMPLE_FACES = true;

                Config::DETECTOR_IMG_DIR = std::string(optarg);
                break;                      
            case 'R':
                Config::DETECTOR_DRAW = 1;
                break;                                   
            case 'C':
                Config::DETECTOR_SCAN_WINDOW_FACTOR = atof(optarg);
                break;                                   
            case 'Z':
                Config::DETECTOR_IMG_PATH = std::string(optarg);
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
                printf("-j Faces Training Set Size \n");
                printf("-k Scenes Training Set Size \n");
                printf("-v Faces Validation Set Size \n");
                printf("-y Scenes Validation Set Size \n");
                printf("-l Scenes crops Training set for first classifier \n");

                printf("-a Ardis Width \n");
                printf("-b Ardis Height \n");
                printf("-i Buffer Size \n");
                printf("-u Usa Buffer \n");

                printf("-G Detector Generations \n");
                printf("-S Detector Shift Step \n");
                printf("-W Detector Subwindow Start Width \n");
                printf("-H Detector Subwindow Start Height \n");

                printf("-e Viewer Feature Id \n");

                printf("-P Detector Classifier Path \n");
                printf("-O Detector Detect Faces \n");
                printf("-Q Detector Simple Faces \n");
                printf("-R Detector Draw Rectangles \n");
                printf("-C Detector Subwindow Scan Factor \n");
                printf("-Z Detector Image Path for Detect Faces \n");
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
