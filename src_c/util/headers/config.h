#ifndef __CONFIG__
#define __CONFIG__

#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <string>

class Config{
public:
    static std::string CONFIG_FILE;
    static int ARDIS_WIDTH;
    static int ARDIS_HEIGHT;

    static int CLASSIFIER_SHIFT_STEP;
    static double CLASSIFIER_RESIZE_FACTOR;
    static int CLASSIFIER_SUBWINDOW_START_WIDTH;
    static int CLASSIFIER_SUBWINDOW_START_HEIGHT;

    static double CLASSIFIER_FINAL_FALSE_POSITIVE_RATE;
    static double CLASSIFIER_FINAL_DETECTION_RATE;

    static double CLASSIFIER_STAGE_MAX_FALSE_POSITIVE_RATE;
    static double CLASSIFIER_STAGE_MIN_DETECTION_RATE;

    static int CLASSIFIER_MAX_STAGES;
    static double CLASSIFIER_BETA_MIN_VALUE;

    static int CLASSIFIER_MAX_HYPOTHESIS_PER_STAGE;
    static int CLASSIFIER_SCENE_MIN_TRAINING_SET_ELEMENTS;

    static int CLASSIFIER_INTEGRAL_IMAGE_BUFFER_SIZE;
    static bool CLASSIFIER_HAS_BUFFER;

    static int DETECTOR_GENERATIONS;
    static int DETECTOR_SHIFT_STEP;

    static int DETECTOR_SUBWINDOW_START_WIDTH;
    static int DETECTOR_SUBWINDOW_START_HEIGHT;

    static std::string DETECTOR_CLASSIFIER_PATH;
    static std::string DETECTOR_IMG_DIR;
    static bool DETECTOR_DETECT_FACES;
    static bool DETECTOR_SIMPLE_FACES;
    static int DETECTOR_DRAW;
    static double DETECTOR_SCAN_WINDOW_FACTOR;
    static std::string DETECTOR_IMG_PATH;

    static int VIEWER_FEATURE_ID;

    static std::string PROJECT_PATH;
    static std::string LOG_PATH;
    static std::string CLASSIFIERS_PATH;
    static std::string STATES_PATH;
    static std::string DATASET_PATH;
    static std::string ATT_FACES_PATH;

    static std::string TRAINING_IMG_PATH;
    static std::string VALIDATION_IMG_PATH;
    static std::string TEST_IMG_PATH;

    static std::string TEST_FACES_PATH;
    static std::string TRAINING_FACES_PATH;
    static std::string TRAINING_SCENES_PATH;

    static std::string VALIDATION_FACES_PATH;
    static std::string VALIDATION_SCENES_PATH;

    static int readInput(int argc, char* argv[]);
    static int readConfigFile(std::string file_path);
    static int readParam(std::ifstream& ifs, std::string prefix,std::string& p);
};



#endif