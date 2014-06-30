#ifndef __C_IMAGE__
#define __C_IMAGE__

#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>
#include "basic.h"

typedef unsigned long ulong;

#define A1 0
#define A2 1
#define B1 2
#define B2 3
#define C1 4
#define C2 5
#define D1 6
#define D2 7
#define W 8
#define H 9

#define HEIGHT 0
#define WIDTH 1

void loadImage(ulong*** data,const char* filepath,Point* size);
void getSize(const char* filepath,Point* size);
void drawRectangles(const char* filepath,int num_boxes,int** boxes, const char* color);
void drawRectangles(MagickWand* image_wand,int num_boxes,int** boxes, const char* color);
#endif