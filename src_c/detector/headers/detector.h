#ifndef __DETECTOR__
#define __DETECTOR__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "../../util/headers/basic.h"
#include "../../util/headers/image.h"
#include "../../classifier/headers/libclassifier.h"

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

    int detectFaces(ClassifierInterface* ci, const std::string img_path,int draw);
    int detectFaces(ClassifierInterface* ci, const std::string img_path,int draw, int save);
    int detectFaces(ClassifierInterface* ci, const std::string img_path, double ac,int draw);       
    int detectFaces(ClassifierInterface* ci, const std::string img_path, double ac, int draw, int save);
};

#endif