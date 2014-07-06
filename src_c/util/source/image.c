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

void loadAsOriginalImage(ulong*** data, const char* filepath,Point* size){
	MagickWand * image_wand;
	PixelIterator* iterator;
	MagickBooleanType status;
	MagickPixelPacket pixel;
	PixelWand** pixels;

	long x,y;

	image_wand = NewMagickWand();
	status=MagickReadImage(image_wand,filepath);	

	iterator=NewPixelIterator(image_wand);

	size->y = MagickGetImageHeight(image_wand);
	size->x = MagickGetImageWidth(image_wand);

	if(data==NULL){
		return;
	}

	(*data) = (unsigned long**) malloc(sizeof(unsigned long*)*(size->y));


	for (y=0; y < size->y; ++y){		
	    pixels=PixelGetNextIteratorRow(iterator,&(size->x));
	    (*data)[y] = (unsigned long*) calloc(size->x,sizeof(unsigned long));

	    if ( (pixels == (PixelWand **) NULL) ) break;

	    for (x=0; x < (long) size->x; ++x){
    		(*data)[y][x] = PixelGetBlue(pixels[x])*255;
    		
    	}	
    }    

    DestroyPixelIterator(iterator);
    DestroyMagickWand(image_wand);	    
}

int getImageCrops(ulong**** data, const char* filepath, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp){
	ulong** img_data;
	Point img_size;
	loadAsOriginalImage(&img_data,filepath,&img_size);	//Jah retorna com as somas do integral image		
	return getImageCrops(data,&img_data,&img_size,crop_start_index,ncrops,maxCrops,crop_width,crop_height,shift_step,checkData,vp,1);
}

int getImageCrops(ulong**** data, ulong*** img_data, Point* img_size, const char* filepath, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp){
	printf("IMG LOADED \n");
	loadAsOriginalImage(img_data,filepath,img_size);	//Jah retorna com as somas do integral image		
	return getImageCrops(data,img_data,img_size,crop_start_index,ncrops,maxCrops,crop_width,crop_height,shift_step,checkData,vp,0);
}

int getImageCrops(ulong**** data, ulong*** img_data, Point* img_size, int* crop_start_index, int ncrops, int maxCrops, int crop_width, int crop_height, int shift_step, int(* checkData)(ulong**,Point,void*), void* vp, int freeImg){
	int x,y;

	int real_width = img_size->x - crop_width;
	int real_height = img_size->y - crop_height;

	(*data) = (ulong***) malloc(sizeof(*data)*ncrops);

	int t,h;
	for(t=0;t<ncrops;t++){
		(*data)[t] = (ulong**) malloc(sizeof(**data)*crop_height);		
		for(h=0;h<crop_height;h++){
			(*data)[t][h] = (ulong*) malloc(sizeof(***data)*crop_width);
		}
	}

	// int** boxes = (int**) malloc(sizeof(int*)*ncrops);
	// int k;
	// for(int k=0;k<ncrops;k++){
	// 	boxes[k] = (int*) malloc(sizeof(int)*4);
	// }

	int total_crops=0;
	int i = *crop_start_index;
	ulong line_sum;
	while(total_crops<ncrops && i<maxCrops){
		Point loc;
		loc.x = (i*shift_step)%real_width;
		loc.y = (i*shift_step)/real_width*shift_step;

		// boxes[total_crops][0] = loc.x;
		// boxes[total_crops][1] = loc.y;
		// boxes[total_crops][2] = loc.x + crop_width;
		// boxes[total_crops][3] = loc.y + crop_height;

		for(y=0;y<crop_height;y++){			
			line_sum=0;
			for(x=0;x<crop_width;x++){
				line_sum+=(*img_data)[loc.y+y][loc.x+x];

				if(y>0){
					(*data)[total_crops][y][x] = (*data)[total_crops][y-1][x] + line_sum;				
				}else{
					(*data)[total_crops][y][x] = line_sum;			
				}		
			}
		}

		Point crop_size;
		crop_size.x = crop_width;
		crop_size.y = crop_height;

		if(vp==NULL){
			total_crops+=1;
		}else{
			if(checkData((*data)[total_crops],crop_size,vp)==1){
				total_crops+=1;
			}
		}

		i++;
	}
	*crop_start_index = i;

	// drawRectangles(filepath,total_crops,boxes,"Green");

	for(t=total_crops;t<ncrops;t++){
		for(h=0;h<crop_height;h++){
			free((*data)[t][h]);
		}
		free((*data)[t]);
	}

	if(freeImg==1){
		freeImgData(*img_data,*img_size);
	}

	return total_crops;
}

void freeImgData(ulong** img_data,Point img_size){
	for(int i=0;i<img_size.y;i++){
		free(img_data[i]);
	}
	free(img_data);		
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