#coding:utf-8

from trainning_tools import *
import Image

folder_faces = "/home/daniel/Desktop/lfwcrop_grey/faces"
fm = FeatureMaster((64,64),4,4,1.25,4,4)
img_filename = "%s/%s" % (folder_faces,"Aaron_Peirsol_0004.pgm")
img = Image.open(img_filename)
fm.add_image(img,FeatureMaster.FACE)