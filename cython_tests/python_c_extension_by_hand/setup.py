#coding:utf-8

from distutils.core import setup, Extension

module = Extension('time_test',sources=['time_test.c'])

setup(	name="IntegralImage",
		version="1.0",
		description="First Package",
		ext_modules=[module])
