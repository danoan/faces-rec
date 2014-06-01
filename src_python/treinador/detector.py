#! usr/bin/python
#coding:utf-8

import Image,ImageDraw,math,time

import windowgen
from features import *

class Detector():

	def __init__(self,classifier,ng,ref_ardis,ref_mask,masks=[MaskTwoHorizontalFactory,
																MaskTwoVerticalFactory,
																MaskThreeHorizontalFactory,
																MaskThreeVerticalFactory,
																MaskDiagonalFactory],
				shift_step=1,fn=windowgen.SubwindowGenerator.FIXED_FACTOR,ac=0.5):

		self.classifier = classifier
		self.ng = ng
		self.wr = ref_ardis
		self.ref_mask = ref_mask

		self.masks = masks
		self.shift_step = shift_step		
		self.ac = ac
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
			if self.classifier.is_face(sw,ac=self.ac):		
				dr.label_face(sw.crop_box())
			else:
				dr.label_not_face(sw.crop_box())
		
		return dr

	def is_face(self,img_path,sw):
		self.classifier.set_image(img_path)
		return self.classifier.is_face(sw,ac=self.ac)


class DetectorReport():

	def __init__(self,labeled_img,ac=None,shift_step=None,report_name=""):
		self.report_name = report_name

		self.false_negative=0
		self.false_positive=0		

		self.num_faces=0
		self.num_sw=0

		self.ac = ac
		self.shift_step = shift_step

		self.faces_boxes = []

		self.labeled_img = labeled_img

	def sum(self,dr):
		self.false_negative+=dr.false_negative
		self.false_positive+=dr.false_positive

		self.num_faces+=dr.num_faces
		self.num_sw+=dr.num_sw


	def label_face(self,box):
		self.faces_boxes.append(box)		
		self.num_faces+=1

		if not self.labeled_img.check_box(box):
			self.false_positive+=1

	def label_not_face(self,box):
		if self.labeled_img.check_box(box):
			self.false_negative+=1


	def show_image(self):
		img_display = Image.open(self.labeled_img.image_filepath()).convert("RGB")
		draw = ImageDraw.Draw(img_display) 
		for b in self.faces_boxes:
			draw.rectangle( b,outline=(0,255,0) )
		
		for b in self.labeled_img.faces_boxes():
			draw.rectangle( b,outline=(0,0,255) )

		img_display.show()	


	def subwindows(self,num_sw):
		self.num_sw = num_sw

	def data(self):
		return {"false_positive":self.false_positive,
				"false_negative":self.false_negative,
				"faces":self.num_faces,
				"shift_step":self.shift_step,
				"ac":self.ac}		

	def __repr__(self):
		return "\n%s\n\nSUBWINDOWS: %d\nFACES: %d\nFALSE POSITIVES: %d\nFALSE NEGATIVES: %d" % (self.report_name,
																								self.num_sw,
																							    self.num_faces,
																							    self.false_positive,
																							    self.false_negative)		

