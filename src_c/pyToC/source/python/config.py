#coding:utf-8

PROJECT_PATH = "/home/daniel/Projects/faces-rec"


DATASET_PATH = "%s/dataset" % (PROJECT_PATH,)
TRAINING_IMG_PATH = "%s/training_images" % (DATASET_PATH,)
VALIDATION_IMG_PATH = "%s/validation_images" % (DATASET_PATH,)

FACES_PATH = "%s/faces/lfwcrop_grey/faces" % (TRAINING_IMG_PATH,)
ATT_FACES_PATH = "%s/faces/att_faces" % (TRAINING_IMG_PATH,)
SCENES_PATH = "%s/non_faces"  % (TRAINING_IMG_PATH,)
SCENES_PATH_VALIDATION = "%s/non_faces"  % (VALIDATION_IMG_PATH,)

TEST_IMG_PATH = "%s/test_images"  % (DATASET_PATH,)	
VALIDATION_IMG_PATH = "%s/validation_images"  % (DATASET_PATH,)	

GRAPH_PATH = "%s/resultados/graficos" % (PROJECT_PATH,)

CLASSIFIERS_PATH = "%s/classifiers" %(PROJECT_PATH,)
CLASSIFIERS_3500_PATH = "%s/3500" % (CLASSIFIERS_PATH,)
CLASSIFIERS_10000_PATH = "%s/10000" % (CLASSIFIERS_PATH,)

STATES_PATH = "%s/classifiers/states" % (PROJECT_PATH,)
RESULTSET_PATH = "%s/resultset" % (PROJECT_PATH,)