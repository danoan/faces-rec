#coding:utf-8

import pickle,os
import pyToCClassifier as cc

def test_converter():
	cc.init( True, [23,24], 2)

	cc.init_hypothesis( 100,200,300 )
	cc.init_feature( (400,500) )
	cc.init_mask( "MaskTwoHorizontalFactory", (600,700),2,2 )
	cc.add_block("white",[1,1,2,2,3,3,4,4,10,20])
	cc.add_block("white",[5,5,6,6,7,7,8,8,30,40])
	cc.add_block("black",[9,9,10,10,11,11,12,12,50,60])
	cc.add_block("black",[13,13,14,14,15,15,16,16,70,80])
	cc.close_mask()
	cc.close_feature();
	cc.close_hypothesis();	

	cc.init_hypothesis( 100,200,300 )
	cc.init_feature( (400,500) )
	cc.init_mask( "MaskTwoHorizontalFactory", (600,700),2,2 )
	cc.add_block("white",[1,1,2,2,3,3,4,4,10,20])
	cc.add_block("white",[5,5,6,6,7,7,8,8,30,40])
	cc.add_block("black",[9,9,10,10,11,11,12,12,50,60])
	cc.add_block("black",[13,13,14,14,15,15,16,16,70,80])
	cc.close_mask()
	cc.close_feature();
	cc.close_hypothesis();	

	cc.close_classifier("/home/daniel/Projects/faces-rec/src_c/pyToC/3500_teste_cl");	

def add_block(block_type,bw):
	args = []
	for p in bw.points:
		args.append(p[0])
		args.append(p[1])

	args.append(bw.w)
	args.append(bw.h)

	cc.add_block(block_type,args)	

def convert_classifier(filenamePy,filenameC):
	py_classifier = None
	with open(filenamePy,"rb") as input:
		py_classifier = pickle.load(input)


	cc.init( py_classifier.final, py_classifier.ardis, len(py_classifier.hypothesis) )
	for h in py_classifier.hypothesis:		

		cc.init_hypothesis( int( h[0] ), h[1], h[4] )
		fm = h[3]

		cc.init_feature( fm.location )
		cc.init_mask( fm.mask.mf.__class__.__name__, fm.mask.size, len(fm.mask.white), len(fm.mask.black) )

		for bw in fm.mask.white:
			add_block("white",bw)

		for bb in fm.mask.black:
			add_block("black",bb)

		cc.close_mask()
		cc.close_feature()
		cc.close_hypothesis()

	cc.close_classifier(filenameC)

def compare_classifiers(filenamePy,filenameC):
	cc.load_classifier(filenameC)
	py_classifier = None
	with open(filenamePy,"rb") as input:
		py_classifier = pickle.load(input)

	assert cc.get_attribute("FINAL")==(1 if py_classifier.final else 0) 
	assert cc.get_attribute("ARDIS")[0]==py_classifier.ardis[0]
	assert cc.get_attribute("ARDIS")[1]==py_classifier.ardis[1]

	for i in range(0,len(py_classifier.hypothesis)):
		h = py_classifier.hypothesis[i]
		fm = h[3]

		assert cc.get_attribute("THRESHOLD",i)==h[0]
		assert cc.get_attribute("DIRECTION",i)==h[1]
		assert cc.get_attribute("ALPHA",i)==h[4]

		assert cc.get_attribute("ORIGINAL_LOCATION",i)[0]==fm.location[0]
		assert cc.get_attribute("ORIGINAL_LOCATION",i)[1]==fm.location[1]

		assert cc.get_attribute("ORIGINAL_SIZE",i)[0]==fm.size[0]
		assert cc.get_attribute("ORIGINAL_SIZE",i)[1]==fm.size[1]

		assert cc.get_attribute("LOCATION",i)[0]==fm.location[0]
		assert cc.get_attribute("LOCATION",i)[1]==fm.location[1]

		for bw in range(0,len(fm.mask.white)):			
			points = cc.get_attribute("WHITE_POINTS",i,bw)			

			for k in range(0,4):				
				assert points[k*2]==fm.mask.white[bw].points[k][0]
				assert points[k*2+1]==fm.mask.white[bw].points[k][1]

			assert cc.get_attribute("WHITE_W",i,bw)==fm.mask.white[bw].w
			assert cc.get_attribute("WHITE_H",i,bw)==fm.mask.white[bw].h


		for bb in range(0,len(fm.mask.black)):			
			points = cc.get_attribute("BLACK_POINTS",i,bb)			

			for k in range(0,4):				
				assert points[k*2]==fm.mask.black[bb].points[k][0]
				assert points[k*2+1]==fm.mask.black[bb].points[k][1]

			assert cc.get_attribute("BLACK_W",i,bb)==fm.mask.black[bb].w
			assert cc.get_attribute("BLACK_H",i,bb)==fm.mask.black[bb].h			


		assert cc.get_attribute("SIZE",i)[0]==fm.mask.size[0]
		assert cc.get_attribute("SIZE",i)[1]==fm.mask.size[1]


if __name__=='__main__':	
	filenamePy = "/home/daniel/Projects/faces-rec/classifiers/3500/classifier_3500_(3-100).pk"
	filenameC = "/home/daniel/Projects/faces-rec/src_c/pyToC/3500_3_100_cl"

	convert_classifier(filenamePy,filenameC)
	test_converter()

	cc.print_classifier( cc.load_classifier(filenameC) )
	compare_classifiers(filenamePy,filenameC)
