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

	def detect_faces(self,img_path):
		self.classifier.set_image(img_path)
		subwindows = windowgen.SubwindowGenerator(self.wr,
										self.ref_mask,
										self.masks,
										img_path,
										self.shift_step).generate_subwindows(self.ng,self.fn)

		color_img = Image.open(img_path).convert("RGB")
		draw = ImageDraw.Draw(color_img) 

		cnt = 1
		rec_faces = 0
		print "END GENERATE SUBWINDOWS :", len(subwindows)
		for sw in subwindows:
			if self.classifier.is_face(sw):		
				draw.rectangle( sw.crop_box(),outline=(15*sw.cur_ng,50+ 10*sw.cur_ng,20*sw.cur_ng) )
				rec_faces+=1
			else:
				# draw.rectangle( sw.crop_box(),outline="red" )
				pass

			if cnt%100==0:
				# print cnt
				pass
			cnt+=1

		color_img.show()	
		print "REC FACES: %d/%d" % (rec_faces,len(subwindows))

	def is_face(self,img_path,sw):
		self.classifier.set_image(img_path)
		return self.classifier.is_face(sw)

