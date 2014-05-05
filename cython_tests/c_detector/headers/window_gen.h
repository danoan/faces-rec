#ifndef __WINDOW_GEN
#define __WINDOW_GEN

#include <cstdio>
#include <cstdlib>
#include <string>
#include <math.h>

#include "image.h"
#include "basic.h"

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

#endif
