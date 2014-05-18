#include "../../headers/feature/maskBlock.h"

MaskBlock::MaskBlock(Point pa, Point pb, Point pc, Point pd){
    this->_points[0] = pa;
    this->_points[1] = pb;
    this->_points[2] = pc;
    this->_points[3] = pd;

    this->_w = pb.x - pa.x + 1;
    this->_h = pc.y - pc.y + 1;
};