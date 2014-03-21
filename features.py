#coding:utf-8

import misc,copy
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

	WIDTH=1
	HEIGHT=0

	'''
		To avoid misunderstanding, points are stored as the same manner as matrix elements. Then, a point
		is always in the form (y,x)
	'''
	X = 1
	Y = 0

	A=0
	B=1
	C=2
	D=3

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
				yield (local[FMF.HEIGHT],local[FMF.WIDTH])
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

	MIN_MASK = (2,2)

	def __init__(self,ardis,shift_w=1,shift_h=1,resize_factor=1,w=2,h=2):
		#w must be even
		if w%2!=0:
			w+=1
		FMF.__init__(self,ardis,shift_w,shift_h,resize_factor,w,h,resize_w_step=2,resize_h_step=1)

	def _create_mask(self):
		middle = self._w/2

		white_a = (0,middle)
		white_b = (0,self._w-1)
		white_c = (self._h-1,middle)
		white_d = (self._h-1,self._w-1)

		black_a = (0,0)
		black_b = (0,middle-1)
		black_c = (self._h-1,0)
		black_d = (self._h-1,middle-1)

		white = MaskBlock( white_a, white_b, white_c, white_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask()
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

	def _create_mask(self):
		middle = self._h/2

		white_a = (middle,0)
		white_b = (middle,self._w-1)
		white_c = (self._h-1,0)
		white_d = (self._h-1,self._w-1)

		black_a = (0,0)
		black_b = (0,self._w-1)
		black_c = (middle-1,0)
		black_d = (middle-1,self._w-1)

		white = MaskBlock( white_a, white_b, white_c, white_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask()
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

	def _create_mask(self):
		third = self._w/3

		white_1_a = (0,0)
		white_1_b = (0,third-1)
		white_1_c = (self._h-1,0)
		white_1_d = (self._h-1,third-1)

		white_2_a = (0,2*third)
		white_2_b = (0,self._w-1)
		white_2_c = (self._h-1,2*third)
		white_2_d = (self._h-1,self._w-1)

		black_a = (0,third)
		black_b = (0,2*third-1)
		black_c = (self._h-1,third)
		black_d = (self._h-1,2*third-1)		

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask()
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

	def _create_mask(self):
		_mask = np.ones( (self._h,self._w), di8 )
		third = self._h/3

		for l in range(third,2*third):
			_mask[l][0] = 0

		for c in range(1,self._w):
			_mask[:,c] = _mask[:,0].copy()


		middle = self._h/3

		white_1_a = (0,0)
		white_1_b = (0,self._w-1)
		white_1_c = (third-1,0)
		white_1_d = (third-1,self._w-1)

		white_2_a = (2*third,0)
		white_2_b = (2*third,self._w-1)
		white_2_c = (self._h-1,0)
		white_2_d = (self._h-1,self._w-1)

		black_a = (third,0)
		black_b = (third,self._w-1)
		black_c = (2*third-1,0)
		black_d = (2*third-1,self._w-1)		

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black = MaskBlock( black_a, black_b, black_c, black_d )

		m = Mask()
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
		middle_h = self._h/2
		middle_w = self._w/2

		black_1_a = (0,0)
		black_1_b = (0,middle_w-1)			
		black_1_c = (middle_h-1,0)
		black_1_d = (middle_h-1,middle_w-1)

		black_2_a = (middle_h,middle_w)
		black_2_b = (middle_h,self._w-1)
		black_2_c = (self._h-1,middle_h)
		black_2_d = (self._h-1,self._w-1)

		white_1_a = (0,middle_w)
		white_1_b = (0,self._w-1)
		white_1_c = (middle_h-1,middle_w)
		white_1_d = (middle_h-1,self._w-1)

		white_2_a = (middle_h,0)
		white_2_b = (middle_h,middle_w-1)
		white_2_c = (self._h-1,0)
		white_2_d = (self._h-1,middle_w-1)

		white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d )
		white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d )
		black_1 = MaskBlock( black_1_a, black_1_b, black_1_c, black_1_d )		
		black_2 = MaskBlock( black_2_a, black_2_b, black_2_c, black_2_d )		

		m = Mask()
		m.add_white(white_1)
		m.add_white(white_2)
		m.add_black(black_1)
		m.add_black(black_2)

		return m

class MaskBlock():
	def __init__(self,a,b,c,d):
		self.points = [a,b,c,d]

		self.w = b[FMF.WIDTH] - a[FMF.WIDTH]
		self.h = c[FMF.HEIGHT] - a[FMF.HEIGHT]

	def resize(self,ce):
		'''
			Given the scale coefficient (ce), resize the mask
			a b
			c d
			Point a is kept fixed. 
			Point b goes to left (diminish) or right (enlarge)
			Point c goes up (diminish) or bottom (enlarge)
			Point d goes up and left (diminish) or bottom and right (enlarge)
		'''

		new_w = int( round(self.w*ce) )
		new_h = int( round(self.h*ce) )
		if new_w==0: new_w=1.0
		if new_h==0: new_h=1.0

		diff_w = new_w-self.w
		diff_h = new_h-self.h
		self.points[FMF.B] = ( self.points[FMF.B][FMF.HEIGHT], self.points[FMF.B][FMF.WIDTH]+diff_w ) 
		self.points[FMF.C] = ( self.points[FMF.C][FMF.HEIGHT]+diff_h, self.points[FMF.C][FMF.WIDTH] )
		self.points[FMF.D] = ( self.points[FMF.D][FMF.HEIGHT]+diff_h, self.points[FMF.D][FMF.WIDTH]+diff_w )

		self.w = new_w
		self.h = new_h


	def __getitem__(self,i):
		return self.points[i]

	def __iter__(self):
		for p in self.points:
			yield p

class Mask():
	def __init__(self):
		self.white = []
		self.black = []	

	def add_white(self,block):
		self.white.append(block)

	def add_black(self,block):
		self.black.append(block)		

	def __getitem__(self,i):
		return self.white if i==0 else self.black

	def __iter__(self):
		yield self.white
		yield self.black

class FeatureMask():	

	def __init__(self,mask,size,location,__hard_coded_mask=None):
		
		self.__original_mask = mask
		self.__original_location = location
		self.__original_size = size

		self.size = size
		self.mask = mask
		self.location = location
		self.__hard_coded_mask = __hard_coded_mask #Used for Test

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
		self.location = (	subwindow.y,
							subwindow.x)
		self.size = subwindow.size
		self.mask = copy.deepcopy(self.__original_mask)

		for block_type in self.mask:
			for block in block_type:
				block.resize(subwindow.ce)

		#print "ORIGINAL",self.__original_mask.white[0].w
		print "NEW LOCATION ", self.location, self.size,subwindow.x

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
			if self.size==comp.size:
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
		