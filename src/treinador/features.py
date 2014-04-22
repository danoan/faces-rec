#coding:utf-8

import misc,copy,math
import numpy as np

di8 = np.dtype('int8')

class FMF():
	'''
		FeatureMaskFactory
		Linha x Coluna <==> Height x Width
	'''

	BLACK_PIXEL = 0
	WHITE_PIXEL = 1
	IGNORE_PIXEL = 2

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
		

		left_w = self._ardis[misc.WIDTH] - self._w + 1
		left_h = self._ardis[misc.HEIGHT] - self._h + 1

		original_w = self._w
		original_h = self._h

		width_list = misc.increment_list(self._resize_factor,self._resize_w_step,self._w,self._ardis[misc.WIDTH])
		height_list = misc.increment_list(self._resize_factor,self._resize_h_step,self._h,self._ardis[misc.HEIGHT])

		for i in width_list:
			self._w = i
			for j in height_list:
				self._h = j
				size = (self._h, self._w)
				yield self._create_mask(size)
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

		left_w = self._ardis[misc.WIDTH] - self._w - local[misc.WIDTH] + 1
		left_h = self._ardis[misc.HEIGHT] - self._h - local[misc.HEIGHT] + 1

		for i in range(0,left_w,self._shift_w):
			local[misc.WIDTH]=i
			for j in range(0,left_h,self._shift_h):
				local[misc.HEIGHT]=j
				yield (local[misc.HEIGHT],local[misc.WIDTH])
			local[misc.HEIGHT] = 0

	def next(self):
		'''
			Given a resize, it will tries all the possible local combinations for that resize
		'''
		count = 0
		for mask in self._resize():
			for location in self._change_local():
				yield ( FeatureMask( mask, location), count)
				count+=1


class MaskTwoHorizontalFactory(FMF):
	'''
		0011
		0011
		0011

		It always has even width.
	'''

	MIN_MASK = (2,2)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		#w must be even
		if w%2!=0:
			w+=1
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=2,resize_h_step=1)

	def _create_mask(self,size):
		h = size[misc.HEIGHT]
		w = size[misc.WIDTH]

		middle = w/2

		white_a = (0,middle)
		white_b = (0,w-1)
		white_c = (h-1,middle)
		white_d = (h-1,w-1)

		black_a = (0,0)
		black_b = (0,middle-1)
		black_c = (h-1,0)
		black_d = (h-1,middle-1)

		white = MaskBlock( white_a, white_b, white_c, white_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask(size,self)
		m.add_white(white)
		m.add_black(black)

		return m	

class MaskTwoVerticalFactory(FMF):
	'''
		0000
		0000
		1111
		1111

		It always has even height.
	'''

	MIN_MASK = (2,2)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		#h must be even
		if h%2!=0:
			h+=1
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=1,resize_h_step=2)

	def _create_mask(self,size):
		h = size[misc.HEIGHT]
		w = size[misc.WIDTH]

		middle = h/2

		white_a = (middle,0)
		white_b = (middle,w-1)
		white_c = (h-1,0)
		white_d = (h-1,w-1)

		black_a = (0,0)
		black_b = (0,w-1)
		black_c = (middle-1,0)
		black_d = (middle-1,w-1)

		white = MaskBlock( white_a, white_b, white_c, white_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask(size,self)
		m.add_white(white)
		m.add_black(black)

		return m			

class MaskThreeHorizontalFactory(FMF):
	'''
		110011
		110011
		110011
	'''

	MIN_MASK = (3,2)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=3,h=2):
		#w must be divisible by 3
		if w%3!=0:
			w+= 3-w%3
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=3,resize_h_step=1)

	def _create_mask(self,size):
		h = size[misc.HEIGHT]
		w = size[misc.WIDTH]

		third = w/3

		'''
			In case of h is not a multiple of 3. 
			diff,extra_a: (2,1); (1,0)
			This variables decided which strategy to adopt when rescaling.
			This behavior should be tested. TODO
		'''
		diff = w%3		
		extra_a = max(diff-1,0)		

		white_1_a = (0,0)
		white_1_b = (0,third-1+extra_a)
		white_1_c = (h-1,0)
		white_1_d = (h-1,third-1+extra_a)

		white_2_a = (0,2*third)
		white_2_b = (0,w-1)
		white_2_c = (h-1,2*third)
		white_2_d = (h-1,w-1)

		black_a = (0,third+extra_a)
		black_b = (0,2*third-1+diff)
		black_c = (h-1,third+extra_a)
		black_d = (h-1,2*third-1+diff)		

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask(size,self)
		m.add_white(white_1)
		m.add_white(white_2)
		m.add_black(black)

		return m		


class MaskThreeVerticalFactory(FMF):
	'''
		1111
		1111
		0000
		0000
		1111
		1111
	'''

	MIN_MASK = (2,3)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=3):
		#h must be divisible by 3
		if h%3!=0:
			h+= 3-w%3		
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=1,resize_h_step=3)	

	def _create_mask(self,size):
		h = size[misc.HEIGHT]
		w = size[misc.WIDTH]
		third = h/3
		
		diff = h%3		
		extra_a = max(diff-1,0)

		middle = h/3

		white_1_a = (0,0)
		white_1_b = (0,w-1)
		white_1_c = (third-1+extra_a,0)
		white_1_d = (third-1+extra_a,w-1)

		white_2_a = (2*third,0)
		white_2_b = (2*third,w-1)
		white_2_c = (h-1,0)
		white_2_d = (h-1,w-1)

		black_a = (third+extra_a,0)
		black_b = (third+extra_a,w-1)
		black_c = (2*third-1+diff,0)
		black_d = (2*third-1+diff,w-1)		

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask(size,self)
		m.add_white(white_1)
		m.add_white(white_2)
		m.add_black(black)

		return m			
	

class MaskDiagonalFactory(FMF):
	'''
		0011
		1100


		00001111
		00001111
		11110000
		11110000
	'''

	MIN_MASK = (2,2)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		#w and h must be even
		if w%2!=0:
			w+=1
		if h%2!=0:
			h+=1
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
		

		left_w = self._ardis[misc.WIDTH] - self._w + 1
		left_h = self._ardis[misc.HEIGHT] - self._h + 1

		original_w = self._w
		original_h = self._h

		width_list = misc.increment_list(self._resize_factor,self._resize_w_step,self._w,self._ardis[misc.WIDTH])

		for i in width_list:
			size = (i,i)
			yield self._create_mask(size)

		self._w = original_w
		self._h = original_h

	def _create_mask(self,size):
		h = size[misc.HEIGHT]
		w = size[misc.WIDTH]

		middle_h = h/2
		middle_w = w/2

		black_1_a = (0,0)
		black_1_b = (0,middle_w-1)			
		black_1_c = (middle_h-1,0)
		black_1_d = (middle_h-1,middle_w-1)

		black_2_a = (middle_h,middle_w)
		black_2_b = (middle_h,w-1)
		black_2_c = (h-1,middle_h)
		black_2_d = (h-1,w-1)

		white_1_a = (0,middle_w)
		white_1_b = (0,w-1)
		white_1_c = (middle_h-1,middle_w)
		white_1_d = (middle_h-1,w-1)

		white_2_a = (middle_h,0)
		white_2_b = (middle_h,middle_w-1)
		white_2_c = (h-1,0)
		white_2_d = (h-1,middle_w-1)

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black_1 = MaskBlock( black_1_a, black_1_b, black_1_c, black_1_d )		
		black_2 = MaskBlock( black_2_a, black_2_b, black_2_c, black_2_d )		

		m = Mask(size,self)
		m.add_white(white_1)
		m.add_white(white_2)
		m.add_black(black_1)
		m.add_black(black_2)

		return m

class MaskBlock():
	def __init__(self,a,b,c,d):
		self.points = [a,b,c,d]

		self.w = b[misc.WIDTH] - a[misc.WIDTH] + 1
		self.h = c[misc.HEIGHT] - a[misc.HEIGHT] + 1

	def __getitem__(self,i):
		return self.points[i]

	def __iter__(self):
		for p in self.points:
			yield p

class Mask():
	def __init__(self,size,mf):
		self.white = []
		self.black = []	
		self.c_args = []
		self.size = size
		self.mf = mf 	#FactoryMask

	def add_white(self,block):
		self.white.append(block)
		self.c_args.extend( [block.points[0][0],block.points[0][1],
							  block.points[1][0],block.points[1][1],
							  block.points[2][0],block.points[2][1],
							  block.points[3][0],block.points[3][1],
							  block.w,block.h] )

	def add_black(self,block):
		self.black.append(block)		
		self.c_args.extend( [block.points[0][0],block.points[0][1],
							  block.points[1][0],block.points[1][1],
							  block.points[2][0],block.points[2][1],
							  block.points[3][0],block.points[3][1],
							  block.w,block.h] )

	def rescale(self,ce,original_size):
		new_h = int(round(original_size[misc.HEIGHT]*ce))
		new_w = int(round(original_size[misc.WIDTH]*ce))

		self.size = (new_h,new_w)
		return self.mf._create_mask(self.size)

	def __getitem__(self,i):
		return self.white if i==0 else self.black

	def __iter__(self):
		yield self.white
		yield self.black

class FeatureMask():	
	"""
		A FeatureMask is a rectangle composed by a list of white and black MaskBlock.
	"""

	def __init__(self,mask,location,__hard_coded_mask=None):		
		self.__original_mask = mask
		self.__original_location = location
		self.__original_size = mask.size

		self.mask = mask
		self.location = location
		self.__hard_coded_mask = __hard_coded_mask #Used for Test

	@property
	def size(self):
		return self.mask.size	

	def make_mask(self):
		if self.__hard_coded_mask!=None:
			return self.__hard_coded_mask

		built_mask = np.ones( self.size,di8 )
		for block_white in self.mask['white']:
			self.__make_block(built_mask,block_white,FMF.WHITE_PIXEL)

		for block_black in self.mask['black']:
			self.__make_block(built_mask,block_black,FMF.BLACK_PIXEL)

		return built_mask

	def adjust_mask(self,subwindow):
		'''	
			When FeatureMask is created, its locations is based on the ardis used on the
			mask creation time (usually, 64x64). When detecting faces on an image, the ardis
			(now called subwindows) would be located in different coordinates, that's why we
			have to adjust it.
		'''
		#Mask's Subwindow Rescale		
		mask_location = (	int( math.floor( self.__original_location[misc.HEIGHT]*subwindow.ce ) ),
							int( math.floor( self.__original_location[misc.WIDTH]*subwindow.ce ) )
						)

		#Subwindow Location
		self.location = (	mask_location[0]+subwindow.y,
							mask_location[1]+subwindow.x)
		

		#Mask Rescale
		self.mask = self.mask.rescale(subwindow.ce,self.__original_size)
		#print "ORIGINAL",self.__original_mask.white[0].w
		# print "NEW LOCATION ", self.location, self.size,subwindow.x

	def __make_block(self,built_mask,block,color):
		block_w_start = block[0][1]
		block_w_end = block[1][1]

		block_h_start = block[0][0] 
		block_h_end = block[2][0]

		for l in range(block_h_start,block_h_end+1):
			for c in range(block_w_start,block_w_end+1):
				built_mask[l][c] = color

	def __str__(self):
		return "MASK: \n %s \n\nLOCATION: %s" % (self.make_mask().__str__(),self.location.__str__())

	def __eq__(self,comp):
		if (self.make_mask()==comp.make_mask()).prod()==1:
			if self.mask.size==comp.mask.size:
				if self.location==comp.location:
					return True
		return False

#HORIZONTAL
# mthf = MaskTwoHorizontalFactory((2,2))
# for fm,count in mthf.next():
# 	print count
# 	print fm
# 	print fm.make_mask()	

#VERTICAL
# mthf = MaskTwoVerticalFactory((4,4))
# for fm,count in mthf.next():
# 	print count
# 	print fm
# 	print fm.make_mask()

#TRIPLE_VERTICAL
# mthf = MaskThreeVerticalFactory((4,4))
# for fm,count in mthf.next():
# 	print count
# 	print fm
# 	print fm.make_mask()

#TRIPLE_HORIZONTAL
# mthf = MaskThreeHorizontalFactory((4,6))
# for fm,count in mthf.next():
# 	print count
# 	print fm
# 	print fm.make_mask()

#DIAGONAL
# mthf = MaskDiagonalFactory((6,6))
# for fm,count in mthf.next():
# 	print count
# 	print fm
# 	print fm.make_mask()
		