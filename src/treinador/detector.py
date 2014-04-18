#! usr/bin/python
#coding:utf-8

import Image,ImageDraw,math

import windowgen
from features import *

class Detector():

	def __init__(self,classifier,ng,ref_ardis,ref_mask,masks=[MaskTwoHorizontalFactory,
																MaskTwoVerticalFactory,
																MaskThreeHorizontalFactory,
																MaskThreeVerticalFactory,
																MaskDiagonalFactory],
				shift_step=1,fn=windowgen.SubwindowGenerator.FIXED_FACTOR):

		self.classifier = classifier
		self.ng = ng
		self.wr = ref_ardis
		self.ref_mask = ref_mask

		self.masks = masks
		self.shift_step = shift_step		
		self.fn = fn

	def detect_faces(self,img_path,dr):
		self.classifier.set_image(img_path)
		subwindows = windowgen.SubwindowGenerator(self.wr,
										self.ref_mask,
										self.masks,
										img_path,
										self.shift_step).generate_subwindows(self.ng,self.fn)

		cnt = 1
		dr.subwindows( len(subwindows) )
		for sw in subwindows:
			if self.classifier.is_face(sw):		
				dr.label_face(sw.crop_box())
			else:
				dr.label_not_face(sw.crop_box())
		
		return dr

	def is_face(self,img_path,sw):
		self.classifier.set_image(img_path)
		return self.classifier.is_face(sw)


class DetectorReport():

	def __init__(self,labeled_img,report_name=""):
		self.__labeled_img = labeled_img
		self.__report_name = report_name

		self.__false_negative=0
		self.__false_positive=0		

		self.__num_faces=0
		self.__num_sw=0

		self.__faces_boxes = []


	def label_face(self,box):
		self.__faces_boxes.append(box)		
		self.__num_faces+=1

		if not self.__labeled_img.check_box(box):
			self.__false_positive+=1

	def label_not_face(self,box):
		if self.__labeled_img.check_box(box):
			self.__false_negative+=1


	def subwindows(self,num_sw):
		self.__num_sw = num_sw


	def show_image(self):
		img_display = Image.open(self.__labeled_img.image_filepath()).convert("RGB")
		draw = ImageDraw.Draw(img_display) 

		for b in self.__faces_boxes:
			draw.rectangle( b,outline=(0,255,0) )
		
		for b in self.__labeled_img.faces_boxes():
			draw.rectangle( b,outline=(0,0,255) )

		img_display.show()	


	def __repr__(self):
		return "\n%s\n\nSUBWINDOWS: %d\nFACES: %d\nFALSE POSITIVES: %d\nFALSE NEGATIVES: %d" % (self.__report_name,
																								self.__num_sw,
																							    self.__num_faces,
																							    self.__false_positive,
																							    self.__false_negative)
