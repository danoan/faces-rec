#coding:utf-8

import Image
import math,random,os,time

'''
	Given an image, this class computes all the possible image windows
	with dimensions window_size could exist in the image

	256x256 ss=70
	(0,0) (0,70) (0,140) (0,210)
	(24,70) (94,70) ...
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
		self.shift_step = shift_step

		self.window_size = window_size
		''' I open and close the file every time some image processing needs to be done,
			otherwise, too many files would be opened.
		'''
		self.img_w=0
		self.img_h=0

		img = Image.open(self.img_filename)
		print self.img_filename
		self.img_w = img.size[0] - self.shift_step
		self.img_h = img.size[1] - self.shift_step
		print self.img_w,self.img_h

		self.total_windows = (self.img_w*self.img_h)/(self.shift_step*self.shift_step)
		self.iter = self.get_next_random_window()
		print self.total_windows

	def get_next_window(self,i):
		print i
		i = i*self.shift_step
		x = i%self.img_w
		y = (i/self.img_w)*self.shift_step
		
		return (x,y)

	def get_next_random_window(self):
		index_to_sort = [i for i in xrange(0,self.total_windows) ]

		for n in xrange(0,self.total_windows):			
			windows_left = self.total_windows-n
			r = random.randint(0,windows_left-1)
			i = index_to_sort[r]

			index_to_sort = index_to_sort[:r]+index_to_sort[r+1:]

			w = self.get_next_window(i)
			crop_box = (w[0],w[1],w[0]+self.window_size[0],w[1]+self.window_size[1])

			img = Image.open(self.img_filename)
			crop_window = img.crop( crop_box )

			saved_name = "crop_%s.pgm" %(ImageWindowGenerator._get_next_count(),)

			# yield saved_name
			yield ImageWindow(self.name,self.folder,self.folder_to_save,crop_window,w,n,saved_name)

	def next(self):
		try:
			return self.iter.next()
		except Exception as e:
			print e
			return None

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
			os.makedirs(path)

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

	shift_step = 64
	img_windows = []

	windows_total = 0
	a=0
	for fn in file_names:		
		if fn=="crop":
			continue		
		iw = ImageWindowGenerator(fn,folder,folder_to_save,face_size,shift_step)
		img_windows.append( iw )	
		windows_total+=iw.total_windows

	print "TOTAL WINDOWS %d (%d)" % (windows_total,n)

	#windows_total = 223922
	if n > windows_total:
		# raise Exception("The number of windows asked for is greater than the available windows. Please increase the number os scene images or decrease the shift step value")
		print "The number of windows asked for is greater than the available windows. "\
			   "Please increase the number os scene images or decrease the shift step "\
			   "value. The maximum number (%d) will be used instead." % (windows_total,)
		n = windows_total

	for a in xrange(0,n):
		window = None
		while(window==None):
			if len(img_windows)>1:
				r = random.randint(0,len(img_windows)-1)		
				iw = img_windows[r]
			else:
				iw = img_windows[0]

			window = iw.next()
			if window==None: img_windows = img_windows[:r] + img_windows[r+1:]

		yield window



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
		factor = ce0*math.pow(1.25,ng)
		if factor>cef:
			factor=cef
		return factor

	@staticmethod
	def dynamic_factor(ng,tg,ce0,cef,wr):
		factor = (cef - ce0)/(tg*1.0)
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
		'''
			Each FeatureMask has a minimum size to work. For example. TwoHorizontalMask
			should be at least 2x2 and ThreeHorizontalMask should be at least 2x3.
			This discovers the minimum mask that accomplishes all the Features.
		'''
		min_mask = [0,0]
		
		for m in masks:
			if m.MIN_MASK[0] > min_mask[0]:
				min_mask[0] = m.MIN_MASK[0]
			if m.MIN_MASK[1] > min_mask[1]:
				min_mask[1] = m.MIN_MASK[1]

		return (min_mask[0],min_mask[1])

	def __initial_scale_coef(self,ref_ardis,ref_mask,min_mask):
		'''
			All the features compute windows starting at 8x8 inside a 64x64. However, the subwindows
			generated for a random image, could generate windows of 4x4, for example, because it is 
			using the minimum_mask value. A better approach would be to choose the minimum generated
			subwindow based on the size of the image, lets say 10%. For example, a image of 640x480 
			starts detecting subwindows that have, 64x48
		'''
		# mask_coef = math.floor( 	max( ref_mask[0]/min_mask[0],
		# 		  				    	 ref_mask[1]/min_mask[1] )
		# 		  			  )

		# return 1.0/mask_coef
				
		mask_coef = min( (self.img_size[0]*0.2)/self.wr[0],
  				    	 (self.img_size[1]*0.2)/self.wr[1] )

		#I can't have mask_coef<0.5, because this implies masks littler than tha minimum
		return max(mask_coef,0.5)

	def __max_scale_coef(self):
		img_coef = min(	self.img_size[0]/self.wr[0], 
	  			  		self.img_size[1]/self.wr[1] )

		return img_coef

	def generate_subwindows(self,ng,fn):
		cur_ng = 0
		ab = 0
		subwindows = []
		while cur_ng<ng:
			ce = fn(cur_ng,ng,self.ce0,self.ce_max,self.wr)
			# print "SUBW ",ce

			subwindow_size = Subwindow.size(self.wr,ce)
			if subwindow_size[0] > self.img_size[0] or subwindow_size[1] > self.img_size[1]:
				break
			
			max_y_pos = self.img_size[1] - subwindow_size[1] +1
			max_x_pos = self.img_size[0] - subwindow_size[1] +1

			for y in range(0,max_y_pos,self.shift_step):
				for x in range(0,max_x_pos,self.shift_step):
					subwindows.append( Subwindow(x,y,self.wr,ce,cur_ng) )
					# print "COORD: ",x,y

			# print ce
			if ce==self.ce_max:
				break

			cur_ng+=1

		# print len(subwindows),self.ce0, self.ce_max

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