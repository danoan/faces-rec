#coding:utf-8

import numpy as np
import Image
import imglib
import math

'''
Given a folder, reads all the JPG files as Images, converts each of
them in row vectors and returns a list with all the row vectors
'''
def create_image_database(folder,n):
	k = 1
	X = None
	while k<=n:
		img = Image.open("%s/img_%d.jpg" % (folder,k))
		if X is None:
			X = np.array( [imglib.img_to_rvector(img)] )
		else:
			X = np.append(X,[imglib.img_to_rvector(img)],axis=0)
		k+=1

	return X.T




'''
Read all the images following the original file hierarchy 
from the AT&T Database
	att_faces
		s1
			1.pgm
			2.pgm
			...
		s2
			1.pgm
			2.pgm
			...
p: number of subjects
n: number of images per subject
tl: list of image indexes. These ones will not be use as the database, 
	but as the test database
'''
def create_image_database_2(folder,p,n,tl):
	X = None
	Y = None
	for pi in range(1,p+1):
		k = 1
		while k<=n:
			img = Image.open("%s/s%d/%d.pgm" % (folder,pi,k))

			if k in tl:
				#Used as test Image
				if Y is None:
					Y = np.array( [imglib.img_to_rvector(img)] )
				else:
					Y = np.append(Y,[imglib.img_to_rvector(img)],axis=0)
			else:
				#Used as Database Image
				if X is None:
					X = np.array( [imglib.img_to_rvector(img)] )
				else:
					X = np.append(X,[imglib.img_to_rvector(img)],axis=0)
			k+=1

	return X.T,Y.T

'''
	n: Number of image per person
	p_db: List of people indexes presented at the database
	p_uk: List of people indexes will not be at the database
	tl: List of image indexes (for all database people) that it will be used
		as test images

	Return
	X: Database image column vectors
	rtcl: Recognition Test Cases List
'''
def create_image_database_3(folder,n,p_db,p_uk,tl):
	X = None
	Y = None
	metadb = []
	rtcl = []

	#Database People
	for p in p_db:
		k=1
		while k<=n:
			f_str = "%s/s%d/%d.pgm" % (folder,p,k)
			img = Image.open(f_str)			
			if k in tl:
				#Used as test Image
				vc = np.array( [imglib.img_to_rvector(img)] )
				rtcl.append( create_rtc(ti=f_str,po=p,ii=k,uk=False) ) 

				if Y is None:
					Y = np.array( [imglib.img_to_rvector(img)] )
				else:
					Y = np.append(Y,[imglib.img_to_rvector(img)],axis=0)				
			else:				
				#Used as Database Image
				metadb.append( create_metadb(im=f_str,po=p,ii=k) )
				img_v = imglib.img_to_rvector(img)
				if X is None:
					X = np.array( [img_v] )
				else:
					X = np.append(X,[img_v],axis=0)

			k+=1		

	db_images = len(p_db)*( n-len(tl) )
	test_images = len(p_uk)*n + len(p_db)*( len(tl) )

	#Unknown People
	for p in p_uk:
		for k in range(1,11):
			f_str = "%s/s%d/%d.pgm" % (folder,p,k)
			img = Image.open(f_str)

			vc = np.array( [imglib.img_to_rvector(img)] )			
			rtcl.append( create_rtc(ti=f_str,po=p,ii=k,uk=True) ) 

			if Y is None:
				Y = np.array( [imglib.img_to_rvector(img)] )
			else:
				Y = np.append(Y,[imglib.img_to_rvector(img)],axis=0)							


	return X.T,Y.T,metadb,rtcl


def create_rtc(ti=None,mi=None,po=None,pi=None,ii=None,di=None,uk=None,vc=None):
	return {"test_image":ti,"match_image":mi,"person_owner":po,"person_identified":pi,"image_index":ii,"distance":di,"unknown":uk,"vector":vc}

def create_metadb(im=None,po=None,ii=None):
	return {"image_name":im,"person_owner":po,"image_index":ii}

	
