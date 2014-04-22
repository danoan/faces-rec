#coding:utf-8

import os,pickle,random,time
from multiprocessing import Process, Pipe, Pool

import windowgen,config
from trainning_tools import *
from detector import *
import labeled_images,analyser
import numpy as np

def standard_detector():
	classifier_filepath = '3500/new_classifier_3500_(3-100).pk'
	n_gen = 6
	ref_ardis = (64,64)
	ref_mask = (8,8)
	ss=1
	window_strategy=windowgen.SubwindowGenerator.FIXED_FACTOR

	with open('%s/%s' % (config.CLASSIFIERS_PATH,classifier_filepath),'rb') as input:
		classifier = pickle.load(input)

	det = Detector(classifier,n_gen,ref_ardis,ref_mask,shift_step=ss,fn=window_strategy,ac=0.5)				

	return det

class Experiment():	
	def setup(self,title):
		self.time = 0
		self.title = title

		return self

	def run(self):
		self.time = time.time()

	def stop(self):
		self.time = time.time() - self.time
		print "END %s: %s" % (self.title,self.time)


class Experiment1(Experiment):
	'''
		Test classifier against Seinfeld's image
	'''

	def setup(self):
		Experiment.setup(self,"Experiment1")
		self.det = standard_detector()
		return self

	def run(self):
		Experiment.run(self)
		li = labeled_images.li2()
		dr = DetectorReport(li,"EXPERIMENT 1")
		ret = self.det.detect_faces(li.image_filepath(),dr)
		self.stop()
		return ret

class Experiment2(Experiment):
	'''
		Test the classifier against the test database
	'''

	def setup(self):
		Experiment.setup(self,"Experiment2")
		self.n_img = 1000
		self.det = standard_detector()
		return self

	def run(self):
		Experiment.run(self)
		# face_filenames = random.sample(os.listdir(config.FACES_PATH),self.n_img)
		face_filenames = os.listdir(config.FACES_PATH)[:self.n_img]

		sw = windowgen.Subwindow(0,0,(64,64))

		true=0
		for i in face_filenames:
			img_path = "%s/%s" % (config.FACES_PATH,i)
			true = true+1 if self.det.is_face(img_path,sw) else true
		
		print "%d/%d" % (true,self.n_img)

		true=0
		for scene_window in windowgen.get_next_random_image_window(config.SCENES_PATH_VALIDATION,self.n_img):
			img_path = scene_window.window_img_path
			true = true+1 if self.det.is_face(img_path,sw) else true		

		print "%d/%d" % (true,self.n_img)			

		self.stop()


class Experiment3(Experiment):
	'''
		Acceptance Coefficiente Analysis
		Different Shift Steps
		Different Classifiers
		Different WindowStrategy

		resultados/coeficiente aceitacao/classifier_1/...
		resultados/coeficiente aceitacao/classifier_2/...
	'''

	def setup(self):
		Experiment.setup(self,"Experiment3")
		self.n_gen = 6
		self.ref_ardis = (64,64)
		self.ref_mask = (8,8)

		self.classifiers_files = os.listdir(config.CLASSIFIERS_3500_PATH)
		self.ss_range = [16,8,4,2]
		self.ws_range = [	windowgen.SubwindowGenerator.FIXED_FACTOR,
							windowgen.SubwindowGenerator.DYNAMIC_FACTOR]
		self.ac_range = np.linspace(0.7,0.85,20)

		# self.classifiers_files = ["%s/%s" % (config.CLASSIFIERS_3500_PATH,"classifier_3500_(54-100).pk")]
		# self.ss_range = [16]
		# self.ws_range = [windowgen.SubwindowGenerator.FIXED_FACTOR]
		# self.ac_range = np.linspace(0.5,1.0,4)		

		self.li = labeled_images.li1()

		return self

	def __get_next_input__(self):
		
		for cf in self.classifiers_files:
			classifier = None
			class_name = ( "".join( cf.split("_")[-1] ) )[:-3]
			with open("%s/%s" % (config.CLASSIFIERS_3500_PATH,cf),'rb') as input:
				classifier = pickle.load(input)				

			for ss in self.ss_range:
				for ws in self.ws_range:
					graph_filepath = "%s/%s/%s" % (config.GRAPH_PATH,"Coeficiente Aceitacao",cf.split("/")[-1])
					graph_filepath = "%s/shift_step_%d_ws_%s_%s" % (graph_filepath,ss,ws.func_name,class_name)
					new_anal=True
					print graph_filepath
					for ac in self.ac_range:	
						last_entry = ac==self.ac_range[-1]
						yield (classifier,ss,ws,ac,graph_filepath,new_anal,last_entry)
						new_anal=False	
						

	def __main_process__(self,li,classifier,n_gen,ref_ardis,ref_mask,shift_step,fn,ac,cconn):
		det = Detector(classifier,n_gen,ref_ardis,ref_mask,shift_step=shift_step,fn=fn,ac=ac)
		dr = DetectorReport(li,ac=ac,shift_step=shift_step,report_name="EXPERIMENT 1")
		
		final_dr = det.detect_faces(li.image_filepath(),dr)
		cconn.send( final_dr )

	def join_and_free(self,procs,callback,pipe_conn):
		while len(procs)>0:
			if callback:
				callback(pipe_conn.recv())					

			procs.pop().join()		

	def update_analyser(self,report):
		data = report.data()

		self.anal.open_item()
		self.anal.put_value("ac",data["ac"])
		self.anal.put_value("false_positive",data["false_positive"])
		self.anal.put_value("false_negative",data["false_negative"])
		self.anal.put_value("faces",data["faces"])
		self.anal.close_item()
		
	def run(self):
		Experiment.run(self)
		callback = self.update_analyser

		procs = []
		pconn,cconn = Pipe()		

		#Classifier, ShiftStepp, WindowStrategy, AcceptanceCoefficient
		self.anal = None
		for c,ss,ws,ac,graph_filepath,new_anal,le in self.__get_next_input__():

			#New Analyser it means a new graph
			if new_anal:
				self.anal = analyser.AcceptanceCoefficientAnalyser(title="Resultados variando coeficiente de aceitacao",
							   	  								   graph_filepath=graph_filepath)	

			p = Process(target= self.__main_process__, args=[self.li,
														c,
														self.n_gen,
													   	self.ref_ardis,
													   	self.ref_mask,
													   	ss,
													   	ws,
													   	ac,
													   	cconn])
			p.start()
			procs.append(p)

			#If 4 processes running or is the last entry of the analyser, wait them to finish
			if len(procs)>=4 or le:
				self.join_and_free(procs,callback,pconn)
				if le:
					self.anal.display_results()			
			


		self.join_and_free(procs,callback,pconn)
		self.stop()

class Experiment4(Experiment):
	'''
		Executa sobre os exemplos do banco de teste

		Acceptance Coefficiente Analysis
		Different Classifiers
		ShiftStep==1		
		WS = FixedFactor

		resultados/coeficiente aceitacao/classifier_1/...
		resultados/coeficiente aceitacao/classifier_2/...
	'''

	def setup(self):
		Experiment.setup(self,"Experiment4")

		self.n_gen = 4
		self.ref_ardis = (64,64)
		self.ref_mask = (8,8)

		def fixed_factor(*args,**kwargs):
			return 1.0

		# self.classifiers_files = os.listdir(config.CLASSIFIERS_3500_PATH)
		self.classifiers_files = ["classifier_3500_(3-100).pk"]
		self.ss_range = [1]
		self.ws_range = [fixed_factor]
		self.ac_range = np.linspace(0.5,1.0,25)

		self.li_list = [labeled_images.li2(),labeled_images.liKramer(),labeled_images.liJerry(),labeled_images.liElaine()]
		# self.li_list.extend(labeled_images.li_faces(1000))
		# self.li_list.extend(labeled_images.li_non_faces(1000))

		self.min_error = 1e6
		self.min_error_ac = None
		self.error_anal = analyser.FeatureErrorAnalyser(title="Coeficiente de Aceitacao Minimo para erro 0 por feature",
												graph_filepath="%s/%s/%s" % (config.GRAPH_PATH,"Coeficiente Aceitacao Banco","error_analysis"))
		return self

	def __get_next_input__(self):
		
		for cf in self.classifiers_files:
			classifier = None
			class_name = ( "".join( cf.split("_")[-1] ) )[:-3]
			feat = int( class_name.split("-")[0][1:] )

			with open("%s/%s" % (config.CLASSIFIERS_3500_PATH,cf),'rb') as input:
				classifier = pickle.load(input)				

			for ss in self.ss_range:
				for ws in self.ws_range:
					graph_filepath = "%s/%s/%s" % (config.GRAPH_PATH,"Coeficiente Aceitacao Banco",cf.split("/")[-1])
					graph_filepath = "%s/shift_step_%d_ws_%s_%s" % (graph_filepath,ss,ws.func_name,class_name)
					new_anal=True
					print graph_filepath
					for ac in self.ac_range:							
						last_entry = ac==self.ac_range[-1]												
						yield (classifier,ss,ws,ac,graph_filepath,new_anal,last_entry,feat)
						new_anal=False	


	def __main_process__(self,classifier,n_gen,ref_ardis,ref_mask,shift_step,fn,ac,cconn):
		det = Detector(classifier,n_gen,ref_ardis,ref_mask,shift_step=shift_step,fn=fn,ac=ac)
		final_dr = DetectorReport(None,ac=ac,shift_step=shift_step,report_name="EXPERIMENT 1")		
		
		for li in self.li_list:		
			partial_dr = DetectorReport(li,ac=ac,shift_step=shift_step,report_name="EXPERIMENT 1")						
			partial_dr = det.detect_faces(li.image_filepath(),partial_dr)
			
			final_dr.sum(partial_dr)

		error = (final_dr.false_positive+final_dr.false_negative,ac)

		cconn.send( (final_dr,error) )

	def join_and_free(self,procs,callback,pipe_conn):
		while len(procs)>0:
			if callback:
				callback(pipe_conn.recv())					

			procs.pop().join()		

	def update_analyser(self,result):
		report = result[0]
		error = result[1][0]
		ac = result[1][1]

		if error<=self.min_error:			
			self.min_error = error
			if self.min_error_ac==None:
				self.min_error_ac = ac
			elif ac < self.min_error_ac:
				self.min_error_ac = ac

		data = report.data()

		self.anal.open_item()
		self.anal.put_value("ac",data["ac"])
		self.anal.put_value("false_positive",data["false_positive"])
		self.anal.put_value("false_negative",data["false_negative"])
		self.anal.put_value("faces",data["faces"])
		self.anal.close_item()

	def analyser_error_check(self,feat):		
		self.error_anal.open_item()
		self.error_anal.put_value("feature",feat)
		self.error_anal.put_value("ac",self.min_error_ac)
		self.error_anal.close_item()

		self.min_error = 1e6
		self.min_error_ac = None		
		
	def run(self):
		Experiment.run(self)
		callback = self.update_analyser

		procs = []
		pconn,cconn = Pipe()		

		#Classifier, ShiftStepp, WindowStrategy, AcceptanceCoefficient
		self.anal = None
		for c,ss,ws,ac,graph_filepath,new_anal,le,feat in self.__get_next_input__():

			#New Analyser it means a new graph
			if new_anal:				
				self.anal = analyser.AcceptanceCoefficientAnalyser(title="Resultados variando coeficiente de aceitacao",
							   	  								   graph_filepath=graph_filepath)	

			p = Process(target= self.__main_process__, args=[c,
														self.n_gen,
													   	self.ref_ardis,
													   	self.ref_mask,
													   	ss,
													   	ws,
													   	ac,
													   	cconn])
			p.start()
			procs.append(p)

			#If 4 processes running or is the last entry of the analyser, wait them to finish
			if len(procs)>=4 or le:
				self.join_and_free(procs,callback,pconn)
				if le: 	
					self.analyser_error_check(feat)
					self.anal.display_results()			
			


		self.join_and_free(procs,callback,pconn)
		self.error_anal.display_results()
		print self.error_anal.data()
		self.stop()



class Experiment5(Experiment):
	'''
		Executa sobre os exemplos do banco de validacao
		
		Acceptance Coefficiente Analysis
		Different Classifiers
		ShiftStep==1		
		WS = FixedFactor

		resultados/coeficiente aceitacao/classifier_1/...
		resultados/coeficiente aceitacao/classifier_2/...
	'''

	def setup(self):
		Experiment.setup(self,"Experiment5")
		self.n_gen = 4
		self.ref_ardis = (64,64)
		self.ref_mask = (8,8)

		def fixed_factor(*args,**kwargs):
			return 1.0

		self.classifiers_files = os.listdir(config.CLASSIFIERS_3500_PATH)
		# self.classifiers_files = ["classifier_3500_(30-100).pk","classifier_3500_(33-100).pk"]
		self.ss_range = [1]
		self.ws_range = [fixed_factor]
		self.ac_range = np.linspace(0.5,1.0,25)

		self.li_list = []
		self.li_list.extend(labeled_images.li_faces_validation(3000))
		self.li_list.extend(labeled_images.li_non_faces_validation(3000))

		self.min_error = 1e6
		self.min_error_ac = None
		self.error_anal = analyser.FeatureErrorAnalyser(title="Coeficiente de Aceitacao Minimo para erro 0 por feature",
												graph_filepath="%s/%s/%s" % (config.GRAPH_PATH,"Coeficiente Aceitacao Validacao","error_analysis"))
		return self

	def __get_next_input__(self):
		
		for cf in self.classifiers_files:
			classifier = None
			class_name = ( "".join( cf.split("_")[-1] ) )[:-3]
			feat = int( class_name.split("-")[0][1:] )

			with open("%s/%s" % (config.CLASSIFIERS_3500_PATH,cf),'rb') as input:
				classifier = pickle.load(input)				

			for ss in self.ss_range:
				for ws in self.ws_range:
					graph_filepath = "%s/%s/%s" % (config.GRAPH_PATH,"Coeficiente Aceitacao Validacao",cf.split("/")[-1])
					graph_filepath = "%s/shift_step_%d_ws_%s_%s" % (graph_filepath,ss,ws.func_name,class_name)
					new_anal=True
					print graph_filepath
					for ac in self.ac_range:							
						last_entry = ac==self.ac_range[-1]												
						yield (classifier,ss,ws,ac,graph_filepath,new_anal,last_entry,feat)
						new_anal=False	


	def __main_process__(self,classifier,n_gen,ref_ardis,ref_mask,shift_step,fn,ac,cconn):
		det = Detector(classifier,n_gen,ref_ardis,ref_mask,shift_step=shift_step,fn=fn,ac=ac)
		final_dr = DetectorReport(None,ac=ac,shift_step=shift_step,report_name="EXPERIMENT 1")		
		
		for li in self.li_list:		
			partial_dr = DetectorReport(li,ac=ac,shift_step=shift_step,report_name="EXPERIMENT 1")						
			partial_dr = det.detect_faces(li.image_filepath(),partial_dr)
			
			final_dr.sum(partial_dr)

		error = (final_dr.false_positive+final_dr.false_negative,ac)

		cconn.send( (final_dr,error) )

	def join_and_free(self,procs,callback,pipe_conn):
		while len(procs)>0:
			if callback:
				callback(pipe_conn.recv())					

			procs.pop().join()		

	def update_analyser(self,result):
		report = result[0]
		error = result[1][0]
		ac = result[1][1]

		if error<=self.min_error:			
			self.min_error = error
			if self.min_error_ac==None:
				self.min_error_ac = ac
			elif ac < self.min_error_ac:
				self.min_error_ac = ac

		data = report.data()

		self.anal.open_item()
		self.anal.put_value("ac",data["ac"])
		self.anal.put_value("false_positive",data["false_positive"])
		self.anal.put_value("false_negative",data["false_negative"])
		self.anal.put_value("faces",data["faces"])
		self.anal.close_item()

	def analyser_error_check(self,feat):		
		self.error_anal.open_item()
		self.error_anal.put_value("feature",feat)
		self.error_anal.put_value("ac",self.min_error_ac)
		self.error_anal.close_item()

		self.min_error = 1e6
		self.min_error_ac = None		
		
	def run(self):
		Experiment.run(self)
		callback = self.update_analyser

		procs = []
		pconn,cconn = Pipe()		

		#Classifier, ShiftStepp, WindowStrategy, AcceptanceCoefficient
		self.anal = None
		for c,ss,ws,ac,graph_filepath,new_anal,le,feat in self.__get_next_input__():

			#New Analyser it means a new graph
			if new_anal:				
				self.anal = analyser.AcceptanceCoefficientAnalyser(title="Resultados variando coeficiente de aceitacao",
							   	  								   graph_filepath=graph_filepath)	

			p = Process(target= self.__main_process__, args=[c,
														self.n_gen,
													   	self.ref_ardis,
													   	self.ref_mask,
													   	ss,
													   	ws,
													   	ac,
													   	cconn])
			p.start()
			procs.append(p)

			#If 4 processes running or is the last entry of the analyser, wait them to finish
			if len(procs)>=4 or le:
				self.join_and_free(procs,callback,pconn)
				if le: 	
					self.analyser_error_check(feat)
					self.anal.display_results()			
			


		self.join_and_free(procs,callback,pconn)
		self.error_anal.display_results()
		print self.error_anal.data()
		self.stop()


if __name__=="__main__":
	# r1 = Experiment1().setup().run()
	# print r1
	# r1.show_image()
	# print Experiment2().setup().run()
	# Experiment3().setup().run()
	Experiment4().setup().run()
	# Experiment5().setup().run()




