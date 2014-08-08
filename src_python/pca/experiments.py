#coding:utf-8
#!/usr/bin/python

import imglib,misc,mathlib,config

class Experiment():
	def setup(self):
		pass

	def run(self):
		pass

	def match_images(self,db_name,np,Px,Py,len_test_imgs=2):
		"""
			db_name: Name of the image Database
			np: Number of images per unique person
			Px: Database Images Projected on to the eigenspace
			Py: Test Images Projected on to the eigenspace
		"""

		rate_correct = 0
		step_correct = 1.0/Py.shape[1]
		print "Match Images Test for database: %s" % (db_name,)
		for cy in range(0,Py.shape[1]):
			y = Py[:,cy]
			min_d = None
			min_im = None
			im = 0
			for cx in range(0,Px.shape[1]):
				x = Px[:,cx]
				d = mathlib.get_distance(x,y)
				if min_d is None:	
					min_d = d
					min_im = im
				elif d<min_d:
					min_d = d
					min_im = im

				im+=1

			if( (cy/len_test_imgs+1)==min_im/np+1 ):
				rate_correct+=step_correct

			print len(Px),len(Py)
			print "Image %d (%d) : Closest one: Person %d" % (cy+1,cy/len_test_imgs+1,min_im/np+1)	

		print "Correct: %.2f \n Incorrect: %.2f" % (rate_correct,1-rate_correct)

	def match_rtcl(self,metadb,rtcl,Px):
		"""
			db_name: Name of the image Database
			np: Number of images per unique person
			Px: Database Images Projected on to the eigenspace
			Py: Test Images Projected on to the eigenspace
		"""

		for rtc in rtcl:
			y = rtc["vector"]
			min_d = None
			min_im = None
			im = 0
			for cx in range(0,Px.shape[1]):
				x = Px[:,cx]
				d = mathlib.get_distance(x,y)
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


class Experiment1(Experiment):
	"""
		my_database folder contains 6 images from 2 different people.
		The four first files are 2 different images of these 2 people, and are used as database.
		The other 2 files are 1 different image of each person, and are used as a test.

		In these experiment, both the images in the database and not in tha database are used in 
		the test.
	"""
	def setup(self):
		self.Y = misc.create_image_database("%s/my_database" % (config.DATASET_PATH,),6)		#Test Matrix of Image Column Vectors
		self.X =  misc.create_image_database("%s/my_database" % (config.DATASET_PATH,),4)		#Database Matrix of Image Column Vectors		
		return self

	def run(self):
		Px,Py = mathlib.run_PCA(self.X,self.Y)
		self.match_images("my_database",2,Px,Py)
		return self


class Experiment2(Experiment):
	"""
		All the images in the ATT database are included in the database (except the ones in the tl list, which
		are used in the test). At the end, all the test images are tested against the database.
		Example:
			The ATT database is composed by 40 different people, each one has 10 images.
			tl = [3,7]
			TestImages = [i1,i2,...i80]
			i0,i1 are the images with index 3 and 7 from the person 1
			i2,i3 are the images with index 3 and 7 from the person 2
			....

	"""
	def setup(self):
		self.tl = [3,7]		#Image Indexes from each person used as Test (not included in the database)
		self.ni = 10		#Number of Images per person 
		self.X,self.Y = misc.create_image_database_2( "%s/att_faces" % (config.DATASET_PATH,),40,self.ni,self.tl)
		return self

	def run(self):
		Px,Py = mathlib.run_PCA(self.X,self.Y)
		self.match_images("att_database",self.ni-len(self.tl),Px,Py)
		return self


class Experiment3(Experiment):
	def setup(self):
		p_index = [i for i in range(1,41)]	#All indices from 1 to 40	
		
		p_uk = [7,13,19,23,31]		#People indexes that will not be in database
		p_db = filter(lambda x: not(x in p_uk),p_index )	#Remove unknown indices from all indexes

		tl = [3,7]	#Image Indexes used as Test (not included in the database)	
		ni = 10		#Number of Images per person
		np = ni - len(tl)
		self.X,self.Y,self.metadb,self.rtcl = misc.create_image_database_3( "%s/att_faces" % (config.DATASET_PATH,),ni,p_db,p_uk,tl)

		return self

	def run(self):
		Px,Py = mathlib.run_PCA(self.X,self.Y) 	#When using X_m and Y_m the result is horrible, but with X and Y are nicer.

		cols = Py.shape[1]
		for i in range(0,cols):
			self.rtcl[i]["vector"] = Py[:,i]

		self.rtcl = self.match_rtcl(self.metadb,self.rtcl,Px)
		self.print_results(self.rtcl)
		# imglib.draw_results(self.rtcl)	
		return self

	def print_results(self,rtcl):
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

		TH_UNKNOWN = 28267208.01

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

		return self

class ExperimentFriends(Experiment):
	"""
		The database will be formed by 6 pictures of each Friend's character TV show. The database structure
		it is the same of the AT&T database.

	"""
	def setup(self):
		self.tl = [6]		#Image Indexes from each person used as Test (not included in the database)
		self.ni = 6		#Number of Images per person 
		self.X,self.Y = misc.create_image_database_2( "%s/friends_database/crop" % (config.DATASET_PATH,),6,self.ni,self.tl)
		return self

	def run(self):
		Px,Py = mathlib.run_PCA(self.X,self.Y)
		self.match_images("friends_database",self.ni-len(self.tl),Px,Py,1)
		return self	

if __name__=="__main__":
	#Executed as a main file
	Experiment2().setup().run()
else:
	#Used as an import from another python file
	pass
