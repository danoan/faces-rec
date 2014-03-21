# -*- coding: utf-8 -*-
#!/usr/bin/python
import numpy as np
import misc
import math_lib as mlib
import img_lib as ilib

'''
	Case Test
'''

def TEST_find_heigen():

	A1 = np.array( [ [3.4,-1.2],[0.8,0.6] ] ) #Resulting Matrix
	A2 = np.array( [ [1,1],[2,0] ] ) #Resulting Matrix
	A3 = np.array( [ [-4.9674694,1.0,3.0 ], [0.0, -7.9674694, 2.0], [3.0,4.0,-2.9674694] ] )

	V = np.array( [ [1,3], [2,1] ] ) #Eigenvectors
	S = np.array( [ [1,0], [0,3] ] ) #Eigenvalues	

	A = A3

	e1,v1 = mlib.find_heigen(A)
	print e1,v1

# TEST_find_heigen()

def TEST_find_all_heigen():
	A1 = np.array( [ [3.4,-1.2],[0.8,0.6] ] ) #Resulting Matrix
	A2 = np.array( [ [10.0,1.0,3.0],[0.0,7.0,2.0],[3.0,4.0,12.0] ] ) #Resulting Matrix
	A3 = np.array( [ [-4.9674694,1.0,3.0 ], [0.0, -7.9674694, 2.0], [3.0,4.0,-2.9674694] ] )
	A4 = np.array( [ [10,1,0],[0,7,3],[0,2,12] ] )
	A5 = np.array( [ [14,5,1,2],[0,24,2,0],[3,2,36,1],[1,12,0,10] ])
	
	A = A4
	pairs = mlib.find_all_heigen(A)

	for i in pairs:
		print i

TEST_find_all_heigen()

def TEST_create_database():
	# normalize_images("img",3)
	C = misc.create_image_database("img",3)
	print C.shape,C[0]

# TEST_create_database()

def TEST_memory():
	'''
		When I compute C, I have to compute the dot product between the image 
		vector columns. The dimension of C it will be p^2 x p^2. (e.g. 400 pixels means
		25.600.000.000, 25 billion de registros)

		100 -> 100.000.000 (100 milion)
	'''
	img_px = 125
	x_d = pow(img_px,2)

	x = np.ones( (1,x_d) )
	C = np.zeros( (x_d,x_d) )
	C = C + x.T.dot(x)
