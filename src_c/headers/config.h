#ifndef __CONFIG__
#define __CONFIG__

#include <string>

class Config{
public:
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

    static int MAX_LENGHT_VALIDATION_SET;
    static int MAX_HYPOTHESIS_PER_STAGE;

    static int ARDIS_WIDTH;
    static int ARDIS_HEIGHT;

    static int INTEGRAL_IMAGE_BUFFER_SIZE;
    static bool HAS_BUFFER;

    static bool TEST_EXECUTION;

    static std::string PROJECT_PATH;

    static std::string LOG_PATH;

    static std::string DATASET_PATH;

    static std::string ATT_FACES_PATH;

    static std::string TRAINING_IMG_PATH;
    static std::string VALIDATION_IMG_PATH;

    static std::string TEST_IMG_PATH;    
    static std::string TRAINING_TEST_IMG_PATH;
    static std::string VALIDATION_TEST_IMG_PATH;    
    
    static std::string TEST_FACES_PATH;
    static std::string TRAINING_FACES_PATH;
    static std::string TRAINING_SCENES_PATH;
    
    static std::string VALIDATION_FACES_PATH;
    static std::string VALIDATION_SCENES_PATH;

    static std::string TRAINING_FACES_TEST_PATH;
    static std::string TRAINING_SCENES_TEST_PATH;

    static std::string VALIDATION_FACES_TEST_PATH;
    static std::string VALIDATION_SCENES_TEST_PATH;

    static std::string GRAPH_PATH;

    static std::string CLASSIFIERS_PATH;
    static std::string CLASSIFIERS_3500_PATH;
    static std::string CLASSIFIERS_10000_PATH;

    static std::string STATES_PATH;
    static std::string RESULTSET_PATH;
};

#endif