#ifndef __BASIC__
#define __BASIC__

#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>

#include "log.h"
#include <sys/time.h>

typedef unsigned long int ulong;

void startClock();
void stopClock(std::string testName);

class Logger{
public:
    static FilePrint* logger;
    static FilePrint* error;
    static FilePrint* debug;

    static FilePrint* feat_anal;

    static void init(std::string folder);
};

typedef struct{
	ulong x;
	ulong y;
} Point;

typedef struct{
	Point points[4];
	ulong w,h;
} Block;

class Subwindow{
public:
    int _x;
    int _y;

    Point _size;
    int _cur_ng;
    double _ce;

    Subwindow(){};
    Subwindow(int x, int y, Point wr, double ce, int ng);   

    inline int x(){return _x;};
    inline int y(){return _y;};

    inline Point size(){return _size;};
    inline double ce(){return _ce;};

    inline void cropBox(int* box){
        box[0]=_x;
        box[1]=_y;
        box[2]=_x + _size.x;
        box[3]=_y + _size.y;
    };
};

inline Point computeSubwindowSize(Point wr,double ce){
    Point p;
    p.y = (int) ( round(wr.y*ce) );
    p.x = (int) ( round(wr.x*ce) );

    return p;
};

#endif