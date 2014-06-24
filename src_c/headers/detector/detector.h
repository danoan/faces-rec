#ifndef __DETECTOR__
#define __DETECTOR__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "../util/basic.h"
#include "../util/image.h"
#include "../classifier/libclassifier.h"

#include "subwindowGenerator.h"


class Detector{
private:
	int _ng;
	
	Point _wr;			//Smallest feature subwindow
	Point _ref_mask;	//Window size of the training set

	int _shift_step;

public:
	Detector(){};
	Detector(int ng, Point wr, Point ref_mask, int shift_step);

	void detectFaces(ClassifierInterface* ci, const std::string img_path);
	void detectFaces(ClassifierInterface* ci, const std::string img_path, double ac);	
};

#endif