#coding:utf-8

import matplotlib.pyplot as plt
import random,os
import config

class Analyser():
	def __init__(self,list_var,title="Title",graph_filepath=None):
		self.__data = {}
		self.__cur_item = {}
		self.__item_opened = False
		self.__list_var = list_var

		self.graph_filepath = graph_filepath
		fig,self.plt = plt.subplots()
		plt.title(title)

	def plot_data(self,x,y):

		def __comp_sort__(x,y):
			if x[0]<y[0]:
				return -1
			elif x[0]==y[0]:
				return 0
			else:
				return 1		

		data = self.data()

		pairs = [ (data[x][i],data[y][i]) for i in range(0,len(data[x]) )]
		pairs.sort(__comp_sort__)

		self.plt.plot( 	[p[0] for p in pairs], 	
						[p[1] for p in pairs],
						label=y )		
		self.plt.legend()

	def put_value(self,name_var,value):
		if not self.__item_opened:
			raise Exception("Current item is closed")

		if name_var not in self.__list_var:
			raise Exception("This variable is not in the original list")

		self.__cur_item.update({name_var:value})			

	def open_item(self):
		if self.__item_opened:
			raise Exception("Item is already opened")		
		self.__item_opened = True
		self.__cur_item = {}

	def close_item(self):
		if not self.__item_opened:
			raise Exception("Item is already closed")
		self.__item_opened = False

		for n in self.__cur_item.keys():
			if self.__data.has_key(n):
				self.__data[n].append(self.__cur_item[n])
			else:
				self.__data.update({n:[self.__cur_item[n]]})

	def data(self):
		return self.__data

	def list_var(self):
		return self.__list_var

	def display_results(self):
		pass

class ShiftStepAnalyser(Analyser):
	
	def __init__(self,title="ShiftStep Analyser",graph_filepath=None):
		Analyser.__init__(self,["shift_step","false_positive","false_negative","faces"],title,graph_name)

	def display_results(self):
		self.plot_data("shift_step","false_positive")
		self.plot_data("shift_step","false_negative")
		self.plot_data("shift_step","faces")

		if self.graph_filepath:
			plt.savefig( self.graph_filepath )

		plt.show()


class AcceptanceCoefficientAnalyser(Analyser):
	
	def __init__(self,title="AcceptanceAnalyser",graph_filepath=None):
		Analyser.__init__(self,["ac","false_positive","false_negative","faces"],title,graph_filepath)

	def display_results(self):

		self.plot_data("ac","faces")
		self.plot_data("ac","false_positive")
		self.plot_data("ac","false_negative")

		if self.graph_filepath:
			dirs = "/".join(self.graph_filepath.split("/")[:-1])
			if not os.path.exists(dirs):				
				os.makedirs(dirs)

			plt.savefig( self.graph_filepath )
			# plt.show()
			plt.close()		


class FeatureErrorAnalyser(Analyser):
	
	def __init__(self,title="FeatureErrorAnalyser",graph_filepath=None):
		Analyser.__init__(self,["feature","ac"],title,graph_filepath)

	def display_results(self):

		self.plot_data("feature","ac")

		if self.graph_filepath:
			dirs = "/".join(self.graph_filepath.split("/")[:-1])
			if not os.path.exists(dirs):				
				os.makedirs(dirs)

			plt.savefig( self.graph_filepath )
			# plt.show()
			plt.close()					


if __name__=="__main__":
	ssa = ShiftStepAnalyser()

	x = random.sample( [x for x in range(0,100)],30 )
	x.sort()
	for i in x:
		ssa.open_item()
		ssa.put_value("shift_step",i)
		ssa.put_value("false_positive",random.randint(5,100))
		ssa.put_value("false_negative",random.randint(5,100))
		ssa.put_value("faces",random.randint(5,100))
		ssa.close_item()

	ssa.display_results()



