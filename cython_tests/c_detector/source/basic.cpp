#include "../headers/basic.h"

Subwindow::Subwindow(int x, int y, Point wr, double ce, int ng){
    _x = x;
    _y = y;
    
    _size = computeSubwindowSize(wr,ce);
    
    _cur_ng = ng;
    _ce = ce;
}