#coding:utf-8

from distutils.core import setup, Extension

module = Extension('integral_image',
					sources=['integral_image.c'],
					libraries=['MagickWand-6.Q16']
				  )


setup(	name="IntegralImage",
		version="1.0",
		description="First Package",
		ext_modules=[module])
