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

void MaskBlock::asPlainVector(PlainWriter<ulong>& pw, const MaskBlock& mb){
    for(register int i=0;i<4;i++){
		pw.write(mb._points[i].x);
		pw.write(mb._points[i].y);
    }
    pw.write(mb._w);
    pw.write(mb._h);
}

MaskBlock MaskBlock::fromPlainVector(PlainWriter<ulong>& pw){
	MaskBlock mb;
    for(register int i=0;i<4;i++){
        mb._points[i].x = pw.read();
        mb._points[i].y = pw.read();
    }

    mb._w = pw.read();
    mb._h = pw.read();

    return mb;	
}
