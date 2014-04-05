#coding:utf-8

import os,pickle,random

import windowgen,config
from trainning_tools import *
from detector import *

n=10

folder_faces = "%s/lfwcrop_grey/faces" % (config.DATASET_PATH,)
folder_scenes = "%s/training non-face images"  % (config.DATASET_PATH,)
folder_test_img = "%s/test_images"  % (config.DATASET_PATH,)

classifier = None
with open('%s/%s' % (config.CLASSIFIERS_PATH,'classifier_2_2.pk'),'rb') as input:
	classifier = pickle.load(input)

print classifier.hypothesis

#Creating the Detector
ng = 4
ref_ardis = (64,64)
ref_mask = (8,8)

shift_step=4
fn=windowgen.SubwindowGenerator.FIXED_FACTOR

det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step,fn=fn)	

def test_classifier():
	face_filenames = random.sample(os.listdir(folder_faces),n)

	sw = windowgen.Subwindow(0,0,(64,64))

	true=0
	for i in face_filenames:
		img_path = "%s/%s" % (folder_faces,i)
		true = true+1 if det.is_face(img_path,sw) else true
	
	print "%d/%d" % (true,n)

	true=0
	for scene_window in windowgen.get_next_random_image_window(folder_scenes,n):
		img_path = scene_window.window_img_path
		true = true+1 if det.is_face(img_path,sw) else true		

	print "%d/%d" % (true,n)	

def test_detector():
	det.detect_faces("%s/%s" % (folder_test_img,"1.pgm"))

	# det.detect_faces("%s/%s" % (folder_test_img,"3.pgm"))

# for h in classifier.hypothesis:
# 	print h[0],h[1],h[2],h[4]

# test_classifier()
test_detector()