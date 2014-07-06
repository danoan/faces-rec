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
void loadAsOriginalImage(ulong*** data, const char* filepath,Point* size);

/*Free memory from imgData loaded in getImageCrops*/
void freeImgData(ulong** img_data,Point img_size);

/*Dont care about imgData. It loads and free it right after*/
int getImageCrops(ulong**** data, const char* filepath, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp);

/*imgData is not free it after the operation is done. The programmer must call freeImgData on imgData later on*/
int getImageCrops(ulong**** data, ulong*** img_data, Point* img_size, const char* filepath, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp);

/*Programmer specifies if imgData should be or should not be free it after crop operation*/
int getImageCrops(ulong**** data, ulong*** img_data, Point* img_size, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp, int freeImg);



void getSize(const char* filepath,Point* size);
void drawRectangles(const char* filepath,int num_boxes,int** boxes, const char* color);
void drawRectangles(MagickWand* image_wand,int num_boxes,int** boxes, const char* color);
#endif