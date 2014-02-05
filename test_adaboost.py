#!/usr/bin/python
#coding:utf-8

import unittest
from adaboost import *


class MaskTwoHorizontalFactoryTestCase(unittest.TestCase):
	def test_A_simple_mask(self):
		'''
			Minimum size of a mask of this type
		'''
		fac = MaskTwoHorizontalFactory((2,2))
		
		mask_to_match = np.array( [ [0,1], [0,1] ])
		size_to_match = (2,2)
		location_to_match = (0,0)

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
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

		feature_to_match = FeatureMask(mask_to_match,size_to_match,location_to_match)
		feature_returned = fac.next().next()[0]

		assert feature_returned==feature_to_match


def run_MaskDiagonalFactory_test_suite():
	suite = unittest.makeSuite(MaskDiagonalFactoryTestCase,'test')
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


if __name__=='__main__':
	main()

