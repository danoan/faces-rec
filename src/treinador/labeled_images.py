#coding:utf-8
import config

class LabeledImage():

	def __init__(self,img_filepath):
		self.__img_filepath = img_filepath
		self.__faces_boxes = []

	def faces_boxes(self):
		return self.__faces_boxes

	def image_filepath(self):
		return self.__img_filepath

	def label_face(self,box):
		self.__faces_boxes.append(box)

	def check_box(self,box):
		x1,y1,x2,y2=box[0],box[1],box[2],box[3]		
		for fb in self.__faces_boxes:	
			x1b,y1b,x2b,y2b = fb[0],fb[1],fb[2],fb[3]
			if x1>=x1b and y1>=y1b and x2<=x2b and y2<=y2b:
				return True

		return False

def li1():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"1.pgm"))
	li.label_face( (68,89,142,170) )
	li.label_face( (188,129,267,210) )
	li.label_face( (284,177,365,257) )
	li.label_face( (367,130,413,215) )

	return li