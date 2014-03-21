#coding:utf-8

from trainning_tools import *
from detector import *
import os,pickle,random

folder_faces = "/home/daniel/Desktop/lfwcrop_grey/faces"
folder_scenes = "/home/daniel/Desktop/training non-face images"
n = 25
T = 2

n_faces = n
n_non_faces = n

face_filenames = os.listdir(folder_faces)[:n]

faces = []
non_faces = []

'''
	Create a FeatureMaster instance for each image face exemplar and puts the correspondent generator
	in the appropriated list. 
'''
FeatureMaster.init_training_set()
fm = FeatureMaster((64,64),4,4,1.25,8,8)
for face_file in face_filenames:
	path = "%s/%s" % (folder_faces,face_file)
	im = Image.open(path)
	faces.append(im)
	fm.add_image(im,FeatureMaster.FACE)

'''It does the same for the scene images'''
for scene_window in get_next_random_image_window(folder_scenes,n):
	im = scene_window.window_img
	non_faces.append(im)
	fm.add_image(im,FeatureMaster.NON_FACE)


fc = FeatureChooser((64,64),fm,n_faces,n_non_faces)
classifier = fc.find_hypothesis(T)
with open('classifier.pk','wb') as output:
	pickle.dump(classifier,output,pickle.HIGHEST_PROTOCOL)

print "FIM"

#Creating the Detector
ng = 1
ref_ardis = (64,64)
ref_mask = (8,8)

shift_step=4
fn=SubwindowGenerator.FIXED_FACTOR

det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step)	

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


