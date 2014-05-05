#coding:utf-8

from distutils.core import setup, Extension

module_1 = Extension(	'c_converter',
					sources=['c_converter.c'],
					libraries=[])

module_2 = Extension('c_loader',
					sources=['c_loader.c'],
					libraries=[])

setup(	name="CConverter",
		version="1.0",
		description="Convertor de classificadores de Python para C",
		ext_modules=[module_1,module_2])