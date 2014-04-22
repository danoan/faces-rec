#coding:utf-8
import config
import pickle,os

l = os.listdir(config.CLASSIFIERS_3500_PATH)
q
for cf in l:
	cf = "%s/%s" % (config.CLASSIFIERS_3500_PATH,cf)
	classifier=None
	with open(cf,"rb") as input:
		classifier = pickle.load(input)

	for h in classifier.hypothesis:			
		f = h[3]	#feature mask

		f.mask.c_args = []

		for block in f.mask.white:
			f.mask.c_args.extend( [block.points[0][0],block.points[0][1],
								  block.points[1][0],block.points[1][1],
								  block.points[2][0],block.points[2][1],
								  block.points[3][0],block.points[3][1],
								  block.w,block.h] )		

		for block in f.mask.black:
			f.mask.c_args.extend( [block.points[0][0],block.points[0][1],
								  block.points[1][0],block.points[1][1],
								  block.points[2][0],block.points[2][1],
								  block.points[3][0],block.points[3][1],
								  block.w,block.h] )				


	with open(cf,"wb") as output:
		pickle.dump(classifier,output,pickle.HIGHEST_PROTOCOL)	
