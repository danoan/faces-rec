#ifndef __CONFIG__
#define __CONFIG__

#include <string>

namespace Config{

    const std::string PROJECT_PATH = "/home/daniel/Projects/faces-rec";

    const std::string DATASET_PATH =  PROJECT_PATH + "/dataset";
    const std::string TRAINING_IMG_PATH = DATASET_PATH + "/training_images";
    const std::string VALIDATION_IMG_PATH = DATASET_PATH + "/validation_images";
    const std::string TEST_IMG_PATH = DATASET_PATH + "/test_images";

    const std::string ATT_FACES_PATH = TRAINING_IMG_PATH + "/faces/att_faces";
    
    const std::string TRAINING_FACES_PATH =  TRAINING_IMG_PATH + "/faces";        
    const std::string TRAINING_SCENES_PATH = TRAINING_IMG_PATH + "/non_faces";
    
    const std::string TRAINING_FACES_TEST_PATH =  TRAINING_IMG_PATH + "/test_faces";    
    const std::string TRAINING_SCENES_TEST_PATH = TRAINING_IMG_PATH + "/test_non_faces";

    const std::string TRAINING_FACES_TEST_1000_PATH =  TRAINING_IMG_PATH + "/test_faces_1000";    
    const std::string TRAINING_SCENES_TEST_1000_PATH = TRAINING_IMG_PATH + "/test_non_faces_1000";    
    
    const std::string VALIDATION_FACES_PATH = VALIDATION_IMG_PATH + "/faces";
    const std::string VALIDATION_SCENES_PATH = VALIDATION_IMG_PATH + "/non_faces";    

    const std::string VALIDATION_FACES_TEST_PATH = VALIDATION_IMG_PATH + "/test_faces";
    const std::string VALIDATION_SCENES_TEST_PATH = VALIDATION_IMG_PATH + "/test_non_faces";        

    const std::string GRAPH_PATH = PROJECT_PATH + "/resultados/graficos";

    const std::string CLASSIFIERS_PATH = PROJECT_PATH + "/classifiers";
    const std::string CLASSIFIERS_3500_PATH = CLASSIFIERS_PATH + "/3500";
    const std::string CLASSIFIERS_10000_PATH = CLASSIFIERS_PATH + "/10000";

    const std::string STATES_PATH = PROJECT_PATH + "/classifiers/states";
    const std::string RESULTSET_PATH = PROJECT_PATH + "/resultset";
}

#endif