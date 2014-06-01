#coding:utf-8

import os
import fractions as fr
import Image
import ImageFont,ImageDraw


#Converts the image data to a row vector
def img_to_rvector(img):
	return list(img.getdata())

#Given a folder, turn all the images on grayscale mode
def normalize_images(folder,n):
	k = 1
	while k<=n:
		Image.open("%s/img_%d.jpg" % (folder,k)).convert('L').save("%s/img_%d.jpg" % (folder,k))
		k+=1

'''Given a folder, the files receive a numeric name, and converts 
to black and white .pgm file format'''
def normalize_scene_images(folder):
	#Rename
	file_names = os.listdir(folder)
	count = 0
	for fn in file_names:
		extension = fn.split(".")[-1]

		path = "%s/%s" % (folder,fn)
		new_path = "%s/%d.%s" %(folder,count,extension)

		f = open(path)
		g = open(new_path,'w')
		g.writelines(f.readlines())
		g.close()
		f.close()

		os.remove(path)
		count+=1


'''
	As input, receives the experiments results, and draws test_image |
	recognized_image | distance and saves it as a PNG file called saida.png
'''
def draw_results(rtcl):
	im_h = 92
	im_w = 112
	txt_w = 135
	col_gap = 20
	row_gap = 50

	n = len(rtcl)
	width = (2*im_w+4*col_gap+txt_w)
	height = n*(im_h+row_gap)+row_gap

	new_im = Image.new('RGB',(width,height),(255,255,255))

	font = ImageFont.truetype("/usr/share/fonts/truetype/freefont/FreeSans.ttf",22)
	cur_h = row_gap
	cur_w = col_gap
	for r in rtcl:

		im_1 = Image.open(r["test_image"])
		im_2 = Image.open(r["match_image"])
		
		new_im.paste(im_1,(cur_w,cur_h))
		cur_w+=im_w+col_gap

		new_im.paste(im_2,(cur_w,cur_h))
		cur_w+=im_w+col_gap

		draw = ImageDraw.Draw(new_im)
		draw.text((cur_w,cur_h+45),"%.2f" % ( r["distance"],),(0,0,0),font=font)

		cur_w = col_gap
		cur_h+=im_h+row_gap

	new_im.save("saida.png")