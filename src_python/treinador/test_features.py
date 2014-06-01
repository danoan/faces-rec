#!/usr/bin/python
#coding:utf-8

import unittest
import windowgen,config,misc
from trainning_tools import *


class MaskTwoHorizontalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimum size of a mask of this type
		'''
		fac = MaskTwoHorizontalFactory((2,2))
		
		mask_to_match = np.array( [ [0,1], [0,1] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match

	def test_B_extra_mask(self):
		'''
			Testing what happens if an ardis with odd width is given.
		'''
		fac = MaskTwoHorizontalFactory((2,3))
		
		mask_to_match = np.array( [ [0,1], [0,1] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match	

	def test_C_adjust_mask(self):
		'''
			Initial Mask on line 2 and column 4 of the Ardis(64,64)
			01
			01

			Adjust Mask with a Subwindow located in x=13(column) and y=16(line) in a double ardis (128,128)
			0011
			0011
			0011
			0011

			The result should be this 4x4 mask located at line 2*2+16=20 and column 2*4+13=21
		'''
		fmask_gen = MaskTwoHorizontalFactory((2,2)).next().next()[0]
		fmask_gen = FeatureMask(fmask_gen.mask,(2,4))
		
		sw = windowgen.Subwindow(13,16,ce=2.0)
		fmask_gen.adjust_mask(sw)

		mask_to_match = np.array( [ [0,0,1,1],[0,0,1,1],[0,0,1,1],[0,0,1,1] ] )
		size_to_match = (4,4)
		location_to_match = (20,21)

		fmask_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		assert fmask_gen==fmask_match



def run_MaskTwoHorizontalFactory_test_suite():
	suite = unittest.makeSuite(MaskTwoHorizontalFactoryTestCase,'test')
	runner = unittest.TextTestRunner()
	runner.run(suite)


class MaskTwoVerticalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimum size of a mask of this type
		'''		
		fac = MaskTwoVerticalFactory((2,2))
		
		mask_to_match = np.array( [ [0,0], [1,1] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match	

	def test_B_extra_mask(self):
		'''
			Testing what happens if an ardis with odd height is given.
		'''
		fac = MaskTwoVerticalFactory((3,2))
		
		mask_to_match = np.array( [ [0,0], [1,1] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match			

	def test_C_adjust_mask(self):
		'''
			Initial Mask on line 2 and column 4 of the Ardis(64,64)
			00
			11

			Adjust Mask with a Subwindow located in x=13(column) and y=16(line) in a double ardis (128,128)
			0000
			0000
			1111
			1111

			The result should be this 4x4 mask located at line 2*2+16=20 and column 2*4+13=21
		'''
		fmask_gen = MaskTwoVerticalFactory((2,2)).next().next()[0]
		fmask_gen = FeatureMask(fmask_gen.mask,(2,4))
		
		sw = windowgen.Subwindow(13,16,ce=2.0)
		fmask_gen.adjust_mask(sw)

		mask_to_match = np.array( [ [0,0,0,0],[0,0,0,0],[1,1,1,1],[1,1,1,1] ] )
		size_to_match = (4,4)
		location_to_match = (20,21)

		fmask_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		assert fmask_gen==fmask_match		


def run_MaskTwoVerticalFactory_test_suite():
	suite = unittest.makeSuite(MaskTwoVerticalFactoryTestCase,'test')
	runner = unittest.TextTestRunner()
	runner.run(suite)		


class MaskThreeHorizontalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimum size of a mask of this type
		'''				
		fac = MaskThreeHorizontalFactory((2,3))
		
		mask_to_match = np.array( [ [1,0,1], [1,0,1] ])
		size_to_match = (2,3)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match

	def test_B_extra_mask(self):
		'''
			Testing what happens if an ardis with even width is given.
		'''
		fac = MaskThreeHorizontalFactory((2,4))
		
		mask_to_match = np.array( [ [1,0,1], [1,0,1] ])
		size_to_match = (2,3)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match	

	def test_C_adjust_mask(self):
		'''
			Initial Mask on line 2 and column 4 of the Ardis(64,64)
			101
			101

			Adjust Mask with a Subwindow located in x=13(column) and y=16(line) in a double ardis (128,128)
			110011
			110011
			110011
			110011

			The result should be this 4x6 mask located at line 2*2+16=20 and column 2*4+13=21
		'''
		fmask_gen = MaskThreeHorizontalFactory((2,3)).next().next()[0]
		fmask_gen = FeatureMask(fmask_gen.mask,(2,4))
		
		sw = windowgen.Subwindow(13,16,ce=2.0)
		fmask_gen.adjust_mask(sw)

		mask_to_match = np.array( [ [1,1,0,0,1,1],[1,1,0,0,1,1],[1,1,0,0,1,1],[1,1,0,0,1,1] ] )
		size_to_match = (4,6)
		location_to_match = (20,21)

		fmask_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		assert fmask_gen==fmask_match									


def run_MaskThreeHorizontalFactory_test_suite():
	suite = unittest.makeSuite(MaskThreeHorizontalFactoryTestCase,'test')
	runner = unittest.TextTestRunner()
	runner.run(suite)		



class MaskThreeVerticalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimum size of a mask of this type
		'''				
		fac = MaskThreeVerticalFactory((3,2))
		
		mask_to_match = np.array( [ [1,1], [0,0], [1,1] ])
		size_to_match = (3,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match

	def test_B_extra_mask(self):
		'''
			Testing what happens if an ardis with even height is given.
		'''
		fac = MaskThreeVerticalFactory((4,2))
		
		mask_to_match = np.array( [ [1,1], [0,0], [1,1] ])
		size_to_match = (3,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match			

	def test_C_adjust_mask(self):
		'''
			Initial Mask on line 2 and column 4 of the Ardis(64,64)
			11
			00
			11

			Adjust Mask with a Subwindow located in x=13(column) and y=16(line) in a double ardis (128,128)
			1111
			1111
			0000
			0000
			1111
			1111

			The result should be this 6x4 mask located at line 2*2+16=20 and column 2*4+13=21
		'''
		fmask_gen = MaskThreeHorizontalFactory((3,2)).next().next()[0]
		fmask_gen = FeatureMask(fmask_gen.mask,(2,4))
		
		sw = windowgen.Subwindow(13,16,ce=2.0)
		fmask_gen.adjust_mask(sw)

		mask_to_match = np.array( [ [1,1,1,1],[1,1,1,1],[0,0,0,0],[0,0,0,0],[1,1,1,1],[1,1,1,1] ] )
		size_to_match = (6,4)
		location_to_match = (20,21)

		fmask_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		assert fmask_gen==fmask_match				

class MaskDiagonalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimun size of a mask of this type
		'''
		fac = MaskDiagonalFactory((2,2))

		mask_to_match = np.array( [ [0,1],[1,0] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match

	def test_B_extra_mask(self):
		'''
			Testing what happens if an ardis with different width and height is given.
		'''				
		fac = MaskDiagonalFactory((2,4))

		mask_to_match = np.array( [ [0,1],[1,0] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match

	def test_C_adjust_mask(self):
		'''
			Initial Mask on line 2 and column 4 of the Ardis(64,64)
			01
			10

			Adjust Mask with a Subwindow located in x=13(column) and y=16(line) in a double ardis (128,128)
			0011
			0011
			1100
			1100

			The result should be this 2x4 mask located at line 2*2+16=20 and column 2*4+13=21
		'''
		fmask_gen = MaskDiagonalFactory((2,2)).next().next()[0]
		fmask_gen = FeatureMask(fmask_gen.mask,(2,4))
		
		sw = windowgen.Subwindow(13,16,ce=2.0)
		fmask_gen.adjust_mask(sw)

		mask_to_match = np.array( [ [0,0,1,1],[0,0,1,1],[1,1,0,0],[1,1,0,0] ] )
		size_to_match = (4,4)
		location_to_match = (20,21)

		fmask_match = FeatureMask(Mask(size_to_match,None),location_to_match,mask_to_match)
		assert fmask_gen==fmask_match			


def run_MaskDiagonalFactory_test_suite():
	suite = unittest.makeSuite(MaskDiagonalFactoryTestCase,'test')
	runner = unittest.TextTestRunner()
	runner.run(suite)	


class FeatureMasterTestCase(unittest.TestCase):
	def test_A_simple_image(self):
		im = Image.new("L",(4,4))
		im.putdata([10,20,30,50,
					60,70,80,90,
					100,110,120,130,
					140,150,160,170])

		filepath = "%s/%s" % (config.TEST_IMG_PATH,"featmastertestcase.pgm")
		with open( filepath, "wb" ) as output:
			im.save( output )

		fm = FeatureMaster((4,4),1,1,1,2,2,test=True)
		fm.add_image(filepath,True)
		for i in fm.get_next_frs():
			pass

		assert fm.m2h_values[0] == 20
		assert fm.m2v_values[0] == 100
		assert fm.m3h_values[0] == 90
		assert fm.m3v_values[0] == 110
		assert fm.md_values[0] == 0

	def test_B_ii_consistency(self):
		k1 = "%s/%s" %(config.TEST_IMG_PATH,"kramer1.pgm")
		k2 = "%s/%s" %(config.TEST_IMG_PATH,"kramer2.pgm")

		ii_k1 = misc.IntegralImage(k1)
		ii_k2 = misc.IntegralImage(k2)

		fmask_k1 = MaskTwoHorizontalFactory( (16,16) ).next().next()[0]
		fmask_k1 = FeatureMask(fmask_k1.mask,(0,0))

		fmask_k2 = MaskTwoHorizontalFactory( (16,16) ).next().next()[0]
		fmask_k2 = FeatureMask(fmask_k2.mask,(0,0))		

		sw_k1 = windowgen.Subwindow( 80,94 )
		sw_k2 = windowgen.Subwindow( 0,0 )

		fmask_k1.adjust_mask(sw_k1)
		fmask_k2.adjust_mask(sw_k2)

		assert ii_k1.filter(fmask_k1)==ii_k2.filter(fmask_k2)



def run_FeatureMaster_test_suite():
	suite = unittest.makeSuite(FeatureMasterTestCase,'test')
	runner = unittest.TextTestRunner()
	runner.run(suite)

def main():
	print "MaskTwoHorizontal Test"
	run_MaskTwoHorizontalFactory_test_suite()

	print "MaskTwoVertical Test"
	run_MaskTwoVerticalFactory_test_suite()

	print "MaskThreeHorizontalFactory Test"
	run_MaskThreeHorizontalFactory_test_suite()

	print "MaskThreeVerticalFactory Test"
	run_MaskThreeHorizontalFactory_test_suite()

	print "MaskDiagonalFactory Test"
	run_MaskDiagonalFactory_test_suite()

	print "FeatureMaster Test"
	run_FeatureMaster_test_suite()	


if __name__=='__main__':
	main()

