#coding:utf-8
#!/usr/bin/python

import img_lib as ilib
import misc
import math_lib as mlib


def experiment_1():
	p_index = [i for i in range(1,41)]	#All indices from 1 to 40	
	
	p_uk = [7,13,19,23,31]		#People indexes that will not be in database
	p_db = filter(lambda x: not(x in p_uk),p_index )	#Remove unknown indices from all indexes

	tl = [3,7]	#Image Indexes used as Test (not included in the database)	
	ni = 10		#Number of Images per person
	np = ni - len(tl)
	X,Y,metadb,rtcl = misc.create_image_database_3("img/att_faces",ni,p_db,p_uk,tl)

	CT = misc.build_transpose_covariance_matrix(X)

	S,U = mlib.find_all_heigen(CT)
	U = X.dot(U)	#Eigenbasis

	Px = U.T.dot(X)	#Projection of X on the EigenBasis
	Py = U.T.dot(Y)	#Projection of Y on the EigenBasis

	# print X.shape
	print Py.shape

	cols = Py.shape[1]
	for i in range(0,cols):
		rtcl[i]["vector"] = Py[:,i]


	rtcl = match_rtcl(metadb,rtcl,Px)
	print_results(rtcl)
	ilib.draw_results(rtcl)	


def match_rtcl(metadb,rtcl,Px):
	'''
	db_name: Name of the image Database
	np: Number of images per unique person
	Px: Database Images Projected on to the eigenspace
	Py: Test Images Projected on to the eigenspace
	'''

	for rtc in rtcl:
		y = rtc["vector"]
		min_d = None
		min_im = None
		im = 0
		for cx in range(0,Px.shape[1]):
			x = Px[:,cx]
			d = mlib.get_distance(x,y)
			if min_d is None:	
				min_d = d
				min_im = cx
			elif d<min_d:
				min_d = d
				min_im = cx

		rtc["match_image"] = metadb[min_im]["image_name"]
		rtc["person_identified"] = metadb[min_im]["person_owner"]
		rtc["distance"] = min_d

	return rtcl

def print_results(rtcl):
	total = len(rtcl)
	total_db = 0
	total_uk = 0
	correct_db = 0
	correct_uk = 0
	
	correct_high_d = 0
	incorrect_low_d = 1e10
	unknown_low_d = 1e10

	correct_sum_d = 0
	incorrect_sum_d = 0
	unknown_sum_d = 0

	TH_UNKNOWN = 38178110.60

	print "Match Results \n"
	for r in rtcl:
		s= "Person %d identified as %d (%s) with distance %.2f \t" % (r["person_owner"],r["person_identified"],r["match_image"],r["distance"])

		if not r["unknown"]:
			total_db+=1		

			if r["distance"] > TH_UNKNOWN:
				print "%s %s" % (s,"unknown")
				continue

			if r["person_owner"]==r["person_identified"]:
				correct_db+=1

				correct_sum_d += r["distance"]
				if correct_high_d < r["distance"]:
					correct_high_d = r["distance"]

				print "%s %s" % (s,"correctly")
			else:
				incorrect_sum_d += r["distance"]
				if incorrect_low_d > r["distance"]:
					incorrect_low_d = r["distance"]

				print "%s %s" % (s,"incorrectly")
		else:
			if r["distance"] >= TH_UNKNOWN:
				print "%s %s" % (s,"unknown")
				correct_uk+=1
			else:
				print "%s %s" % (s,"incorrectly")

			unknown_sum_d += r["distance"]
			if unknown_low_d > r["distance"]:
				unknown_low_d = r["distance"]

	correct = correct_db+correct_uk
	total_uk = total - total_db

	print "**Database People**\n"
	print "# Test Images: %d \n%% Right %.2f \n" % (total_db,1.0*correct_db/total_db)

	print "**Unknown People**\n"
	print "# Test Images: %d \n%% Right %.2f \n" % (total_uk,1.0*correct_uk/total_uk)

	print "**General Results**"
	print "# Test Images: %d \n%% Right %.2f \n" % (total,1.0*correct/total)
	
	print "Correct Higher Distance: %.2f" % (correct_high_d,)
	print "Incorrect Lower Distance %.2f" % (incorrect_low_d,)
	print "Unknown Lower Distance %.2f" % (unknown_low_d,)

	print "Correct Average Distance: %.2f" % (correct_sum_d/correct_db,)
	print "Incorrect Average Distance %.2f" % (incorrect_sum_d/(total_db-correct_db),)
	print "Unknown Average Distance %.2f" % (unknown_sum_d/total_uk,)

experiment_1()

