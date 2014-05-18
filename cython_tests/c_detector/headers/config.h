#ifndef __CONFIG__
#define __CONFIG__

namespace Config{

    const string PROJECT_PATH = "/home/daniel/Projects/faces-rec";

    const string DATASET_PATH =  PROJECT_PATH + "/dataset";
    const string TRAINING_IMG_PATH = DATASET_PATH + "/training_images";
    const string VALIDATION_IMG_PATH = DATASET_PATH + "/validation_images";
    const string TEST_IMG_PATH = DATASET_PATH + "/test_images";

    const string FACES_PATH =  TRAINING_IMG_PATH + "/faces/lfwcrop_grey/faces";
    const string ATT_FACES_PATH = TRAINING_IMG_PATH + "/faces/att_faces";
    const string SCENES_PATH = TRAINING_IMG_PATH + "/non_faces";
    const string SCENES_PATH_VALIDATION = VALIDATION_IMG_PATH + "/non_faces";

    const string GRAPH_PATH = PROJECT_PATH + "/resultados/graficos";

    const string CLASSIFIERS_PATH = PROJECT_PATH + "/classifiers";
    const string CLASSIFIERS_3500_PATH = CLASSIFIERS_PATH + "/3500";
    const string CLASSIFIERS_10000_PATH = CLASSIFIERS_PATH + "/10000";

    const string STATES_PATH = PROJECT_PATH + "/classifiers/states";
    const string RESULTSET_PATH = PROJECT_PATH + "/resultset";
}

#endif