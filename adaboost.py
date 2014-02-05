#!/usr/bin/python
#coding:utf-8

import numpy as np
from PIL import Image
import os,random

di8 = np.dtype('int8')

class FMF():
	'''
		FeatureMaskFactory
		Linha x Coluna <==> Height x Width
	'''

	BLACK_PIXEL = 0
	WHITE_PIXEL = 1
	IGNORE_PIXEL = 2

	WIDTH=1
	HEIGHT=0

	def _resize(self):
		'''	
			Resize the current template mask. For each horizontal stretch, it tries all the possible combinations
			of vertical strecthes.
		'''
		if self._ardis == None:
			raise Exception("Ardis not setted")
		if type(self._ardis) != type((0,)):
			raise Exception("Ardis is not a tuple")
		if self._ardis[0] <2 or self._ardis[1]<2:
			raise Exception("Ardis's size is less than the minimal")
		

		left_w = self._ardis[FMF.WIDTH] - self._w + 1
		left_h = self._ardis[FMF.HEIGHT] - self._h + 1

		original_w = self._w
		original_h = self._h

		for i in range(0,left_w,self._resize_w_step):
			self._w = original_w + i
			for j in range(0,left_h,self._resize_h_step):
				self._h = original_h + j
				yield (self._create_mask(),(self._h,self._w))
			self._h = original_h

		self._w = original_w
		self._h = original_h			

	def _change_local(self):
		'''
			Change the current local position on a Ardis accordinlgy with the current size of the mask and
			the size of the Ardis.
		'''
		if self._ardis == None:
			raise Exception("Ardis not setted")
		if type(self._ardis) != type((0,)):
			raise Exception("Ardis is not a tuple")
		if self._ardis[0] <2 or self._ardis[1]<2:
			raise Exception("Ardis's size is less than the minimal")

		local = [0,0]

		left_w = self._ardis[FMF.WIDTH] - self._w - local[FMF.WIDTH] + 1
		left_h = self._ardis[FMF.HEIGHT] - self._h - local[FMF.HEIGHT] + 1

		for i in range(0,left_w):
			local[FMF.WIDTH]=i
			for j in range(0,left_h):
				local[FMF.HEIGHT]=j
				yield (local[FMF.WIDTH],local[FMF.HEIGHT])
			local[FMF.HEIGHT] = 0

	def next(self):
		'''
			Given a resize, it will tries all the possible local combinations for that resize
		'''
		count = 0
		for mask_and_size in self._resize():
			for location in self._change_local():
				yield ( FeatureMask( mask_and_size[0], mask_and_size[1], location), count)
				count+=1


class MaskTwoHorizontalFactory(FMF):
	'''
		0011
		0011
		0011

		It always has even width.
	'''

	def __init__(self,ardis):
		self._w = 2
		self._h = 2

		self._resize_w_step = 2
		self._resize_h_step = 1

		self._ardis = ardis


	def _create_mask(self):
		middle = self._w/2
		_mask = np.ones( (self._h,self._w),di8 )
		for l in range(0,self._h):
			for c in range(0,middle):
				_mask[l][c]=0

		return _mask


class MaskTwoVerticalFactory(FMF):
	'''
		0000
		0000
		1111
		1111

		It always has even height.
	'''

	def __init__(self,ardis):
		self._w = 2
		self._h = 2

		self._resize_w_step = 1
		self._resize_h_step = 2		

		self._ardis = ardis


	def _create_mask(self):
		middle = self._h/2
		_mask = np.ones( (self._h,self._w),di8 )
		for l in range(0,middle):
			_mask[l]=np.zeros( (1,self._w), di8 )

		return _mask		

class MaskThreeHorizontalFactory(FMF):

	def __init__(self,ardis):
		self._w = 3
		self._h = 2

		self._resize_w_step = 3
		self._resize_h_step = 1		

		self._ardis = ardis

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		third = self._w/3
		for c in range(third,2*third):
			_mask[0][c] = 0

		for l in range(1,self._h):
			_mask[l] = _mask[0].copy()

		return _mask


class MaskThreeVerticalFactory(FMF):

	def __init__(self,ardis):
		self._w = 2
		self._h = 3

		self._resize_w_step = 1
		self._resize_h_step = 3		

		self._ardis = ardis

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		third = self._h/3

		for l in range(third,2*third):
			_mask[l][0] = 0

		for c in range(1,self._w):
			_mask[:,c] = _mask[:,0].copy()

		return _mask	

class MaskDiagonalFactory(FMF):

	def __init__(self,ardis):
		self._w = 2
		self._h = 2

		self._ardis = ardis

		self._resize_w_step = 1
		self._resize_h_step = 1	

	def _resize(self):
		'''	
			Resize the current template mask. For each horizontal stretch, it tries all the possible combinations
			of vertical strecthes.
		'''
		if self._ardis == None:
			raise Exception("Ardis not setted")
		if type(self._ardis) != type((0,)):
			raise Exception("Ardis is not a tuple")
		if self._ardis[0] <2 or self._ardis[1]<2:
			raise Exception("Ardis's size is less than the minimal")
		

		left_w = self._ardis[FMF.WIDTH] - self._w + 1
		left_h = self._ardis[FMF.HEIGHT] - self._h + 1

		original_w = self._w
		original_h = self._h

		for i in range(0,min(left_w,left_h),self._resize_w_step):
			self._w = original_w + i
			self._h = original_h + i
			yield (self._create_mask(),(self._w,self._h))

		self._w = original_w
		self._h = original_h

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		for i in range(0,self._h):
			_mask[i][i] = 0

		return _mask

class FeatureMask():	

	def __init__(self,mask,size,location):
		self.mask = mask
		self.size = size
		self.location = location

	def __str__(self):
		return "MASK: \n %s \n\nLOCATION: %s" % (self.mask.__str__(),self.location.__str__())

	def __eq__(self,comp):
		if (self.mask==comp.mask).prod()==1:
			if self.size==comp.size:
				if self.location==comp.location:
					return True
		return False

'''
	Given a folder of images, it gets a crop of size face_size from a random
	image in the folder and returns as a ImageWindow. This should be the function
	used for retrieving of the negative face image instances on the training 
	step of the features.
'''
def get_next_random_image_window(folder,n):
	#folder: folder where the scene image files are located
	#n: number of random window to be generated

	file_names = os.listdir(folder)
	face_size = (64,64)

	shift_step = 4
	img_windows = []

	windows_total = 0
	for fn in file_names:		
		if fn=="crop":
			continue

		iw = ImageWindowGenerator(fn,folder,face_size,shift_step)
		img_windows.append( iw )

		windows_total+=iw.total_windows

	if n > windows_total:
		raise Exception("The number of windows asked for is greater than the available windows. Please increase the number os scene images or decrease the shift step value")


	for a in range(0,n):
		r = random.randint(0,len(img_windows)-1)
		iw = img_windows[r]

		yield iw.get_next_random_window().next()

		

'''
	Given an image, this class computes all the possible image windows
	with dimensions window_size could exist in the image
'''
class ImageWindowGenerator():

	SAVED_COUNT=-1

	@staticmethod
	def _get_next_count():
		ImageWindowGenerator.SAVED_COUNT+=1
		return ImageWindowGenerator.SAVED_COUNT

	def __init__(self,name,folder,window_size,shift_step=1):	
		self.name = name
		self.folder = folder
		img_file_name = "%s/%s" %(self.folder,self.name)
		self.img = Image.open(img_file_name)

		self.window_size = window_size
		
		left_w = self.img.size[0] - window_size[0]
		left_h = self.img.size[1] - window_size[1]

		self.windows = []
		self.total_windows = 0
		for x in range(0,left_w,shift_step):
			for y in range(0,left_h,shift_step):
				self.windows.append( (x,y) )
				self.total_windows+=1

	def get_next_random_window(self):
		windows_total = len(self.windows)
		index_to_sort = [i for i in range(0,windows_total) ]

		for n in range(0,windows_total):
			windows_left = windows_total-n
			r = random.randint(0,windows_left-1)
			i = index_to_sort[r]

			index_to_sort = index_to_sort[:r]+index_to_sort[r+1:]

			w = self.windows[i]
			crop_box = (w[0],w[1],w[0]+self.window_size[0],w[1]+self.window_size[1])
			crop_window = self.img.crop( crop_box )

			saved_name = "crop_%s.pgm" %(ImageWindowGenerator._get_next_count(),)

			yield ImageWindow(self.name,self.folder,crop_window,w,n,saved_name)

'''
	Container retrieved by the get_next_random function of the ImageWindowGenerator.
	It contains the path of the original image, a window crop image and its correspondent
	location related with the original image.
'''
class ImageWindow():
	def __init__(self,original_name,folder,window_img,location,seq_number,saved_name):		
		self.window_img = window_img
		self.original_name = original_name
		self.saved_name = saved_name
		self.folder = folder
		self.location = location
		self.seq_number = seq_number
		

	def save(self):
		path = "%s/crop" % (self.folder,)
		if not os.path.exists(path):
			os.mkdir(path)

		crop_filename = "%s/%s" % (path,self.saved_name,)
		iw.window_img.save(crop_filename)

	def __str__(self):
		return "Image: %s \t Saved: %s \t Location: %s \t Seq. Number: %d" % (self.original_name,self.saved_name,self.location,self.seq_number)

	

#GET_NEXT_RANDOM_IMAGE_WINDOW
# for iw in get_next_random_image_window("/home/daniel/Desktop/training non-face images",10):
# 	iw.save()
# 	print iw


#IMAGE WINDOW
# iw = ImageWindowGenerator("/home/daniel/Desktop/training non-face images/0.pgm",(64,64),64)
# for iw in iw.get_next_random_window():
# 	crop_filename = "/home/daniel/Desktop/training non-face images/crop_%d.pgm" % (iw.seq_number,)
# 	iw.window_img.save(crop_filename)
# 	print iw.location,iw.seq_number


#HORIZONTAL
# mthf = MaskTwoHorizontalFactory((4,4))
# for fm,count in mthf.next():
# 	print count
# 	print fm

#VERTICAL
# mthf = MaskTwoVerticalFactory((4,4))
# for fm,count in mthf.next():
# 	print count
# 	print fm

#TRIPLE_VERTICAL
# mthf = MaskThreeVerticalFactory((9,9))
# for fm,count in mthf.next():
# 	print count
# 	print fm

#TRIPLE_HORIZONTAL
# mthf = MaskThreeHorizontalFactory((2,3))
# for fm,count in mthf.next():
# 	print count
# 	print fm

#DIAGONAL
# mthf = MaskDiagonalFactory((2,3))
# for fm,count in mthf.next():
# 	print count
# 	print fm
