#include "../headers/featureMask.h"

MaskBlock::MaskBlock(Point pa, Point pb, Point pc, Point pd){
	this->_points[0] = pa;
	this->_points[1] = pb;
	this->_points[2] = pc;
	this->_points[3] = pd;

	this->_w = pb.x - pa.x + 1;
	this->_h = pc.y - pc.y + 1;
};

Mask::Mask(Point size, int maskType){
	this->_size = size;
	switch(maskType){
		case 0:
			this->_createMask = &(createMaskM2H);
			break;
		case 1:
			this->_createMask = &(createMaskM2V);
			break;
		case 2:
			this->_createMask = &(createMaskM3H);
			break;
		case 3:
			this->_createMask = &(createMaskM3V);
			break;
		case 4:
			this->_createMask = &(createMaskMD);
			break;
	}
}


void Mask::addBlock(MaskBlock &mb, int blockType){
	if(blockType==0){
		this->addWhite(mb);
	}else{
		this->addBlack(mb);
	}
}

inline void Mask::addWhite(MaskBlock &mb){
	this->_white.push_back(mb);
}

inline void Mask::addBlack(MaskBlock &mb){
	this->_black.push_back(mb);
}

Mask Mask::rescale(double ce, Point original_size){

	this->_size.x = (int) round( original_size.x*ce );
	this->_size.y = (int) round( original_size.y*ce );

	return (*this->_createMask)(this->_size);
}

FeatureMask::FeatureMask(Mask mask,Point location){
	this->_original_mask = mask;
	this->_original_location = location;
	this->_original_size = mask._size;

	this->_mask = mask;
	this->_location = location;
}

void FeatureMask::adjustMask(Subwindow &sw){
	// Mask's Subwindow Rescale		
	ulong maskLocation[2] = { (int) floor( this->_original_location.y*sw._ce ),
			 				  (int) floor( this->_original_location.x*sw._ce )
			 				};

	// Subwindow Location
	this->_location.y =	maskLocation[0]+sw._y;
	this->_location.x = maskLocation[1]+sw._x;
						
	
	// Mask Rescale
	this->_mask = this->_mask.rescale(sw._ce,this->_original_size);

	// printf("%.5f (%lu, %lu) (%lu, %lu)\n",sw._ce,_location.y,_location.x,_mask._size.y,_mask._size.x);
}

Mask createMaskM2H(Point size){
	ulong h = size.y;
	ulong w = size.x;

	int middle = (int) floor(w/2);

	Point white_a,white_b,white_c,white_d;

	white_a.y = 0;
	white_a.x = middle;

	white_b.y = 0;
	white_b.x = w-1;

	white_c.y = h-1;
	white_c.x = middle;

	white_d.y = h-1;
	white_d.x = w-1;


	Point black_a,black_b,black_c,black_d;

	black_a.y = 0;
	black_a.x = 0;

	black_b.y = 0;
	black_b.x = middle-1;

	black_c.y = h-1;
	black_c.x = 0;

	black_d.y = h-1;
	black_d.x = middle-1;


	MaskBlock white = MaskBlock(white_a, white_b, white_c, white_d);
	MaskBlock black = MaskBlock(black_a, black_b, black_c, black_d);

	Mask m = Mask(size, 0 );
	m.addWhite(white);
	m.addBlack(black);

	return m;
}

Mask createMaskM2V(Point size){
	ulong h = size.y;
	ulong w = size.x;

	int middle = (int) floor(h/2);

	Point white_a,white_b,white_c,white_d;

	white_a.y = middle;
	white_a.x = 0;

	white_b.y = middle;
	white_b.x = w-1;

	white_c.y = h-1;
	white_c.x = 0;

	white_d.y = h-1;
	white_d.x = w-1;

	Point black_a,black_b,black_c,black_d;

	black_a.y = 0;
	black_a.x = 0;

	black_b.y = 0;
	black_b.x = w-1;

	black_c.y = middle-1;
	black_c.x = 0;

	black_d.y = middle-1;
	black_d.x = w-1;

	MaskBlock white = MaskBlock(white_a,white_b,white_c,white_d);
	MaskBlock black = MaskBlock(black_a,black_b,black_c,black_d);

	Mask m = Mask(size,1);
	m.addWhite(white);
	m.addBlack(black);

	return m;
}


Mask createMaskM3H(Point size){
	ulong h = size.y;
	ulong w = size.x;

	ulong third = (int) floor(w/3);

	/*
	In case of h is not a multiple of 3. 
	diff,extra_a: (2,1); (1,0)
	This variables decided which strategy to adopt when rescaling.
	This behavior should be tested. TODO
	*/

	int diff = w%3;
	int extra_a = ( (diff-1)> 0)?diff-1:0;	//max between diff-1, 0

	Point white_1_a,white_1_b,white_1_c,white_1_d;
	Point white_2_a,white_2_b,white_2_c,white_2_d;

	white_1_a.y = 0;
	white_1_a.x = 0;

	white_1_b.y = 0;
	white_1_b.x = third-1+extra_a;

	white_1_c.y = h-1;
	white_1_c.x = 0;

	white_1_d.y = h-1;
	white_1_d.x = third-1+extra_a;

	white_2_a.y = 0;
	white_2_a.x = 2*third;

	white_2_b.y = 0;
	white_2_b.x = w-1;

	white_2_c.y = h-1;
	white_2_c.x = 2*third;

	white_2_d.y = h-1;
	white_2_d.x = w-1;

	Point black_a, black_b, black_c, black_d;

	black_a.y = 0;
	black_a.x = third+extra_a;

	black_b.y = 0;
	black_b.x = 2*third-1+diff;

	black_c.y = h-1;
	black_c.x = third+extra_a;

	black_d.y = h-1;
	black_d.x = 2*third-1+diff;

	MaskBlock white_1 = MaskBlock(white_1_a,white_1_b,white_1_c, white_1_d);
	MaskBlock white_2 = MaskBlock(white_2_a,white_2_b,white_2_c, white_2_d);
	MaskBlock black = MaskBlock(black_a,black_b,black_c,black_d);

	Mask m = Mask(size, 2);
	m.addWhite(white_1);
	m.addWhite(white_2);
	m.addBlack(black);

	return m;
}

Mask createMaskM3V(Point size){
	ulong h = size.y;
	ulong w = size.x;

	int diff = h%3;

	int third = (int) floor(h/3);
	int extra_a = (diff-1)>0?diff-1:0;

	int middle = (int) floor(h/3);

	Point white_1_a, white_1_b, white_1_c, white_1_d;
	Point white_2_a, white_2_b, white_2_c, white_2_d;


	white_1_a.y = 0;
	white_1_a.x = 0;

	white_1_b.y = 0;
	white_1_b.x = w-1;

	white_1_c.y = third-1+extra_a;
	white_1_c.x = 0;

	white_1_d.y = third-1+extra_a;
	white_1_d.x = w-1;

	white_2_a.y = 2*third;
	white_2_a.x = 0;

	white_2_b.y = 2*third;
	white_2_b.x = w-1;

	white_2_c.y = h-1;
	white_2_c.x = 0;

	white_2_d.y = h-1;
	white_2_d.x = w-1;

	Point black_a, black_b, black_c, black_d;

	black_a.y = third+extra_a;
	black_a.x = 0;

	black_b.y = third+extra_a;
	black_b.x = w-1;

	black_c.y = 2*third-1+diff;
	black_c.x = 0;

	black_d.y = 2*third-1+diff;
	black_d.x = w-1;

	MaskBlock white_1 = MaskBlock( white_1_a, white_1_b, white_1_c, white_1_d);
	MaskBlock white_2 = MaskBlock( white_2_a, white_2_b, white_2_c, white_2_d);
	MaskBlock black = MaskBlock( black_a, black_b, black_c, black_d );

	Mask m = Mask(size,3);
	m.addWhite(white_1);
	m.addWhite(white_2);
	m.addBlack(black);

	return m;
}

Mask createMaskMD(Point size){
	ulong h = size.y;
	ulong w = size.x;

	int middle_h = (int) floor(h/2);
	int middle_w = (int) floor(w/2);

	Point black_1_a,black_1_b,black_1_c,black_1_d;
	Point black_2_a,black_2_b,black_2_c,black_2_d;

	black_1_a.y = 0;
	black_1_a.x = 0;

	black_1_b.y = 0;
	black_1_b.x = middle_w-1;

	black_1_c.y = middle_h-1;
	black_1_c.x = 0;

	black_1_d.y = middle_h-1;
	black_1_d.x = middle_w-1;

	black_2_a.y = middle_h;
	black_2_a.x = middle_w;

	black_2_b.y = middle_h;
	black_2_b.x = w-1;

	black_2_c.y = h-1;
	black_2_c.x = middle_h;

	black_2_d.y = h-1;
	black_2_d.x = w-1;

	Point white_1_a,white_1_b,white_1_c,white_1_d;
	Point white_2_a,white_2_b,white_2_c,white_2_d;	

	white_1_a.y = 0;
	white_1_a.x = middle_w;

	white_1_b.y = 0;
	white_1_b.x = w-1;

	white_1_c.y = middle_h-1;
	white_1_c.x = middle_w;

	white_1_d.y = middle_h-1;
	white_1_d.x = w-1;

	white_2_a.y = middle_h;
	white_2_a.x = 0;

	white_2_b.y = middle_h;
	white_2_b.x = middle_w-1;

	white_2_c.y = h-1;
	white_2_c.x = 0;

	white_2_d.y = h-1;
	white_2_d.x = middle_w-1;

	MaskBlock white_1 = MaskBlock(white_1_a, white_1_b, white_1_c, white_1_d);
	MaskBlock white_2 = MaskBlock(white_2_a, white_2_b, white_2_c, white_2_d);

	MaskBlock black_1 = MaskBlock(black_1_a, black_1_b, black_1_c, black_1_d);
	MaskBlock black_2 = MaskBlock(black_2_a, black_2_b, black_2_c, black_2_d);

	Mask m = Mask(size,4);
	m.addWhite(white_1);
	m.addWhite(white_2);
	m.addBlack(black_1);
	m.addBlack(black_2);

	return m;
}