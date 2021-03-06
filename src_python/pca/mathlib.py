# coding:utf-8
import numpy as np
from scipy import linalg

'''
	General Comments

		-Vectors are included as rows of the matrix

'''

df = np.dtype('f8') # 64-bit floating-point number

#Restricts the matrix such that after will represent a Basis for a space
def restrict_matrix(A,dn):
	print A,dn
	A = np.delete(A,dn,0)	#Remove Line dn
	A = np.delete(A.T,dn,0)	#Remove Column dn
	return A.T

#Find the Highest Eigenpair of Matrix A
def find_heigen(A):
	if A.shape[0]!=A.shape[1]:
		#A should be square
		return

	cols = A.shape[1]
	yk = np.ones( (1,cols),df)
	yk = yk.T
	
	MIN_DIFF = 1e-12
	MAX_IT = 500

	diff = 100
	it=0

	print "Matrix A",A,"\n"
	while(diff>MIN_DIFF and MAX_IT>it):
		xk = A.dot(yk)
		mk = 0

		for i in range(0,cols):
			if abs(xk[i,0]) > abs(mk):
				mk = xk[i,0]

		yk_1 = (1.0/mk)*xk
		diff = linalg.norm( yk-yk_1 )

		print "Iteration %d:\n" % (it,)
		print "XK:\n",xk,"\n"
		print "yk:\n",yk,"\n"

		yk = yk_1
		it+=1		

		print "yk_1:\n",yk_1,"\n"
		print "mk:\n",mk,"\n"

	return (yk,mk)


#Find all the eigenpairs of A ordered from the highest to the lowest
def find_all_heigen(A):

	return np.linalg.eig(A)

	if A.shape[0]!=A.shape[1]:
		#A should be square
		return

	last_evalue = 0
	cols = A.shape[1]
	I = np.identity(cols)

	pairs=[]
	while(len(pairs)<cols):	

		evector,evalue = find_heigen(A-last_evalue*I)

		print A-last_evalue*I	
		print last_evalue,evalue

		evalue=evalue + last_evalue
		last_evalue = evalue

		pairs.append( (evector,evalue) )

	return pairs

def get_distance(v1,v2):
	return np.linalg.norm(v1-v2)


def run_PCA(X,Y,n=0):
	"""
		X: Trainning DB
		Y: Test DB
		n: Number of eigenvector to use
	"""

	if n==0:
		n = X.shape[1]

	X = subtract_average_image(X)
	Y = subtract_average_image(Y)

	CT = build_transpose_covariance_matrix(X)

	S,W = find_all_heigen(CT)
	U = X.dot(W)	#Eigenbasis

	# for j in range(0,U.shape[1]):
	#     U[:,j] = U[:,j]/(pow(S[j],0.5))	

	Px = U[:,:n].T.dot(X)	#Projection of X on the EigenBasis
	Py = U[:,:n].T.dot(Y)

	return (Px,Py)	

def subtract_average_image(X):
	'''
		Compute the Average Image
	'''
	avg_xj = np.zeros((X.shape[0],1))		#A vector column (nx1)
	for j in range(0,X.shape[1]):
	    avg_xj = avg_xj + X[:,j].reshape(X.shape[0],1)	#Sum all vector columns of the matrix

	avg_xj = avg_xj*(1.0/X.shape[1])	#Average by the number of columns

	'''
		Subtract the Average Image from the matrix
	'''
	mat_avg_xj = np.zeros((X.shape[0],X.shape[1]))
	for j in range(0,X.shape[1]):	
	    mat_avg_xj[:,j] = avg_xj.reshape(X.shape[0])

	X = X - mat_avg_xj	

	return X

def build_transpose_covariance_matrix(X):
	'''
		X has n^4 x n, then C.T has dimensions n x n, and his eigenvector are computed 
		by X multiplied by the return matrix of the function find_all_heigen()
		AVG - Average Image
	'''
	n = X.shape[0]
	if n==0:
		return

	coef = 1.0/n
	C = coef*X.T.dot(X)

	return C		