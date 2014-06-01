#coding:utf-8
import config,random,os,windowgen

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

def li_faces(n):
	face_filenames = random.sample(os.listdir(config.FACES_PATH),n)

	li_list = []
	for i in face_filenames:
		img_path = "%s/%s" % (config.FACES_PATH,i)
		li = LabeledImage(img_path)
		li.label_face((0,0,64,64))
		li_list.append(li)

	return li_list

def li_non_faces(n):
	li_list = []

	for scene_window in windowgen.get_next_random_image_window(config.SCENES_PATH,n):
		img_path = scene_window.window_img_path
		li = LabeledImage(img_path)
		li_list.append(li)

	return li_list

def li_faces_validation(n):
	face_filenames = os.listdir(config.FACES_PATH)[10000:10000+n]

	li_list = []
	for i in face_filenames:
		img_path = "%s/%s" % (config.FACES_PATH,i)
		li = LabeledImage(img_path)
		li.label_face((0,0,64,64))
		li_list.append(li)

	return li_list

def li_non_faces_validation(n):
	li_list = []

	for scene_window in windowgen.get_next_random_image_window(config.SCENES_PATH_VALIDATION,n):
		img_path = scene_window.window_img_path
		li = LabeledImage(img_path)
		li_list.append(li)

	return li_list	

def li1():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"seinfeld.pgm"))
	li.label_face( (68,89,142,170) )
	li.label_face( (188,129,267,210) )
	li.label_face( (284,177,365,257) )
	li.label_face( (367,130,453,215) )
	return li

def li2():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"seinfeld_384_288.pgm"))
	return li	

def li4():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"4.pgm"))
	return li	

def liK1():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"kramer1.pgm"))
	return li

def liK2():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"kramer2.pgm"))
	return li	

def liKramer():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"kramer.pgm"))
	return li		

def liGeorge():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"george.pgm"))
	return li		

def liElaine():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"elaine.pgm"))
	return li		

def liJerry():
	li = LabeledImage("%s/%s" % (config.TEST_IMG_PATH,"jerry.pgm"))
	return li				