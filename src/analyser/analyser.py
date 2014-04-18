#coding:utf-8

import matplotlib.pyplot as plt
import random

class Analyser():
	def __init__(self,list_var):
		self.__data = {}
		self.__cur_item = {}
		self.__item_opened = False
		self.__list_var = list_var

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
	
	def __init__(self):
		Analyser.__init__(self,["shift_step","false_positive","false_negative","faces"])

	def display_results(self):
		plt.plot(	self.data()[self.list_var()["shift_step"]],
					self.data()[self.list_var()["false_positive"]])



if __name__=="__main__":
	ssa = ShiftStepAnalyser()

	x = random.sample( [x for x in range(0,100)],30 )
	x.sort()
	for i in x:
		ssa.open_item()
		ssa.put_value("shift_step",i)
		ssa.put_value("false_positive",random.randint(5,100))
		ssa.close_item()

	ssa.display_results()
	plt.show()


