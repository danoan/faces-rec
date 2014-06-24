#ifndef __SUBWINDOW_GENERATOR__
#define __SUBWINDOW_GENERATOR__

#include <cstdio>
#include <cstdlib>
#include <string>
#include <math.h>
#include <vector>

#include "../util/image.h"
#include "../util/basic.h"

class SubwindowGenerator{
public:
	std::string _img_path;
	
	Point _img_size;
	Point _wr;

	ulong _shift_step;
	double _ce0;
	double _ce_max;

	SubwindowGenerator(){};
	SubwindowGenerator(std::string img_path,Point wr,ulong shift_step);

	std::vector<Subwindow> generateSubwindows(int ng);
};

double fixed_factor(int ng, double ce0, double cef);

#endif
