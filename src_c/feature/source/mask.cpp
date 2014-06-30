#include "../headers/mask.h"

Mask::Mask(Point size, int maskType){
    this->_size = size;
    switch(maskType){
        case 0:
            this->_createMask = &(createMaskM2H);
            this->_createMaskId = 0;
            break;
        case 1:
            this->_createMask = &(createMaskM2V);
            this->_createMaskId = 1;
            break;
        case 2:
            this->_createMask = &(createMaskM3H);
            this->_createMaskId = 2;
            break;
        case 3:
            this->_createMask = &(createMaskM3V);
            this->_createMaskId = 3;
            break;
        case 4:
            this->_createMask = &(createMaskMD);
            this->_createMaskId = 4;
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

    this->_size.x = (int) floor( original_size.x*ce );
    this->_size.y = (int) floor( original_size.y*ce );

    return (*this->_createMask)(this->_size);
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
    int extra_a = ( (diff-1)> 0)?diff-1:0;  //max between diff-1, 0

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

std::ostream& operator<<(std::ostream& os, const Mask& m){
    os << m._size.x << '\n';
    os << m._size.y << '\n';
    
    os << m._white.size() << '\n';
    for(register int i=0;i<m._white.size();i++){
        os << m._white[i];
    }

    os << m._black.size() << '\n';
    for(register int i=0;i<m._black.size();i++){
        os << m._black[i];
    }        

    os << m._createMaskId << '\n';

    return os;
}

std::istream& operator>>(std::istream& is, Mask& m){
    is >> m._size.x;
    is >> m._size.y;

    int s_white;
    is >> s_white;
    for(register int i=0;i<s_white;i++){
        MaskBlock mb;
        is >> mb;
        m._white.push_back(mb);
    }

    int s_black;
    is >> s_black;
    for(register int i=0;i<s_black;i++){
        MaskBlock mb;
        is >> mb;
        m._black.push_back(mb);
    }

    is >> m._createMaskId;

    switch(m._createMaskId){
        case 0:
            m._createMask = &(createMaskM2H);
            break;
        case 1:
            m._createMask = &(createMaskM2V);
            break;
        case 2:
            m._createMask = &(createMaskM3H);
            break;
        case 3:
            m._createMask = &(createMaskM3V);
            break;
        case 4:
            m._createMask = &(createMaskMD);
            break;
    }

    return is;
}
