	#coding:utf-8

import numpy as np
import math
import Image,random,os,pickle,time
from features import *

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

	def __init__(self,name,folder,folder_to_save,window_size,shift_step=1):	
		self.name = name
		self.folder = folder
		self.img_filename = "%s/%s" %(self.folder,self.name)
		self.folder_to_save = folder_to_save

		self.window_size = window_size
		
		''' I open and close the file every time some image processing needs to be done,
			otherwise, too many files would be opened.
		'''
		with open(self.img_filename,"rb") as img_file: 
			img = Image.open(img_file)

			left_w = img.size[0] - window_size[0]
			left_h = img.size[1] - window_size[1]

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

			with open(self.img_filename,"rb") as img_file:
				img = Image.open(img_file)
				crop_window = img.crop( crop_box )

			saved_name = "crop_%s.pgm" %(ImageWindowGenerator._get_next_count(),)

			yield ImageWindow(self.name,self.folder,self.folder_to_save,crop_window,w,n,saved_name)

'''
	Container retrieved by the get_next_random function of the ImageWindowGenerator.
	It contains the path of the original image, a window crop image and its correspondent
	location related with the original image.
'''
class ImageWindow():
	def __init__(self,original_name,folder,folder_to_save,window_img,location,seq_number,saved_name):		
		self.window_img = window_img
		self.original_name = original_name
		self.saved_name = saved_name
		self.folder = folder
		self.location = location
		self.seq_number = seq_number

		#Need it to keep state
		self.save(folder_to_save)
		

	def save(self,folder_to_save):
		path = "%s/crop/%s" % (self.folder,folder_to_save)
		if not os.path.exists(path):
			os.mkdir(path)

		crop_filename = "%s/%s" % (path,self.saved_name,)
		self.window_img.save(crop_filename)
		self.window_img.filename = crop_filename

	def __str__(self):
		return "Image: %s \t Saved: %s \t Location: %s \t Seq. Number: %d" % (self.original_name,self.saved_name,self.location,self.seq_number)

'''
	Given a folder of images, it gets a crop of size face_size from a random
	image in the folder and returns as a ImageWindow. This should be the function
	used for retrieving of the negative face image instances on the trainning 
	step of the features.
'''
def get_next_random_image_window(folder,n):
	#folder: folder where the scene image files are located
	#n: number of random window to be generated

	file_names = os.listdir(folder)
	folder_to_save = str( time.time() )
	face_size = (64,64)

	shift_step = 4
	img_windows = []

	windows_total = 0
	for fn in file_names:		
		if fn=="crop":
			continue

		iw = ImageWindowGenerator(fn,folder,folder_to_save,face_size,shift_step)
		img_windows.append( iw )

		windows_total+=iw.total_windows

	#windows_total = 223922
	if n > windows_total:
		raise Exception("The number of windows asked for is greater than the available windows. Please increase the number os scene images or decrease the shift step value")


	for a in range(0,n):
		r = random.randint(0,len(img_windows)-1)
		iw = img_windows[r]

		yield iw.get_next_random_window().next()

	
class TrainningSample():
	'''
		Class Container that holds a trainning image file name, its label: True for face and False for non-face
		and support information like weight, s_plus and s_minus used by the adaboost algorithm
	'''
	def __init__(self,ts_index,img_path,label,ardis):
		self.index = ts_index
		self.img_path = img_path
		self.img_filename = img_path.split(os.path.sep)[-1]		
		self.label = label

		with open(img_path,"rb") as img_file:
			img = Image.open(img_file)
			self.ii = IntegralImage(img)

		#Variables used for the minimization problem of finding the best feature.
		self.__weight = 0

	def weight(self,w=None):
		if w==None:
			return self.__weight
		else:
			self.__weight=w

	def filter(self,fm):
		return self.ii.filter(fm)

	def is_face(self):
		return self.label==FeatureMaster.FACE

class TrainningSampleFeature():
	'''
		Class returned by each generator of the FeatureMaster's function compute_features. A TrainningSample instance
		has as much TrainningSampleFeature instances as the number of existing features. This class holds specific
		information about a feature, like feature number, type, filter_value and the image associated with (an 
		instance of TrainningSample).

		s_plus and s_minus are characteristics of the feature, and they change from feature table to feature
		table because the ordering of the images are different between them.
	'''
	def __init__(self,ts_index,feature_type,feature_number,filter_value,fm):
		self.ts_index = ts_index	#TrainningSample Index on FeatureMaster.TS
		self.feature_type = feature_type
		self.feature_number = feature_number
		self.filter_value = filter_value
		self.fm = fm

	def __eq__(self,obj):
		return self.filter_value==obj.filter_value

	def __gt__(self,obj):
		return self.filter_value>obj.filter_value

	def __lt__(self,obj):
		return self.filter_value<obj.filter_value

	def image_name(self):
		return FeatureMaster.TS_SET[self.ts_index].img_filename

	def is_face(self):
		return FeatureMaster.TS_SET[self.ts_index].label==FeatureMaster.FACE

	def label(self):
		return FeatureMaster.TS_SET[self.ts_index].label

	def weight(self,w=None):
		if w==None:
			return FeatureMaster.TS_SET[self.ts_index].weight()
		else:
			FeatureMaster.TS_SET[self.ts_index].weight(w)

	def filter(self,fm):
		return FeatureMaster.TS_SET[self.ts_index].filter(fm)

class IntegralImage():

	def __init__(self,img):
		self.size = (img.size[1],img.size[0])
		self.__compute_integral_image(img)

	def __compute_integral_image(self,img):
		'''
			Integral Image is a special data structure to increase the speed of filter computation.
			ii[height][width] = Sum of all the pixels above and on the left of (height,width).
		'''
		self.__integral_image = np.zeros(self.size)

		line_index = 0
		col_index = 0
		line_sum = 0

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
		return p[FMF.HEIGHT] >= self.size[FMF.HEIGHT] or p[FMF.HEIGHT] < 0 or p[FMF.WIDTH] >= self.size[FMF.WIDTH] or p[FMF.WIDTH] < 0


	def __compute_block(self,block,location):
		'''
			When the block has just one column, I just need the Point C
			When the block has just one line, I just need the Point B
		'''
		block_sum=0
		for bl in block:
			point_A = ( location[FMF.Y] + bl[FMF.A][FMF.Y], location[FMF.X] + bl[FMF.A][FMF.X] )
			point_B = ( location[FMF.Y] + bl[FMF.B][FMF.Y], location[FMF.X] + bl[FMF.B][FMF.X] )
			point_C = ( location[FMF.Y] + bl[FMF.C][FMF.Y], location[FMF.X] + bl[FMF.C][FMF.X] )
			point_D = ( location[FMF.Y] + bl[FMF.D][FMF.Y], location[FMF.X] + bl[FMF.D][FMF.X] )

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
		pa = (pa[FMF.HEIGHT]-1,pa[FMF.WIDTH]-1)
		pb = (pb[FMF.HEIGHT]-1,pb[FMF.WIDTH])
		pc = (pc[FMF.HEIGHT],pc[FMF.WIDTH]-1)

		ii_list = []
		for p in [pa,pb,pc,pd]:
			if self.__outside_limits(p):
				ii = 0
			else:
				ii = self.__integral_image[p[FMF.HEIGHT]][p[FMF.WIDTH]]
			ii_list.append(ii)
		return ii_list		


	def filter(self,fm):
		if fm.size[0] > self.size[0] or fm.size[1] > self.size[1]:
			return 0

		white_sum = self.__compute_block(fm.mask.white,fm.location)
		black_sum = self.__compute_block(fm.mask.black,fm.location)

		return abs(black_sum-white_sum)						


class FeatureMaster():
	'''
		Class who will run the filter operations related to a TrainningSample and all the possible existing 
		FeatureMask accordingly with the ardis and shifts.
	'''
	TEST=False
	TS_SET = []
	NON_FACE = -1
	FACE = 1

	@staticmethod
	def init_training_set():
		FeatureMaster.TS_SET = []

	@staticmethod
	def trainning_samples():
		return FeatureMaster.TS_SET

	def __init__(self,ardis,shift_w,shift_h,resize_factor,start_w,start_h):
		self.ardis = ardis
		self.shift_w=shift_w
		self.shift_h=shift_h
		self.resize_factor=resize_factor
		self.start_w = start_w
		self.start_h = start_h

		self.feature_number=0
		self.feature_type=0

		self.m2hf = MaskTwoHorizontalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)		
		self.m2vf = MaskTwoVerticalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		self.m3hf = MaskThreeHorizontalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		self.m3vf = MaskThreeVerticalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)
		self.mdf = MaskDiagonalFactory(self.ardis,self.shift_w,self.shift_h,self.resize_factor,self.start_w,self.start_h)

		#Test purposes
		self.m2h_values = []
		self.m2v_values = []
		self.m3h_values = []
		self.m3v_values = []
		self.md_values = []

	def add_image(self,img_path,img_label):
		ts_index = len(FeatureMaster.TS_SET)
		FeatureMaster.TS_SET.append( TrainningSample(ts_index,img_path,img_label,self.ardis) )	

	def after_filter(self,ts_index,fm,filter_value,filter_list):
		self.feature_number+=1
		if FeatureMaster.TEST:
			filter_list.append(filter_value) 
		return TrainningSampleFeature(ts_index,self.feature_type,self.feature_number,filter_value,fm)

	def __get_frs_by_mask(self,mask_factory,filter_list):
		for fm,count in mask_factory.next():
			frs = FeatureResultSet()
			for ts in FeatureMaster.TS_SET:
				tsf = self.after_filter(ts.index,fm,ts.filter(fm),filter_list )
				frs.add_item(tsf)
			frs.sort()
			yield (frs,count)
			frs = None

	def get_next_frs(self):	
		for frs,count in self.__get_frs_by_mask(self.m2hf,self.m2h_values):
			yield frs

		for frs,count in self.__get_frs_by_mask(self.m2vf,self.m2v_values):
			yield frs

		for frs,count in self.__get_frs_by_mask(self.m3hf,self.m3h_values):
			yield frs

		for frs,count in self.__get_frs_by_mask(self.m3vf,self.m3v_values):
			yield frs

		for frs,count in self.__get_frs_by_mask(self.mdf,self.md_values):
			yield frs

class FeatureResultSet():
	'''
		This is the equivalent to the table of the notebook notation. In other words, this class holds all the
		image faces and scenes information for a single feature.
	'''


	'''T_MINUS and T_PLUS are the same for all the result sets'''
	T_PLUS=0
	T_MINUS=0

	@staticmethod
	def t_plus(tp=None):
		if tp==None:
			return FeatureResultSet.T_PLUS
		else:
			FeatureResultSet.T_PLUS = tp

	@staticmethod
	def t_minus(tm=None):
		if tm==None:
			return FeatureResultSet.T_MINUS
		else:
			FeatureResultSet.T_MINUS = tm

	@staticmethod
	def initialize_weights(trainning_samples,initial_weight_faces,initial_weight_non_faces):
		t_plus = 0
		t_minus = 0
		for ts in trainning_samples:
			if ts.is_face():
				ts.weight(initial_weight_faces)
				t_plus+=initial_weight_faces
			else:
				ts.weight(initial_weight_non_faces)				
				t_minus+=initial_weight_non_faces
				
		FeatureResultSet.t_plus(t_plus)
		FeatureResultSet.t_minus(t_minus)			

	def __init__(self):
		self.face = []		#List of TrainningSampleFeature labeled as a face
		self.non_face = []	#list of TrainningSampleFeature labeled as a scene

		self.t_plus = FeatureResultSet.t_plus
		self.t_minus = FeatureResultSet.t_minus

		#There is no need to store this but tests purposes!!
		self.errors_dir = []	#Pair with error in both directions

		self.all_results_sorted = []

	def add_item(self,item):
		if item.is_face():
			self.face.append(item)
		else:
			self.non_face.append(item)	

	def sort(self):
		self.all_results_sorted.extend( self.face )
		self.all_results_sorted.extend( self.non_face )
		self.all_results_sorted.sort()		

	def prepare_tables(self):
		s_plus = 0
		s_minus = 0
		min_error = (100,0,0)	#Error_Value, Direction, TSF Index
		tables = [ [],[] ]

		#Backward loop is better in this case
		for i in range(len(self.all_results_sorted)-1,-1,-1):
			e1 = s_plus + (self.t_minus() - s_minus)	#Everything below is a non-face			
			e2 = s_minus + (self.t_plus() - s_plus)		#Everything below is a face
			
			
			if FeatureMaster.TEST:
				self.errors_dir.append( (e1,e2) )

			if e1<min_error[0]:
				min_error = (e1,1,i)
			elif e2<min_error[0]:
				min_error = (e2,-1,i)

			# print s_plus,self.t_minus(),s_minus,e1,min_error[0]==0,min_error
			# print s_plus,self.t_minus(),s_minus,e2,min_error[0]==0,min_error

			tsf = self.all_results_sorted[i]

			#TEST PURPOSES
			if FeatureMaster.TEST:
				t1_line = (	tsf.image_name(),
							tsf.filter_value,
							tsf.label(),
							self.t_plus(),
							self.t_minus(),
							s_plus,
							s_minus,
							e1,
							e1==0)			

				t2_line = (	tsf.image_name(),
							tsf.filter_value,
							tsf.label(),
							self.t_plus(),
							self.t_minus(),
							s_plus,
							s_minus,
							e2,
							e2==0)				

				tables[0].append(t1_line)		
				tables[1].append(t2_line)

			if tsf.is_face():
				s_plus+=tsf.weight()
			else:
				s_minus+=tsf.weight()
			

		#self.print_table(tables)

		#Filter_Value, Direction, Error_Value, FeatureMask
		return ( self.all_results_sorted[ min_error[2] ].filter_value, 
				 min_error[1],  
				 min_error[0],
				 self.all_results_sorted[ min_error[2] ].fm )			

	def update_weights(self,beta,best_frs,fc,best_threshold):
		t = best_threshold[0]
		d = best_threshold[1]

		t_plus = 0
		t_minus = 0
		for tsf in best_frs.all_results_sorted:

			if (tsf.filter_value*d) < (t*d):
				#Classifica como nao-face
				if tsf.is_face():
					#Classificado Incorretamente
					new_weight = tsf.weight()*1.0
				else:
					new_weight = tsf.weight()*beta
			else:
				#Classifica como face
				if tsf.is_face():
					#Classificado Corretamente
					new_weight = tsf.weight()*beta
				else:
					new_weight = tsf.weight()*1.0


			if tsf.is_face():
				t_plus+=new_weight
			else:
				t_minus+=new_weight

			tsf.weight(new_weight)

		self.t_plus(t_plus)
		self.t_minus(t_minus)

	def print_table(self,tables):
		print "Imagem \t\t\t\t\t Mascara \t Label \t T+ \t T- \t S+ \t S- \t Erro \t Erro 0"
		for l in tables[0]:
			print "%s \t\t\t\t\t %d \t %d \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %s" % l

		print "\n"

			

class FeatureChooser():
	'''
		At this point, all the feature tables will be already computed. The next step is to compute the sum wiights
		of both directions. This class is responsible for update the weights and update the auxiliar variables used
		to compute the minimizer
	'''

	def __init__(self,ardis,fm,n_faces,n_non_faces):
		self._ardis = ardis
		self._fm = fm
		self._initial_weight_faces = 1.0/(n_faces+n_non_faces)
		self._initial_weight_non_faces = 1.0/(n_faces+n_non_faces)

		print self._initial_weight_faces, self._initial_weight_non_faces


	@staticmethod
	def find_hypothesis_from_previous_state(filepath):		
		variables = FeatureChooser.__load_state(filepath)
		return variables[0].__find_hypothesis(variables[1],variables[2],variables[3],variables[4])

	def find_hypothesis(self,T):
		FeatureResultSet.initialize_weights(FeatureMaster.trainning_samples(),
											self._initial_weight_faces,
											self._initial_weight_non_faces)

		cur_T = 0
		max_alpha = 0
		fc = FinalClassifier(self._ardis)		

		return self.__find_hypothesis(cur_T,max_alpha,fc,T)


	def __find_hypothesis(self,cur_T,max_alpha,fc,T):

		while cur_T<T:
			# print "UIUI"
			#Find the best feature. The one with the lowest error.
			best_threshold = (0,0,100)
			best_frs = None
			for frs in self._fm.get_next_frs():
				th_attempt = frs.prepare_tables()
				if th_attempt[2]<best_threshold[2]:
					best_threshold = th_attempt
					best_frs = frs


			e_t = best_threshold[2]
			b_t = (e_t)/(1-e_t)

			print T,e_t,b_t
			if b_t <= 1e-8:
				a_t = math.log( 1.0/(1e-8) )
			else:				
				a_t = math.log( 1.0/b_t )

			#Filter_Value, Direction, Error_Value, FeatureMask, alpha
			h_t = (	best_threshold[0],
					best_threshold[1],
					best_threshold[2],
					best_threshold[3],
					a_t)


			fc.add_hypothesis(h_t)
			best_frs.update_weights(b_t,best_frs,fc,best_threshold)

			cur_T+=1
			if cur_T%3==0:
				self.__save_state(cur_T,fc,max_alpha,T)

				with open('classifiers/%s_(%d-%d).pk' % ('classifier',cur_T,T),'wb') as output:
					fc.final=True
					pickle.dump(fc,output,pickle.HIGHEST_PROTOCOL)									
					fc.final=False

			if e_t<1e-10:
				#The error is already too small, doesn`t need to keep doing it.
				break

		fc.close_classifier()
		return fc

	def __save_state(self,cur_T,fc,max_alpha,T):
		ts_state_collection = []
		for ts in FeatureMaster.trainning_samples():
			ts_state_obj = (ts.index,
							ts.img_path,
							ts.label,
							ts.weight() )

			ts_state_collection.append(ts_state_obj)

		state_obj = (self._fm.ardis,
					 self._fm.shift_w,
					 self._fm.shift_h,
					 self._fm.resize_factor,
					 self._fm.start_w,
					 self._fm.start_h,
					 fc,
					 cur_T,
					 ts_state_collection,
					 max_alpha,
					 T)

		with open("classifiers/states/featureChooser_%d.pk" % (cur_T,), "wb") as output:
			pickle.dump(state_obj,output,pickle.HIGHEST_PROTOCOL)			


	@staticmethod
	def __load_state(filepath):	
		state_obj = None
		with open(filepath, "r") as input:
			state_obj = pickle.load(input)		

		FeatureMaster.init_training_set()
		fm = FeatureMaster(state_obj[0],state_obj[1],state_obj[2],state_obj[3],state_obj[4],state_obj[5])
		
		ts_state_collection = state_obj[8]

		n_faces = 0
		n_non_faces = 0
		t_plus = 0
		t_minus = 0
		for ts_state in ts_state_collection:
			fm.add_image(ts_state[1],ts_state[2])
			fm.trainning_samples()[-1].weight( ts_state[3] )

			if ts_state[2]==FeatureMaster.FACE:
				n_faces+=1
				t_plus+= ts_state[3]
			else:
				n_non_faces+=1
				t_minus+= ts_state[3]

		FeatureResultSet.t_plus(t_plus)
		FeatureResultSet.t_minus(t_minus)

		chooser = FeatureChooser(state_obj[0],fm,n_faces,n_non_faces)

		#Chooser, cur_T,max_alpha,fc,T
		return ( chooser, state_obj[7],state_obj[9],state_obj[6],state_obj[10] )

class FinalClassifier():
	def __init__(self,ardis):
		self.final=False

		self.hypothesis = []		
		self.ardis = ardis

	def add_hypothesis(self,h):
		self.hypothesis.append(h)

	def close_classifier(self):
		self.final = True

	def h_function(self,f,d,t):		
		'''
			When d=1, it means that everything below (filter_value greater than threshold)
			is a non-face, and everything up and equal (filter_value lesser or equal than threshold)
			is classified as a face.
		'''
		if (d*f)<(d*t):
			return 1
		else:
			return 0

	def set_image(self,img):
		self.ii = IntegralImage(img)

	def __is_face(self,subwindow,tsf=None):
		#tsf is used only with partial classifiers.
		sx=0
		sa=0
		for h in self.hypothesis:			
			t = h[0]	#threshold
			d = h[1]	#direction
			f = h[3]	#feature mask
			a = h[4]	#alpha

			if self.final:
				f.adjust_mask(subwindow)
				sx+= a*self.h_function(self.ii.filter(f),d,t)
			else:
				sx+= a*self.h_function(tsf.filter(f),d,t)
			sa+= a

		# print sx,sa*0.5
		if sx>=(0.5*sa):
			return True
		else:
			return False

	def is_face(self,*args,**kwargs):
		if not self.final:
			return self.__is_face(None,tsf=kwargs["tsf"])
		else:
			return self.__is_face(args[0])


#FEATURE MASTER
# im = Image.open("/home/daniel/Desktop/lfwcrop_grey/faces/Abdullah_Gul_0004.pgm")
# fm = FeatureMaster((64,64),4,4,1.25,4,4,im,True)
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

