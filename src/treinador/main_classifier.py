#coding:utf-8

import os,pickle,random,requests

import windowgen,config
from trainning_tools import *
from detector import *


def init():

	folder_faces = "%s/lfwcrop_grey/faces" % (config.DATASET_PATH,)
	folder_scenes = "%s/training non-face images" % (config.DATASET_PATH,)
	n = 300
	T = 10

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
		img_path = "%s/%s" % (folder_faces,face_file)
		faces.append(img_path)
		fm.add_image(img_path,FeatureMaster.FACE)

	'''It does the same for the scene images'''
	for scene_window in windowgen.get_next_random_image_window(folder_scenes,n):
		img_path = scene_window.window_img_path
		non_faces.append(img_path)
		fm.add_image(img_path,FeatureMaster.NON_FACE)

	print "END STAGE 1"

	fc = FeatureChooser((64,64),fm,n_faces,n_non_faces)
	classifier = fc.find_hypothesis(T)

	with open('%s/%s_%d_%d.pk' % (config.CLASSIFIERS_PATH,'classifier',n,T),'wb') as output:
		pickle.dump(classifier,output,pickle.HIGHEST_PROTOCOL)	

	print "END STAGE 2"
	print classifier.hypothesis

	#Creating the Detector
	ng = 1
	ref_ardis = (64,64)
	ref_mask = (8,8)

	shift_step=4
	fn=windowgen.SubwindowGenerator.FIXED_FACTOR

	det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step)	

	true=0
	for i in range(0,n):
		sw = windowgen.Subwindow(0,0,(64,64))
		true = true+1 if det.is_face( faces[i],sw ) else true

	print "%d/%d" % (true,n)

	true=0
	for i in range(0,n):
		sw = windowgen.Subwindow(0,0,(64,64))
		true = true+1 if det.is_face( non_faces[i],sw) else true		

	print "%d/%d" % (true,n)	

def load_state():
	return FeatureChooser.find_hypothesis_from_previous_state("%s/featureChooser_3.pk" % (config.STATES_PATH,) )

def send_message(subject,msg):
	url = "https://mandrillapp.com/api/1.0/messages/send.json"
	data = {'key':'dLvJBwxABLl7dPtaA353BQ',
			'message[from_email]':'digaxisproducoes@gmail.com',
			'message[to][0][email]':'danoan2008@gmail.com',
			'message[to][0][name]':'Xis Produções',
			'message[to][0][type]':'to',
			'message[autotext]':'true',
			'message[subject]':subject,
			'message[html]':msg
			}

	requests.post(url,data)	


try:
	init()
except Exception as e:
	send_message("Deu Erro no Algoritmo!!!",e.message)
	raise e
else:
	send_message("O algoritmo terminou de rodar!!!","Corre Lah!!")
	pass

# print load_state().hypothesis