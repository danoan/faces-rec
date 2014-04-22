#include <stdio.h>
#include <stdlib.h>

#include <wand/MagickWand.h>


void readImg(const char* filepath,unsigned long*** data){
	MagickWand * image_wand;
	PixelIterator* iterator;
	MagickBooleanType status;
	MagickPixelPacket pixel;
	PixelWand** pixels;

	unsigned long width;
	unsigned long height;
	long x,y;

	image_wand = NewMagickWand();
	status=MagickReadImage(image_wand,filepath);	

	iterator=NewPixelIterator(image_wand);

	height = MagickGetImageHeight(image_wand);

	(*data) = (unsigned long**) malloc(sizeof(unsigned long*)*height);	

	for (y=0; y < height; y++){		
	    pixels=PixelGetNextIteratorRow(iterator,&width);
	    (*data)[y] = (unsigned long*) malloc(sizeof(unsigned long)*width);

	    if ( (pixels == (PixelWand **) NULL) ) break;

	    for (x=0; x < (long) width; x++){
	    	(*data)[y][x] = PixelGetBlue(pixels[x])*255;
    	}	
    }
}

int main(int argc, char* argv[]){
	unsigned long** data;
	readImg("/home/daniel/Projects/faces-rec/dataset/test_images/elaine.pgm",&data);

	return 0;
}