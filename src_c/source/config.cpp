#include "../headers/config.h"

int Config::CLASSIFIER_SHIFT_STEP = 3;
double Config::CLASSIFIER_RESIZE_FACTOR = 1.25;
int Config::CLASSIFIER_SUBWINDOW_START_WIDTH = 6;
int Config::CLASSIFIER_SUBWINDOW_START_HEIGHT = 6;

double Config::CLASSIFIER_FINAL_FALSE_POSITIVE_RATE = 1e-4;
double Config::CLASSIFIER_FINAL_DETECTION_RATE = 0.9;

double Config::CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE = 0.7;
double Config::CLASSIFIER_STAGE_MIN_DETECTION_RATE = 0.946;

int Config::CLASSIFIER_MAX_STAGES = 1;
double Config::CLASSIFIER_BETA_MIN_VALUE = 1e-8;

int Config::MAX_LENGHT_VALIDATION_SET = 6000;

int Config::ARDIS_WIDTH = 64;
int Config::ARDIS_HEIGHT = 64;

int Config::INTEGRAL_IMAGE_BUFFER_SIZE = 5000;
bool Config::HAS_BUFFER = false;

bool Config::TEST_EXECUTION = false;

std::string Config::PROJECT_PATH = "/home/daniel/Projects/faces-rec";

std::string Config::LOG_PATH = PROJECT_PATH + "/log";

std::string Config::DATASET_PATH =  PROJECT_PATH + "/dataset";

std::string Config::ATT_FACES_PATH = DATASET_PATH + "/att_faces";
std::string Config::TEST_IMG_PATH = DATASET_PATH + "/test_images";    

std::string Config::TRAINING_IMG_PATH = DATASET_PATH + "/Instancia SUN/training_images";
std::string Config::VALIDATION_IMG_PATH = DATASET_PATH + "/Instancia SUN/validation_images";

std::string Config::TRAINING_TEST_IMG_PATH = DATASET_PATH + "/Instancia SUN_Teste/training_images";
std::string Config::VALIDATION_TEST_IMG_PATH = DATASET_PATH + "/Instancia SUN_Teste/validation_images";



std::string Config::TRAINING_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
std::string Config::TRAINING_SCENES_PATH = TRAINING_IMG_PATH + "/non_faces";

std::string Config::VALIDATION_FACES_PATH = VALIDATION_IMG_PATH + "/faces";
std::string Config::VALIDATION_SCENES_PATH = VALIDATION_IMG_PATH + "/non_faces";    

std::string Config::TRAINING_FACES_TEST_PATH =  TRAINING_TEST_IMG_PATH + "/faces";    
std::string Config::TRAINING_SCENES_TEST_PATH = TRAINING_TEST_IMG_PATH + "/non_faces";

std::string Config::VALIDATION_FACES_TEST_PATH = VALIDATION_TEST_IMG_PATH + "/faces";
std::string Config::VALIDATION_SCENES_TEST_PATH = VALIDATION_TEST_IMG_PATH + "/non_faces";        

std::string Config::GRAPH_PATH = PROJECT_PATH + "/resultados/graficos";

std::string Config::CLASSIFIERS_PATH = PROJECT_PATH + "/classifiers";
std::string Config::CLASSIFIERS_3500_PATH = CLASSIFIERS_PATH + "/3500";
std::string Config::CLASSIFIERS_10000_PATH = CLASSIFIERS_PATH + "/10000";

std::string Config::STATES_PATH = PROJECT_PATH + "/classifiers/states";
std::string Config::RESULTSET_PATH = PROJECT_PATH + "/resultset";