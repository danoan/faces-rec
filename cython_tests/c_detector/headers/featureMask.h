#ifndef __FEATURE_MASK
#define __FEATURE_MASK

#include <cstdio>
#include "basic.h"

class MaskBlock{
public:
	Point _points[4];
	ulong _w,_h;

	MaskBlock(Point a, Point b, Point c, Point d);

	inline Point a(){ return this->_points[0]; };
	inline Point b(){ return this->_points[1]; };
	inline Point c(){ return this->_points[2]; };
	inline Point d(){ return this->_points[3]; };

	inline ulong w(){ return this->_w; };
	inline void w(ulong w){this->_w=w;};

	inline ulong h(){ return this->_h; };
	inline void h(ulong h){this->_h=h;};

};

class Mask{
public:
	std::vector<MaskBlock> _white;
	std::vector<MaskBlock> _black;

	Point _size;
	Mask(* _createMask)(Point);

	Mask(){};
	Mask(Point size, int maskType);

	void addBlock(MaskBlock &mb, int blockType);
	inline void addWhite(MaskBlock &mb);
	inline void addBlack(MaskBlock &mb);
	Mask rescale(double ce, Point original_size);

	inline std::vector<MaskBlock> white(){ return this->_white; };
	inline std::vector<MaskBlock> black(){ return this->_black; };

	inline Point size(){return this->_size;};
};


/*
A FeatureMask is a rectangle composed by a list of white and black MaskBlock.
*/
class FeatureMask{
public:
	Mask _original_mask;
	Point _original_location;
	Point _original_size;

	Mask _mask;
	Point _location;

	FeatureMask(){};
	FeatureMask(Mask,Point);

	/*	
	When FeatureMask is created, its locations is based on the ardis used on the
	mask creation time (usually, 64x64). When detecting faces on an image, the ardis
	(now called subwindows) would be located in different coordinates, that's why we
	have to adjust it.
	*/
	void adjustMask(Subwindow &sw);
	inline Mask mask(){return _mask;};
	inline Point location(){return _location;};
};

Mask createMaskM2H(Point size);
Mask createMaskM2V(Point size);
Mask createMaskM3H(Point size);
Mask createMaskM3V(Point size);
Mask createMaskMD(Point size);

#endif