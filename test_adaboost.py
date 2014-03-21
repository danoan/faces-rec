#!/usr/bin/python
#coding:utf-8

import unittest
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match	



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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match			


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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match						


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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match				

class MaskDiagonalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimun size of a mask of this type
		'''
		fac = MaskDiagonalFactory((2,2))

		mask_to_match = np.array( [ [0,1],[1,0] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
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

		feature_to_match = FeatureMask(None,size_to_match,location_to_match,mask_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match


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

		fm = FeatureMaster((4,4),1,1,1,2,2)
		fm.add_image(im,True)
		for i in fm.get_next_frs():
			pass

		assert fm.m2h_values[0] == 20
		assert fm.m2v_values[0] == 100
		assert fm.m3h_values[0] == 90
		assert fm.m3v_values[0] == 110
		assert fm.md_values[0] == 0

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

