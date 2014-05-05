#coding:utf-8

from distutils.core import setup, Extension

path = "source"
s1 = "%s/%s" %(path,"pyToCClassifier.c")
s2 = "%s/%s" %(path,"c_loader.c")
s3 = "%s/%s" %(path,"pyApiBasic.c")

module_1 = Extension(	'pyToCClassifier',
					sources=[s1,s2,s3],
					libraries=[])

setup(	name="CConverter",
		version="1.0",
		description="Convertor de classificadores de Python para C",
		ext_modules=[module_1])