#coding:utf-8

from trainning_tools import *
from detector import *
from features import *
import os,pickle,random

# n=25

# folder_faces = "/home/daniel/Desktop/lfwcrop_grey/faces"
# folder_scenes = "/home/daniel/Desktop/training non-face images"
# folder_test_img = "/home/daniel/Desktop/test_images"

# classifier = None
# with open('classifier.pk','rb') as input:
# 	classifier = pickle.load(input)


# #Creating the Detector
# ng = 4
# ref_ardis = (64,64)
# ref_mask = (8,8)

# shift_step=8
# fn=SubwindowGenerator.DYNAMIC_FACTOR

# det = Detector(classifier,ng,ref_ardis,ref_mask,shift_step=shift_step,fn=fn)		

# def test_detector():
# 	img = Image.open("%s/%s" % (folder_test_img,"1.pgm"))
# 	sw = Subwindow(0,0,(64,64))
# 	det.is_face(img,sw)

# test_detector()


# TRIPLE_HORIZONTAL
mthf = MaskDiagonalFactory((6,8))
for fm,count in mthf.next():
	if count==90:
		print fm, "\n\n"
		fm.adjust_mask(Subwindow(50,50,(6,8),2.27))
		print fm


# img1 = Image.open("%s/%s" % (folder_test_img,"1.pgm"))
# img3 = Image.open("%s/%s" % (folder_test_img,"3.pgm"))

# ii1 = IntegralImage(img1)
# ii2 = IntegralImage(img2)
