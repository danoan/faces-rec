#ifndef __DETECTOR
#define __DETECTOR

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "basic.h"
#include "window_gen.h"
#include "integral.h"
#include "classifier.h"

class Detector{
private:
	Classifier _cl;
	int _ng;
	
	Point _wr;
	Point _ref_mask;

	int _shift_step;
	double _ac;

public:
	Detector(){};
	Detector(Classifier cl, int ng, Point wr, Point ref_mask, int shift_step, double ac);

	void detectFaces(std::string img_path);	
};

#endif