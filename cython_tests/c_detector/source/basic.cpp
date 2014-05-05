#include "../headers/basic.h"

Subwindow::Subwindow(int x, int y, Point wr, double ce, int ng){
	_x = x;
	_y = y;
	
	_size = computeSubwindowSize(wr,ce);
	
	_cur_ng = ng;
	_ce = ce;
}

double fixed_factor(int ng, double ce0, double cef){
	double factor = ce0*pow(1.25,ng);
	if(factor>cef){
		factor=cef;
	}

	return factor;
};