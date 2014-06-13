#ifndef __MASK__
#define __MASK__

#include <vector>
#include <cmath>

#include "../basic.h"
#include "maskBlock.h"


class Mask{
public:
    std::vector<MaskBlock> _white;
    std::vector<MaskBlock> _black;

    Point _size;
    int _createMaskId;  //When saving the object, I can`t save the function pointer
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

std::ostream& operator<<(std::ostream& os, const Mask& m);
std::istream& operator>>(std::istream& is, Mask& m);

Mask createMaskM2H(Point size);
Mask createMaskM2V(Point size);
Mask createMaskM3H(Point size);
Mask createMaskM3V(Point size);
Mask createMaskMD(Point size);

#endif