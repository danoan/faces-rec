#coding:utf-8

import os,pickle,random

import windowgen,config
from trainning_tools import *
from detector import *
import labeled_images

class Experiment():	
	def setup(self):
		classifier_filepath = 'classifier_3500_(54-100).pk'
		n_gen = 6
		ref_ardis = (64,64)
		ref_mask = (8,8)
		ss=16
		window_strategy=windowgen.SubwindowGenerator.FIXED_FACTOR

		with open('%s/%s' % (config.CLASSIFIERS_PATH,classifier_filepath),'rb') as input:
			classifier = pickle.load(input)

		self.det = Detector(classifier,n_gen,ref_ardis,ref_mask,shift_step=ss,fn=window_strategy)			

		return self

	def run(self):
		pass


class Experiment1(Experiment):

	def setup(self):
		return Experiment.setup(self)

	def run(self):
		li = labeled_images.li1()
		dr = DetectorReport(li,"EXPERIMENT 1")
		return self.det.detect_faces(li.image_filepath(),dr)


class Experiment2(Experiment):

	def setup(self):
		Experiment.setup(self)
		self.n_img = 10000
		return self

	def run(self):
		face_filenames = random.sample(os.listdir(config.FACES_PATH),self.n_img)

		sw = windowgen.Subwindow(0,0,(64,64))

		true=0
		for i in face_filenames:
			img_path = "%s/%s" % (config.FACES_PATH,i)
			true = true+1 if self.det.is_face(img_path,sw) else true
		
		print "%d/%d" % (true,self.n_img)

		true=0
		for scene_window in windowgen.get_next_random_image_window(config.SCENES_PATH,self.n_img):
			img_path = scene_window.window_img_path
			true = true+1 if self.det.is_face(img_path,sw) else true		

		print "%d/%d" % (true,self.n_img)			


if __name__=="__main__":
	# print Experiment1().setup().run()
	print Experiment2().setup().run()


