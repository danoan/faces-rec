#coding:utf-8

import time
import numpy as np
import math
import Image,os,pickle

import misc,config
import integral_image as testII
from features import *

class TrainningSampleImage():
	'''
		Class Container that holds a trainning image file name, its label: True for face and False for non-face
		and support information like weight, s_plus and s_minus used by the adaboost algorithm
	'''
	def __init__(self,tsi_index,img_path,label,ardis):
		self.index = tsi_index
		self.img_path = img_path
		self.img_filename = img_path.split(os.path.sep)[-1]		
		self.label = label

		# self.ii = misc.IntegralImage(img_path)
		testII.setImage(img_path)

		#Variables used for the minimization problem of finding the best feature.
		self.__weight = 0

	def weight(self,w=None):
		if w==None:
			return self.__weight
		else:
			self.__weight=w

	def filter(self,fm):
		# return self.ii.filter(fm)
		return testII.integral(fm.location[1],fm.location[0],len(fm.mask.white),len(fm.mask.black),*fm.mask.c_args)		

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
	def __init__(self,tsi_index,feature_type,feature_number,filter_value,fm):
		self.tsi_index = tsi_index	#TrainningSample Index on FeatureMaster.TS
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
		return FeatureMaster.TS_SET[self.tsi_index].img_filename

	def is_face(self):
		return FeatureMaster.TS_SET[self.tsi_index].label==FeatureMaster.FACE

	def label(self):
		return FeatureMaster.TS_SET[self.tsi_index].label

	def weight(self,w=None):
		if w==None:
			return FeatureMaster.TS_SET[self.tsi_index].weight()
		else:
			FeatureMaster.TS_SET[self.tsi_index].weight(w)

	def filter(self,fm):
		return FeatureMaster.TS_SET[self.tsi_index].filter(fm)


class FeatureMaster():
	'''
		Class who will run the filter operations related to a TrainningSampleFeature and all the possible existing 
		FeatureMask accordingly with the ardis and shifts.
	'''
	TS_SET = []
	NON_FACE = -1
	FACE = 1

	@staticmethod
	def init_training_set():
		FeatureMaster.TS_SET = []

	@staticmethod
	def tsi_collection():
		return FeatureMaster.TS_SET

	def __init__(self,ardis,shift_w,shift_h,resize_factor,start_w,start_h,test=False):
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
		self.__test__ = test
		self.m2h_values = []
		self.m2v_values = []
		self.m3h_values = []
		self.m3v_values = []
		self.md_values = []

	def add_image(self,img_path,img_label):
		tsi_index = len(FeatureMaster.TS_SET)
		FeatureMaster.TS_SET.append( TrainningSampleImage(tsi_index,img_path,img_label,self.ardis) )	

	def after_filter(self,tsi_index,fm,filter_value,filter_list):
		self.feature_number+=1
		if self.__test__:
			filter_list.append(filter_value) 
		return TrainningSampleFeature(tsi_index,self.feature_type,self.feature_number,filter_value,fm)

	def __get_frs_by_mask(self,mask_factory,filter_list):
		for fmask,count in mask_factory.next():
			frs = FeatureResultSet()
			for tsi in FeatureMaster.TS_SET:
				tsf = self.after_filter(tsi.index,fmask,tsi.filter(fmask),filter_list )
				frs.add_item(tsf)
			frs.sort()
			yield (frs,count)
			frs = None

	def get_next_frs(self):	
		self.feature_type = 0
		for frs,count in self.__get_frs_by_mask(self.m2hf,self.m2h_values):			
			yield frs
		print "M2H %d" % (count,)

		self.feature_type = 1
		for frs,count in self.__get_frs_by_mask(self.m2vf,self.m2v_values):
			yield frs
		print "M2V %d" % (count,)

		self.feature_type = 2
		for frs,count in self.__get_frs_by_mask(self.m3hf,self.m3h_values):
			yield frs
		print "M3H %d" % (count,)			

		self.feature_type = 3
		for frs,count in self.__get_frs_by_mask(self.m3vf,self.m3v_values):
			yield frs
		print "M3V %d" % (count,)

		self.feature_type = 4
		for frs,count in self.__get_frs_by_mask(self.mdf,self.md_values):
			yield frs
		print "MD %d" % (count,)

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
	def initialize_weights(tsi_collection,initial_weight_faces,initial_weight_non_faces):
		t_plus = 0
		t_minus = 0
		for tsi in tsi_collection:
			if tsi.is_face():
				tsi.weight(initial_weight_faces)
				t_plus+=initial_weight_faces
			else:
				tsi.weight(initial_weight_non_faces)				
				t_minus+=initial_weight_non_faces
				
		FeatureResultSet.t_plus(t_plus)
		FeatureResultSet.t_minus(t_minus)			

	def __init__(self):
		self.t_plus = FeatureResultSet.t_plus
		self.t_minus = FeatureResultSet.t_minus

		self.faces_count = 0
		self.non_faces_count = 0

		self.all_results_sorted = []

	def add_item(self,item):
		self.all_results_sorted.append(item)

		if item.is_face():
			self.faces_count+=1
		else:
			self.non_faces_count+=1

	def sort(self):
		self.all_results_sorted.sort()		

	def prepare_tables(self):
		s_plus = 0
		s_minus = 0
		min_error = (100,0,0)	#Error_Value, Direction, TSF Index

		#Backward loop is better in this case
		for i in range(len(self.all_results_sorted)-1,-1,-1):
			e1 = s_plus + (self.t_minus() - s_minus)	#Everything below is a non-face			
			e2 = s_minus + (self.t_plus() - s_plus)		#Everything below is a face
			

			if e1<min_error[0]:
				min_error = (e1,1,i)
			elif e2<min_error[0]:
				min_error = (e2,-1,i)

			tsf = self.all_results_sorted[i]

			if tsf.is_face():
				s_plus+=tsf.weight()
			else:
				s_minus+=tsf.weight()
		
		# print s_plus,self.t_plus(),s_minus,self.t_minus(),min_error

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

			if (tsf.filter_value*d) > (t*d):	#Se esta abaixo da tabela (filtro > threshold), eh classificado como nao-face
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

		norma=0
		for tsf in best_frs.all_results_sorted:	
			if tsf.is_face():
				tsf.weight( tsf.weight()/(t_plus+t_minus) )
			else:
				tsf.weight( tsf.weight()/(t_plus+t_minus) )

			norma+=tsf.weight()

		print "NORMA: %.4f \n T_PLUS: %.4f \n T_MINUS: %.4f \n T_PLUS+T_MINUS: %.4f" % (norma,t_plus,t_minus,(t_plus+t_minus))
		self.t_plus(t_plus/(t_plus+t_minus))
		self.t_minus(t_minus/(t_plus+t_minus))

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
		self._n_faces = n_faces
		self._state_prefix_name = "featureChooser_%d" % (n_faces,)

		print self._initial_weight_faces, self._initial_weight_non_faces


	@staticmethod
	def find_hypothesis_from_previous_state(filepath):		
		variables = FeatureChooser.__load_state(filepath)
		return variables[0].__find_hypothesis(variables[1],variables[2],variables[3],variables[4])

	def find_hypothesis(self,T):
		FeatureResultSet.initialize_weights(FeatureMaster.tsi_collection(),
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

			print "IT: %d \n E_T: %.4f \n B_T: %.4f" % (cur_T,e_t,b_t)
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

				with open('%s/%s_%d_(%d-%d).pk' % (config.CLASSIFIERS_PATH,'classifier',self._n_faces,cur_T,T),'wb') as output:
					fc.final=True
					pickle.dump(fc,output,pickle.HIGHEST_PROTOCOL)									
					fc.final=False

			if e_t<1e-10:
				#The error is already too small, doesn`t need to keep doing it.
				break

		fc.close_classifier()
		return fc

	def __save_state(self,cur_T,fc,max_alpha,T):
		tsi_state_collection = []
		for tsi in FeatureMaster.tsi_collection():
			tsi_state_obj = (tsi.index,
							tsi.img_path,
							tsi.label,
							tsi.weight() )

			tsi_state_collection.append(tsi_state_obj)

		state_obj = (self._fm.ardis,
					 self._fm.shift_w,
					 self._fm.shift_h,
					 self._fm.resize_factor,
					 self._fm.start_w,
					 self._fm.start_h,
					 fc,
					 cur_T,
					 tsi_state_collection,
					 max_alpha,
					 T)

		with open("%s/%s_%d.pk" % (config.STATES_PATH,self._state_prefix_name,cur_T,), "wb") as output:
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
			fm.tsi_collection()[-1].weight( ts_state[3] )

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

	def set_image(self,img_path):
		# self.ii = misc.IntegralImage(img_path)
		testII.setImage(img_path)

	def __is_face(self,subwindow,tsf=None,ac=0.5):
		#tsf is used only with partial classifiers.
		sx=0
		sa=0

		
		for h in self.hypothesis:			
			t = h[0]	#threshold
			d = h[1]	#direction
			f = h[3]	#feature mask
			a = h[4]	#alpha

			if self.final:
				# ii_time = time.time()
				f.adjust_mask(subwindow)				
				# print time.time() - ii_time
				# sx+= a*self.h_function(self.ii.filter(f),d,t)			
				sx+= a*self.h_function(testII.integral(f.location[1],f.location[0],len(f.mask.white),len(f.mask.black),*f.mask.c_args),d,t)
			else:
				sx+= a*self.h_function(tsf.filter(f),d,t)
			sa+= a

		
		# print sx, subwindow.y, subwindow.x, subwindow.size
		if sx>=(ac*sa):
			return True
		else:
			return False

	def is_face(self,*args,**kwargs):
		if not self.final:
			return self.__is_face(None,**kwargs)
		else:
			return self.__is_face(*args,**kwargs)


#FEATURE MASTER
# im = Image.open("/home/daniel/Desktop/lfwcrop_grey/faces/Abdullah_Gul_0004.pgm")
# fm = FeatureMaster((64,64),4,4,1.25,4,4,im,True)
# fm.compute_features()
# print fm.m2h_values,fm.m2v_values,fm.m3h_values,fm.m3v_values,fm.md_values

