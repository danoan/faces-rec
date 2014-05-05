#coding:utf-8

import numpy as np
import Image,math

'''
	To avoid misunderstanding, points are stored as the same manner as matrix elements. Then, a point
	is always in the form (y,x)
'''	
X = 1
Y = 0

WIDTH=1
HEIGHT=0

A=0
B=1
C=2
D=3

class IntegralImage():

	def __init__(self,img_path):		
		self.__compute_integral_image(img_path)

	def __compute_integral_image(self,img_path):
		'''
			Integral Image is a special data structure to increase the speed of filter computation.
			ii[height][width] = Sum of all the pixels above and on the left of (height,width).
		'''		
		line_index = 0
		col_index = 0
		line_sum = 0

		with open(img_path,"rb") as img_file:
			img = Image.open(img_file)

			self.size = (img.size[1],img.size[0])
			self.__integral_image = np.zeros(self.size)

			for p in img.getdata():
				if col_index==self.size[1]:
					line_index+=1
					line_sum=0
					col_index=0

				self.__integral_image[line_index][col_index] = self.__integral_image[line_index-1][col_index] + line_sum + p
				line_sum+= p
				col_index+=1

		# print self.__integral_image

	def __outside_limits(self,p):
		'''
			In order to correctly compute the filter blocks, the block points may lie inside the image limits. Then,
			this function tells you if a point is outside of the image limits.
		'''
		return p[HEIGHT] >= self.size[HEIGHT] or p[HEIGHT] < 0 or p[WIDTH] >= self.size[WIDTH] or p[WIDTH] < 0


	def __compute_block(self,block,location):
		'''
			When the block has just one column, I just need the Point C
			When the block has just one line, I just need the Point B
		'''
		block_sum=0
		for bl in block:
			point_A = ( location[Y] + bl[A][Y], location[X] + bl[A][X] )
			point_B = ( location[Y] + bl[B][Y], location[X] + bl[B][X] )
			point_C = ( location[Y] + bl[C][Y], location[X] + bl[C][X] )
			point_D = ( location[Y] + bl[D][Y], location[X] + bl[D][X] )

			# print location,bl
			# print point_A,point_B,point_C,point_D

			II = self.get_from_ii(point_A,point_B,point_C,point_D)

			block_sum += (II[0]+II[3]-II[2]-II[1])	

		return block_sum	

	def get_from_ii(self,pa,pb,pc,pd):
		'''
			The block points FeatureMaster receive are:
			Point_A = top-left-pixel
			Point_B = top-right-pixel
			Point_C = bottom-left-pixel
			Point_D = bottom-right-pixel

			In order to sum all the pixels inside this block (the points inclusively) by integral image, there is 
			a need to pick points in the border of the block. The only point that remains the same is the point D.

			Remember that: filter(block) = ii(pa)+ii(pd)-ii(pb)-ii(pc)
		'''
		pa = (pa[HEIGHT]-1,pa[WIDTH]-1)
		pb = (pb[HEIGHT]-1,pb[WIDTH])
		pc = (pc[HEIGHT],pc[WIDTH]-1)

		ii_list = []
		for p in [pa,pb,pc,pd]:
			if self.__outside_limits(p):
				ii = 0
			else:
				ii = self.__integral_image[p[HEIGHT]][p[WIDTH]]
			ii_list.append(ii)
		return ii_list		


	def filter(self,fm):
		if fm.size[0] > self.size[0] or fm.size[1] > self.size[1]:
			return 0

		white_sum = self.__compute_block(fm.mask.white,fm.location)
		black_sum = self.__compute_block(fm.mask.black,fm.location)

		return abs(black_sum-white_sum)			

def increment_list(factor,divisor,start,end):
	'''
		Given a start and end value, this function builds a list l of values such that
		l[0] = first multiple of divisor after start or start
		l[i] = first multiple of divisor after (l[i-1]*factor)

		Example: 
		>> increment_list(2,3,2,48)
		>> [3,6,9,15,30] /*pivot [2,4,8,16,32]

	'''
	multiplos = [i*divisor for i in range(0,end/divisor+1)]
	increment_list=[]
	pivot = start
	current_multiplo = int(math.ceil((start*1.0)/divisor))
	current_multiplo =  current_multiplo if current_multiplo > 0 else 1
	while current_multiplo < len(multiplos):
		increment_list.append(multiplos[ current_multiplo ])
		pivot = pivot*factor				
		if current_multiplo < int(math.ceil(pivot/divisor)):
			current_multiplo = int(math.ceil(pivot/divisor))
		else:
			current_multiplo+=1
	return increment_list