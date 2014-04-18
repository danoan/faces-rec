#coding:utf-8

PROJECT_PATH = "/home/daniel/Projects/faces-rec"


DATASET_PATH = "%s/dataset" % (PROJECT_PATH,)

FACES_PATH = "%s/lfwcrop_grey/faces" % (DATASET_PATH,)
SCENES_PATH = "%s/training non-face images"  % (DATASET_PATH,)
TEST_IMG_PATH = "%s/test_images"  % (DATASET_PATH,)	

CLASSIFIERS_PATH = "%s/classifiers" %(PROJECT_PATH,)
STATES_PATH = "%s/classifiers/states" % (PROJECT_PATH,)
RESULTSET_PATH = "%s/resultset" % (PROJECT_PATH,)