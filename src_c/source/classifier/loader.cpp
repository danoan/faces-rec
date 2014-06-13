#include "../../headers/classifier/loader.h"

Point cppPoint(C_Point cp){
	Point p;
	p.x = cp.x;
	p.y = cp.y;

	return p;
}

void cppAddBlocks(C_Block* c_blocks, ulong c_len, Mask* m, int blockType){

	for(int k=0;k<c_len;++k){
		C_Block c_b = c_blocks[k];
		MaskBlock mb( cppPoint(c_b.points[0]), cppPoint(c_b.points[1]), cppPoint(c_b.points[2]), cppPoint(c_b.points[3]) ) ;

		mb.w(c_b.w);
		mb.h(c_b.h);

		m->addBlock(mb,blockType);
	}
}

Classifier load_Classifier(const char* filename){
	C_Classifier c_cl = loadC_Classifier(filename);
	Classifier cl;

	cl.ardis(cppPoint(c_cl.ardis));

	for(int i=0;i<c_cl.len_hypothesis;++i){
		Hypothesy h;
		C_Hypothesy c_h = c_cl.hypothesis[i];

		h.threshold(c_h.threshold);
		h.direction(c_h.direction);
		h.alpha(c_h.alpha);

		
		Mask m(cppPoint(c_h.fm.mask.size), c_h.fm.mask.maskType);

		cppAddBlocks(c_h.fm.mask.white, c_h.fm.mask.len_white, &m, 0);
		cppAddBlocks(c_h.fm.mask.black, c_h.fm.mask.len_black, &m, 1);

		FeatureMask fm(m,cppPoint(c_h.fm.location));
		h.fm(fm);

		cl.addHypothesy(h);
	}	

	return cl;
}
