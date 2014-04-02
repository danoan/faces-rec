#coding:utf-8

from trainning_tools import *
from detector import *
import os,pickle,random

n=10

folder_faces = "dataset/lfwcrop_grey/faces"
folder_scenes = "dataset/training non-face images"
folder_test_img = "dataset/test_images"

classifier = None
with open('classifiers/classifier_10_2.pk','rb') as input:
	classifier = pickle.load(input)

print classifier.hypothesis

#Creating the Detector
ng = 4
ref_ardis = (64,64)
ref_mask = (8,8)

shift_step=4
fn=SubwindowGenerator.FIXED_FACTOR

det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step,fn=fn)	

def test_classifier():
	face_filenames = random.sample(os.listdir(folder_faces),n)

	sw = Subwindow(0,0,(64,64))

	true=0
	for i in face_filenames:
		img = Image.open( "%s/%s" % (folder_faces,i) )
		true = true+1 if det.is_face(img,sw) else true
	
	print "%d/%d" % (true,n)

	true=0
	for scene_window in get_next_random_image_window(folder_scenes,n):
		img = scene_window.window_img
		true = true+1 if det.is_face(img,sw) else true		

	print "%d/%d" % (true,n)	

def test_detector():
	img1 = Image.open("%s/%s" % (folder_test_img,"1.pgm"))
	det.detect_faces(img1)

	# img2 = Image.open("%s/%s" % (folder_test_img,"3.pgm"))
	# det.detect_faces(img2)

# for h in classifier.hypothesis:
# 	print h[0],h[1],h[2],h[4]

# test_classifier()
test_detector()