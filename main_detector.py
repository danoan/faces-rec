#coding:utf-8

from trainning_tools import *
from detector import *
import os,pickle,random

n=25

folder_faces = "/home/daniel/Desktop/lfwcrop_grey/faces"
folder_scenes = "/home/daniel/Desktop/training non-face images"
folder_test_img = "/home/daniel/Desktop/test_images"

classifier = None
with open('classifier.pk','rb') as input:
	classifier = pickle.load(input)

print classifier.hypothesis

#Creating the Detector
ng = 4
ref_ardis = (64,64)
ref_mask = (8,8)

shift_step=8
fn=SubwindowGenerator.DYNAMIC_FACTOR

det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step,fn=fn)	

def test_classifier():
	face_filenames = random.sample(os.listdir(folder_faces),n)
	scene_filenames = random.sample(os.listdir(folder_scenes),n)

	faces = []
	non_faces = []
	for i in face_filenames:
		faces.append(Image.open( "%s/%s" % (folder_faces,i) ) )
	
	for scene_window in get_next_random_image_window(folder_scenes,n):
		im = scene_window.window_img
		non_faces.append(im)

	true=0
	for i in range(0,n):
		sw = Subwindow(0,0,(64,64))
		true = true+1 if det.is_face(faces[i],sw) else true

	print "%d/%d" % (true,n)

	true=0
	for i in range(0,n):
		sw = Subwindow(0,0,(64,64))
		true = true+1 if det.is_face(non_faces[i],sw) else true		

	print "%d/%d" % (true,n)	

def test_detector():
	img = Image.open("%s/%s" % (folder_test_img,"2.pgm"))
	det.detect_faces(img)

# for h in classifier.hypothesis:
# 	print h[0],h[1],h[2],h[4]

# test_classifier()
test_detector()