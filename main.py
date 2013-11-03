#coding:utf-8
#!/usr/bin/python

import img_lib as ilib
import misc
import math_lib as mlib

def my_database():
	Y = misc.create_image_database("img/my_database",6)		#Test Matrix of Image Column Vectors
	X =  misc.create_image_database("img/my_database",4)	#Database Matrix of Image Column Vectors

	CT = misc.build_transpose_covariance_matrix(X)

	S,U = mlib.find_all_heigen(CT)
	U = X.dot(U)	#Eigenbasis

	Px = U.T.dot(X)	#Projection of X on the EigenBasis
	Py = U.T.dot(Y)

	# print U.shape
	# print X.shape
	# print Px.shape
	# print Py.shape

	match_images("my_database",2,Px,Py)


def att_database():
	tl = [3,7]	#Image Indexes used as Test (not included in the database)
	ni = 10		#Number of Images per person
	X,Y = misc.create_image_database_2("img/att_faces/",40,ni,tl)

	CT = misc.build_transpose_covariance_matrix(X)

	S,U = mlib.find_all_heigen(CT)
	U = X.dot(U)	#Eigenbasis

	Px = U.T.dot(X)	#Projection of X on the EigenBasis
	Py = U.T.dot(Y)

	# print X.shape
	# print Y.shape

	match_images("att_database",ni-len(tl),Px,Py)

def match_images(db_name,np,Px,Py):
	'''
	db_name: Name of the image Database
	np: Number of images per unique person
	Px: Database Images Projected on to the eigenspace
	Py: Test Images Projected on to the eigenspace
	'''

	print "Match Images Test for database: %s" % (db_name,)
	for cy in range(0,Py.shape[1]):
		y = Py[:,cy]
		min_d = None
		min_im = None
		im = 0
		for cx in range(0,Px.shape[1]):
			x = Px[:,cx]
			d = mlib.get_distance(x,y)
			if min_d is None:	
				min_d = d
				min_im = im
			elif d<min_d:
				min_d = d
				min_im = im

			im+=1

		print "Image %d : Closest one: Person %d" % (cy+1,min_im/np+1)


def main():
	att_database()

main()
