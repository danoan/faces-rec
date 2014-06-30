#include "../headers/image.h"

void getSize(const char* filepath,Point* size){
	MagickWand * image_wand;
	MagickBooleanType status;

	image_wand = NewMagickWand();
	status=MagickReadImage(image_wand,filepath);	

	size->y = MagickGetImageHeight(image_wand);
	size->x = MagickGetImageWidth(image_wand);

	DestroyMagickWand(image_wand);
}

void loadImage(ulong*** data, const char* filepath,Point* size){
	MagickWand * image_wand;
	PixelIterator* iterator;
	MagickBooleanType status;
	MagickPixelPacket pixel;
	PixelWand** pixels;

	long x,y;
	ulong line_sum;

	image_wand = NewMagickWand();
	status=MagickReadImage(image_wand,filepath);	

	iterator=NewPixelIterator(image_wand);

	size->y = MagickGetImageHeight(image_wand);
	size->x = MagickGetImageWidth(image_wand);

	// printf("MAGICK %lu %lu\n",size->y,size->x);

	if(data==NULL){
		return;
	}

	(*data) = (unsigned long**) malloc(sizeof(unsigned long*)*(size->y));


	for (y=0; y < size->y; ++y){		
	    pixels=PixelGetNextIteratorRow(iterator,&(size->x));
	    (*data)[y] = (unsigned long*) calloc(size->x,sizeof(unsigned long));

	    if ( (pixels == (PixelWand **) NULL) ) break;

	    line_sum=0;
	    for (x=0; x < (long) size->x; ++x){
	    	line_sum+= PixelGetBlue(pixels[x])*255;

	    	if(y>0){
	    		(*data)[y][x] = (*data)[y-1][x] + line_sum;
	    	}else{
	    		(*data)[y][x] = line_sum;
	    	}
	
	    	// printf("%lu\n",(*data)[y][x]);
    	}	
    }
    DestroyPixelIterator(iterator);
    DestroyMagickWand(image_wand);	    
}

void drawRectangles(const char* filepath,int num_boxes,int** boxes, const char* color){
	MagickWand* image_wand;
	DrawingWand* d_wand;
	PixelWand* c_wand;

	MagickBooleanType status;
	MagickPixelPacket pixel;

	long x,y;
	ulong line_sum;

	// printf("%d\n",num_boxes);

	image_wand = NewMagickWand();
	d_wand = NewDrawingWand();
	c_wand = NewPixelWand();

	PixelSetColor(c_wand,color);
	DrawSetFillOpacity(d_wand,0.0);
	DrawSetStrokeColor(d_wand,c_wand);
	DrawSetStrokeWidth(d_wand,1);

	// printf("BEFORE DRAW\n");

	int i;
	for(i=0;i<num_boxes;i++){
		DrawRectangle(d_wand,boxes[i][0],boxes[i][1],boxes[i][2],boxes[i][3]);	
	}	

	status=MagickReadImage(image_wand,filepath);	
	MagickDrawImage(image_wand,d_wand);		

	MagickDisplayImage(image_wand,"");
	DestroyMagickWand(image_wand);	   
}

void drawRectangles(MagickWand* image_wand,int num_boxes,int** boxes, const char* color){
	DrawingWand* d_wand;
	PixelWand* c_wand;

	MagickPixelPacket pixel;

	long x,y;
	ulong line_sum;

	// printf("%d\n",num_boxes);
	d_wand = NewDrawingWand();
	c_wand = NewPixelWand();

	PixelSetColor(c_wand,color);
	DrawSetFillOpacity(d_wand,1.0);
	DrawSetFillColor(d_wand,c_wand);
	DrawSetStrokeColor(d_wand,c_wand);
	DrawSetStrokeWidth(d_wand,1);

	// printf("BEFORE DRAW\n");

	int i;
	for(i=0;i<num_boxes;i++){
		DrawRectangle(d_wand,boxes[i][0],boxes[i][1],boxes[i][2],boxes[i][3]);	
	}	
	
	MagickDrawImage(image_wand,d_wand);			
}