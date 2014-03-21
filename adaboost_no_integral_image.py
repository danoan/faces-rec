#!/usr/bin/python
#coding:utf-8

import numpy as np
from PIL import Image
import os,random,misc

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

	def __init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step,resize_h_step):
		self._ardis = ardis
		self._shift_w = shift_w
		self._shift_h = shift_h
		self._resize_factor = resize_factor
		self._w = w
		self._h = h
		self._resize_w_step = resize_w_step
		self._resize_h_step = resize_h_step


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

		width_list = misc.increment_list(self._resize_factor,self._resize_w_step,self._w,self._ardis[FMF.WIDTH])
		height_list = misc.increment_list(self._resize_factor,self._resize_h_step,self._h,self._ardis[FMF.HEIGHT])

		for i in width_list:
			self._w = i
			for j in height_list:
				self._h = j
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

		for i in range(0,left_w,self._shift_w):
			local[FMF.WIDTH]=i
			for j in range(0,left_h,self._shift_h):
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

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=2,resize_h_step=1)


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

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=1,resize_h_step=2)

	def _create_mask(self):
		middle = self._h/2
		_mask = np.ones( (self._h,self._w),di8 )
		for l in range(0,middle):
			_mask[l]=np.zeros( (1,self._w), di8 )

		return _mask		

class MaskThreeHorizontalFactory(FMF):
	'''
		110011
		110011
		110011
	'''

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=3,h=2):
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=3,resize_h_step=1)

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		third = self._w/3
		for c in range(third,2*third):
			_mask[0][c] = 0

		for l in range(1,self._h):
			_mask[l] = _mask[0].copy()

		return _mask


class MaskThreeVerticalFactory(FMF):
	'''
		1111
		1111
		0000
		0000
		1111
		1111
	'''

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=3):
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=1,resize_h_step=3)	

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		third = self._h/3

		for l in range(third,2*third):
			_mask[l][0] = 0

		for c in range(1,self._w):
			_mask[:,c] = _mask[:,0].copy()

		return _mask	

class MaskDiagonalFactory(FMF):
	'''
		0011
		1100
	'''

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=1,resize_h_step=1)

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

		width_list = misc.increment_list(self._resize_factor,self._resize_w_step,self._w,self._ardis[FMF.WIDTH])

		for i in width_list:
			self._w = i
			self._h = i
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

	
class TrainingImage():
	'''
		Class Container that holds a training image file name and its label: True for face and False for non-face
	'''
	def __init__(self,img_path,label):
		self.img_path = img_path
		self.img_file_name = img_path.split(os.path.sep)[-1]
		self.label = label

class FeatureMaster():
	'''
		Class who will run the filter operations related a TrainingImage and all the possible existing FeatureMask
		accordingly with the ardis and shifts.
	'''
	def __init__(self,ardis,shift_w,shift_h,resize_factor,img,img_label):
		self.ardis = ardis
		self.shift_w=shift_w
		self.shift_h=shift_h
		self.resize_factor=resize_factor
		self.start_w = 6
		self.start_h = 6

		self.img=img
		self.img_label=img_label

		self.m2h_values = []
		self.m2v_values = []
		self.m3h_values = []
		self.m3v_values = []
		self.md_values = []

	def compute_features(self):

		m2hf = MaskTwoHorizontalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		total_m2hf=0
		for fm,count in m2hf.next():
			self.m2h_values.append( self.filter(fm,self.img) )
			total_m2hf+=count
		print "End MaskTwoHorizontal (%d) " % (total_m2hf,)

		m2vf = MaskTwoVerticalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		total_m2vf=0
		for fm,count in m2vf.next():
			self.m2v_values.append( self.filter(fm,self.img) )			
			total_m2vf+=count
		print "End MaskTwoVertical (%d) " % (total_m2vf,)

		m3hf = MaskThreeHorizontalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		total_m3hf=0
		for fm,count in m3hf.next():
			self.m3h_values.append( self.filter(fm,self.img) )			
			total_m3hf+=count
		print "End MaskThreeHorizontal (%d) " % (total_m3hf,)

		m3vf = MaskThreeVerticalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		total_m3vf=0
		for fm,count in m3vf.next():
			self.m3v_values.append( self.filter(fm,self.img) )									
			total_m3vf+=count
		print "End MaskThreeVertical (%d) " % (total_m3vf,)

		mdf = MaskDiagonalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		total_mdf=0
		for fm,count in mdf.next():
			self.md_values.append( self.filter(fm,self.img) )	
			total_mdf+=count					
		print "End MaskDiagonal (%d) " % (total_mdf,)	

	def filter(self,fm,im):
			box = (	fm.location[FMF.WIDTH],
					fm.location[FMF.HEIGHT],
					fm.location[FMF.WIDTH]+fm.size[FMF.WIDTH],
					fm.location[FMF.HEIGHT]+fm.size[FMF.HEIGHT])

			crop_img = im.crop(box)

			lines = fm.size[FMF.HEIGHT]
			cols = fm.size[FMF.WIDTH]

			black_sum = 0
			white_sum = 0
			for l in range(0,lines):
				for c in range(0,cols):
					img_index = l*cols+c
					if fm.mask[l][c]==FMF.BLACK_PIXEL:
						black_sum+=crop_img.getdata()[img_index]
					else:
						white_sum+=crop_img.getdata()[img_index]

			return abs(black_sum-white_sum)

class FeatureResultSet():
	'''
		This is the resulting class of a FeatureMask application over a TrainingImage. It holds the related
		TrainingImage, the FeatureMask used and the filter value. This is the class which the adaBoost technique
		will be interfacing on.
	'''
	def __init__(self):
		self.face = []
		self.non_face = []


# im = Image.open("/home/daniel/Desktop/lfwcrop_grey/faces/Abdullah_Gul_0004.pgm")
# fm = FeatureMaster((64,64),4,4,1.25,im,True)
# fm.compute_features()
# print fm.m2h_values,fm.m2v_values,fm.m3h_values,fm.m3v_values,fm.md_values


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
# mthf = MaskTwoHorizontalFactory((2,2))
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
mthf = MaskThreeHorizontalFactory((2,3))
for fm,count in mthf.next():
	print count
	print fm

#DIAGONAL
# mthf = MaskDiagonalFactory((2,3))
# for fm,count in mthf.next():
# 	print count
# 	print fm
