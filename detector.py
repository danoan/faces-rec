#! usr/bin/python
#coding:utf-8

import Image,ImageDraw,math
from features import *


'''
	Given an image, it will generate all the subwindows in that image that
	satisfies the parameters

	ref_ardis = Window used to generate the masks (64,64)
	ref_mask =  Size of the littlest mask used in the hypohtesis taking as a
				reference the ref_ardis window
	masks 	 =  Feature classes used to create the Final Classifier
	img 	 =  Image whose subwindow will be generated
'''
class SubwindowGenerator():

	@staticmethod
	def fixed_factor(ng,tg,ce0,cef,wr):
		factor = 1.25
		if factor>cef:
			factor=cef
		return 1.0*math.pow(1.25,ng)

	@staticmethod
	def dynamic_factor(ng,tg,ce0,cef,wr):
		factor = (cef - ce0)/(tg*1.0)
		print ce0 + factor*ng
		return ce0 + factor*ng		

	FIXED_FACTOR = fixed_factor
	DYNAMIC_FACTOR = dynamic_factor

	def __init__(self,ref_ardis,ref_mask,masks,img,shift_step=1):		
		self.img = img
		self.wr = ref_ardis				
		self.shift_step = shift_step		

		min_mask = self.__discover_min_mask(masks)
		self.ce0 = self.__initial_scale_coef(ref_ardis,ref_mask,min_mask)
		self.ce_max = self.__max_scale_coef()
		

	def __discover_min_mask(self,masks):
		min_mask = [0,0]
		
		for m in masks:
			if m.MIN_MASK[0] > min_mask[0]:
				min_mask[0] = m.MIN_MASK[0]
			if m.MIN_MASK[1] > min_mask[1]:
				min_mask[1] = m.MIN_MASK[1]

		return (min_mask[0],min_mask[1])

	def __initial_scale_coef(self,ref_ardis,ref_mask,min_mask):
		mask_coef = math.floor( 	max( ref_mask[0]/min_mask[0],
				  				    		  ref_mask[1]/min_mask[1] )
				  			  )
		return 1.0/mask_coef

	def __max_scale_coef(self):
		img_coef = math.floor(	min( self.img.size[0]/self.wr[0], 
							  			  self.img.size[1]/self.wr[1] )
						     )
		return img_coef

	def generate_subwindows(self,ng,fn):
		cur_ng = 0
		ab = 0
		subwindows = []
		while cur_ng<ng:
			ce = fn(cur_ng,ng,self.ce0,self.ce_max,self.wr)

			subwindow_size = ( int( round(self.wr[0]*ce) ),
							   int( round(self.wr[1]*ce) ) )


			if subwindow_size[0] > self.img.size[0] or subwindow_size[1] > self.img.size[1]:
				break
			
			max_y_pos = self.img.size[1] - subwindow_size[1]
			max_x_pos = self.img.size[0] - subwindow_size[1]

			# print max_x_pos,max_y_pos

			for y in range(0,max_y_pos,self.shift_step):
				for x in range(0,max_x_pos,self.shift_step):
					subwindows.append( Subwindow(x,y,subwindow_size,ce) )
					# print "COORD: ",x,y

			cur_ng+=1

		return subwindows

class Subwindow():

	def __init__(self,x,y,size,ce=1.0):
		self.x = x
		self.y = y
		self.size = size
		self.ce = ce

	def crop_box(self):
		return (self.x,self.y,self.x+self.size[0],self.y+self.size[1])

class Detector():

	def __init__(self,classifier,ng,ref_ardis,ref_mask,masks=[MaskTwoHorizontalFactory,
																MaskTwoVerticalFactory,
																MaskThreeHorizontalFactory,
																MaskThreeVerticalFactory,
																MaskDiagonalFactory],
				shift_step=1,fn=SubwindowGenerator.FIXED_FACTOR):

		self.classifier = classifier
		self.ng = ng
		self.wr = ref_ardis
		self.ref_mask = ref_mask

		self.masks = masks
		self.shift_step = shift_step		
		self.fn = fn

	def detect_faces(self,img):
		self.classifier.set_image(img)
		subwindows = SubwindowGenerator(self.wr,
										self.ref_mask,
										self.masks,
										img,
										self.shift_step).generate_subwindows(self.ng,self.fn)

		color_img = img.convert("RGB")
		draw = ImageDraw.Draw(color_img) 

		cnt = 1
		rec_faces = 0
		print "END GENERATE SUBWINDOWS :", len(subwindows)
		for sw in subwindows:
			if self.classifier.is_face(sw):		
				draw.rectangle( sw.crop_box(),outline="green" )
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

	def is_face(self,img,sw):
		self.classifier.set_image(img)
		return self.classifier.is_face(sw)

