#coding:utf-8
import config,pickle


class ResultsetManager():
	'''
		This classes works like a cache and a buffer. If a FeatureResultSet have not been creted yet,
		it creates it using the correspondent FeatureMask and pushs it in the current list of items. 
		When this list gets full (with list_size items), this class saves the list into a file and start
		a new empty list. This process is repeated until all the FeatureResultSet are created.

		After that, the FeatureResultSet will be asked again. At this moment, this class will open the 
		correspondent file that contains the requested FeatureResultSet and put it again in the memory.

		This will optimize the execution of the code just because of the following hypothesis:
			In each iteration, all the FeatureResultSet are read at the SAME ORDER. Therefore, the number
			of open and close operations of files are the minimum possible.


		Na pratica as leituras de arquivos sao tantas que nao compensa ter que sortear sempre os resultsets
	'''

	def __init__(self,prefix,list_size,source_generator):
		self.prefix = prefix	#Prefix used to configure a filename for a saved list
		self.item_list = []		#Current Item List
		self.file_list = []		#Filepaths
		self.source_generator = source_generator()	#The FeatureResultSetGenerator

		self.__initialize_indexes(list_size)
		self.__initialize_flags()

	def __initialize_indexes(self,list_size):
		self.__item_max_index = list_size

		self.__file_cur_index = 0
		self.__file_max_index = 0

	def __initialize_flags(self):
		self.__finish_writing = False


	def __read_file(self,filepath):		
		self.item_list = None
		with open(filepath,"rb") as input:
			self.item_list = pickle.load(input)

	def __write_file(self):
		filepath = "%s/%s_%d.pk" % (config.RESULTSET_PATH,self.prefix,self.__file_max_index)
		self.file_list.append(filepath)
		print "FLUSH!!"
		with open(filepath,"wb") as out:
			pickle.dump(self.item_list,out)

		self.item_list = []

		self.__file_max_index+=1

	def __write_last_file(self):
		self.__item_generator = self.__item_generator_fn()
		self.__finish_writing = True
		self.__write_file()

	def __get_next_source_item(self):

		for next_item in self.source_generator:
			self.item_list.append( next_item )
			if(len(self.item_list)==self.__item_max_index):
				self.__write_file()				

			yield next_item

		self.__write_last_file()

	def __item_generator_fn(self):
		for filepath in self.file_list:
			self.__read_file( filepath )	

			for item in self.item_list:
				yield item

	def __get_next_item(self):
		return self.__item_generator.next()

	def get_next_frs(self):
		if self.__finish_writing:
			for item in self.__item_generator_fn():
				yield item
		else:
			for item in self.__get_next_source_item():
				yield item

if __name__=="__main__":
	def fn():
		for i in range(0,20000000):
			yield (i*2.3,i*1.7,i*3.5)

	rm = ResultsetManager("pair_lista",200*10000,fn)
	for b in range(0,10):
		for i in rm.get_next_frs():
			pass
