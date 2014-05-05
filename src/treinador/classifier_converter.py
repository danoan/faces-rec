#coding:utf-8

import pyToc
import config
import os

def convert_everything(folder,newfolder):
	for fn in os.listdir(folder):
		print fn
		if os.path.isdir("%s/%s" % (folder,fn)):
			continue

		path = "%s/%s" % (folder,fn)
		newPath = "%s/%s" % (newfolder,fn)

		pyToc.convert_classifier(path,newPath)
		pyToc.compare_classifiers(path,newPath)		


if __name__=="__main__":
	folder = "%s" % (config.CLASSIFIERS_3500_PATH,)
	new_folder = "%s/%s" % (config.CLASSIFIERS_3500_PATH,"novos")

	# pyToc.convert_classifier( ("%s/%s" % (folder,"classifier_3500_(51-100).pk")),("%s/%s" % (folder,"classifier_3500_(51-100).cl")) )
	convert_everything(folder,new_folder)
	
