#coding:utf-8

import Image
import math,random,os,time

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
		self.original_name = original_name
		self.saved_name = saved_name
		self.folder = folder
		self.location = location
		self.seq_number = seq_number

		#Need it to keep state
		self.save(window_img,folder_to_save)
		

	def save(self,window_img,folder_to_save):
		path = "%s/crop/%s" % (self.folder,folder_to_save)
		if not os.path.exists(path):
			os.mkdir(path)

		crop_filename = "%s/%s" % (path,self.saved_name,)
		window_img.save(crop_filename)
		self.window_img_path = crop_filename

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


'''
	Given an image, it will generate all the subwindows in that image that
	satisfies the parameters

	ref_ardis = Window used to generate the masks (64,64)
	ref_mask =  Size of the littlest mask used in the hypohtesis taking as a
				reference the ref_ardis window
	masks 	 =  Feature classes used to create the Final Classifier
	img 	 =  Image whose subwindow will be generated
'''
class SubwindowGenerator():

	@staticmethod
	def fixed_factor(ng,tg,ce0,cef,wr):
		factor = 1.25
		if factor>cef:
			factor=cef
		return ce0*math.pow(1.25,ng)

	@staticmethod
	def dynamic_factor(ng,tg,ce0,cef,wr):
		factor = (cef - ce0)/(tg*1.0)
		print ce0 + factor*ng
		return ce0 + factor*ng		

	FIXED_FACTOR = fixed_factor
	DYNAMIC_FACTOR = dynamic_factor

	def __init__(self,ref_ardis,ref_mask,masks,img_path,shift_step=1):		
		self.img_path = img_path
		self.img_size = Image.open(img_path).size

		self.wr = ref_ardis				
		self.shift_step = shift_step		

		min_mask = self.__discover_min_mask(masks)
		self.ce0 = self.__initial_scale_coef(ref_ardis,ref_mask,min_mask)
		self.ce_max = self.__max_scale_coef()
		

	def __discover_min_mask(self,masks):
		min_mask = [0,0]
		
		for m in masks:
			if m.MIN_MASK[0] > min_mask[0]:
				min_mask[0] = m.MIN_MASK[0]
			if m.MIN_MASK[1] > min_mask[1]:
				min_mask[1] = m.MIN_MASK[1]

		return (min_mask[0],min_mask[1])

	def __initial_scale_coef(self,ref_ardis,ref_mask,min_mask):
		mask_coef = math.floor( 	max( ref_mask[0]/min_mask[0],
				  				    		  ref_mask[1]/min_mask[1] )
				  			  )
		return 1.0/mask_coef

	def __max_scale_coef(self):
		img_coef = math.floor(	min( self.img_size[0]/self.wr[0], 
							  			  self.img_size[1]/self.wr[1] )
						     )
		return img_coef

	def generate_subwindows(self,ng,fn):
		cur_ng = 0
		ab = 0
		subwindows = []
		while cur_ng<ng:
			ce = fn(cur_ng,ng,self.ce0,self.ce_max,self.wr)

			subwindow_size = Subwindow.size(self.wr,ce)

			if subwindow_size[0] > self.img_size[0] or subwindow_size[1] > self.img_size[1]:
				break
			
			max_y_pos = self.img_size[1] - subwindow_size[1]
			max_x_pos = self.img_size[0] - subwindow_size[1]

			# print max_x_pos,max_y_pos

			for y in range(0,max_y_pos,self.shift_step):
				for x in range(0,max_x_pos,self.shift_step):
					subwindows.append( Subwindow(x,y,self.wr,ce,cur_ng) )
					# print "COORD: ",x,y

			cur_ng+=1

		return subwindows

class Subwindow():

	def __init__(self,x,y,wr=(64,64),ce=1.0,cur_ng=0):
		self.x = x
		self.y = y
		
		self.size = Subwindow.size(wr,ce)

		self.cur_ng = cur_ng
		self.ce = ce

	@staticmethod
	def size(wr=(64,64),ce=1.0):
		return ( int( round(wr[1]*ce) ),
			     int( round(wr[0]*ce) ) )		

	def crop_box(self):
		return (self.x,self.y,self.x+self.size[0],self.y+self.size[1])