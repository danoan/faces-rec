#include "../headers/maskBlock.h"

MaskBlock::MaskBlock(Point pa, Point pb, Point pc, Point pd){
    this->_points[0] = pa;
    this->_points[1] = pb;
    this->_points[2] = pc;
    this->_points[3] = pd;

    this->_w = pb.x - pa.x + 1;
    this->_h = pc.y - pa.y + 1;
};

std::ostream& operator<<(std::ostream& os, const MaskBlock& mb){
    for(register int i=0;i<4;i++){
        os << mb._points[i].x << '\n';    
        os << mb._points[i].y << '\n';
    }
    os << mb._w << '\n';
    os << mb._h << '\n';

    return os;
};

std::istream& operator>>(std::istream& is, MaskBlock& mb){
    for(register int i=0;i<4;i++){
        is >> mb._points[i].x;
        is >> mb._points[i].y;
    }

    is >> mb._w;
    is >> mb._h;

    return is;
}