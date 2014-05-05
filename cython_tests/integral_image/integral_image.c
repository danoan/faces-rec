#include <Python.h>
#include <sys/time.h>
#include <wand/MagickWand.h>

#define NDEBUG
#include <assert.h>

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

ulong** __imgData;
ulong __iw,__ih;

static PyObject* silly(PyObject* self, PyObject* args){
	int a,b;

	if(!PyArg_ParseTuple(args,"ii",&a,&b)){
		return NULL;
	}

	return Py_BuildValue("i",a+b);	
}

int pyToInt(PyObject* py, ulong* a){
	if(py==NULL){return NULL;}

	if(PyNumber_Check(py)!=1){
		return NULL;
	}

	PyObject* temp = PyNumber_Long(py);
	*a = PyLong_AsUnsignedLong( temp );

	Py_DECREF(temp);

	return 1;
}

int pyToStr(PyObject* py, char** a){
	if(py==NULL){return NULL;}

	if(PyString_Check(py)!=1){
		return NULL;
	}

	*a = PyString_AsString( py );
	
	return 1;
}

int parseArguments(PyObject* args, ulong* x, ulong* y, ulong* nwb, ulong* nbb, ulong** wb, ulong** bb){
	/*	location_x,location_y
		number_white_blocks
		white_block_1 = a1,a2,b1,b2,c1,c2,d1,d2,w,h
		...
		white_block_n
		number_black_blocks
		black_block_1 = a1,a2,b1,b2,c1,c2,d1,d2,w,h
		...
		black_block_n
	*/

	int i,arg_item;
	
	if(!pyToInt( PyTuple_GetItem(args,0), x)){return NULL;}	 		assert(*x==10);
	if(!pyToInt( PyTuple_GetItem(args,1), y)){return NULL;}			assert(*y==10);
	if(!pyToInt( PyTuple_GetItem(args,2), nwb)){return NULL;}		assert(*nwb==2);
	if(!pyToInt( PyTuple_GetItem(args,3), nbb)){return NULL;} 		assert(*nbb==2);

	(*wb) = (ulong*) malloc(sizeof(ulong)*(*nwb)*10);
	(*bb) = (ulong*) malloc(sizeof(ulong)*(*nbb)*10);


	arg_item=4;

	for(i=0;i<(*nwb)*10;i++){
		if(!pyToInt( PyTuple_GetItem(args,arg_item++), &( (*wb)[i] ) ) ){
			return NULL;
		}
	}

	for(i=0;i<(*nbb)*10;i++){
		if(!pyToInt( PyTuple_GetItem(args,arg_item++), &( (*bb)[i] ) ) ){
			return NULL;
		}
	}	

	return 1;
}

ulong attr(ulong* lb,int bn, int at){
	/*
		lp = List of Blocks
		bn = Block Number
		at = Attribute (A,B,C,D,W,H)
	*/
	return lb[bn*10+at];
}

void readImg(const char* filepath,ulong*** data,ulong* iw, ulong* ih){
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

	(*ih) = MagickGetImageHeight(image_wand);

	(*data) = (unsigned long**) malloc(sizeof(unsigned long*)*(*ih));


	for (y=0; y < (*ih); y++){		
	    pixels=PixelGetNextIteratorRow(iterator,iw);
	    (*data)[y] = (unsigned long*) calloc((*iw),sizeof(unsigned long));

	    if ( (pixels == (PixelWand **) NULL) ) break;

	    line_sum=0;
	    for (x=0; x < (long) (*iw); x++){
	    	line_sum+= PixelGetBlue(pixels[x])*255;

	    	if(y>0){
	    		(*data)[y][x] = (*data)[y-1][x] + line_sum;
	    	}else{
	    		(*data)[y][x] = line_sum;
	    	}
	
	    	// printf("%lu\n",(*data)[y][x]);
    	}	
    }
}


void getPoint(ulong* lb, ulong i,ulong x, ulong y, ulong* pa, ulong* pb, ulong* pc, ulong* pd){
	pa[0] = y + attr( lb,i,A1);
	pa[1] = x + attr( lb,i,A2);

	pb[0] = y + attr( lb,i,B1);
	pb[1] = x + attr( lb,i,B2);

	pc[0] = y + attr( lb,i,C1);
	pc[1] = x + attr( lb,i,C2);							

	pd[0] = y + attr( lb,i,D1);
	pd[1] = x + attr( lb,i,D2);				
}

int outsideLimits(ulong** p,ulong w, ulong h){
	return p[HEIGHT] >= h || p[HEIGHT] < 0 || p[WIDTH] >= w || p[WIDTH] < 0;
}

ulong getFromII(ulong** data,ulong* pa, ulong* pb, ulong* pc, ulong* pd,ulong iw, ulong ih){
	pa[HEIGHT] = pa[HEIGHT]-1;
	pa[WIDTH] = pa[WIDTH]-1;

	pb[HEIGHT] = pb[HEIGHT]-1;
	pb[WIDTH] = pb[WIDTH];

	pc[HEIGHT] = pc[HEIGHT];
	pc[WIDTH] = pc[WIDTH]-1;

	// printf("OUT %d\n",outsideLimits(pa,iw,ih));

	ulong sumA = outsideLimits(pa,iw,ih)?0:data[pa[HEIGHT]][pa[WIDTH]];
	ulong sumB = outsideLimits(pb,iw,ih)?0:data[pb[HEIGHT]][pb[WIDTH]];
	ulong sumC = outsideLimits(pc,iw,ih)?0:data[pc[HEIGHT]][pc[WIDTH]];
	ulong sumD = outsideLimits(pd,iw,ih)?0:data[pd[HEIGHT]][pd[WIDTH]];

	return sumA + sumD - sumC - sumB;
}

ulong computeBlock(ulong** data,ulong* lb,ulong nb,ulong x, ulong y, ulong iw, ulong ih){
	/*
		A1 = (y,x)
	*/
	int i;
	ulong pa[2],pb[2],pc[2],pd[2];

	ulong sumBlock = 0;
	for(i=0;i<nb;i++){
		getPoint(lb,i,x,y,&(pa[0]),&(pb[0]),&(pc[0]),&(pd[0]));		
		sumBlock += getFromII(data,pa,pb,pc,pd, iw, ih);
	}

	return sumBlock;
}

static PyObject* setImage(PyObject* self, PyObject* args){
	char* filename;
		
	if(!pyToStr( PyTuple_GetItem(args,0), &filename)){return NULL;}	

	struct timeval stop,start;

	gettimeofday(&start,NULL);
	readImg(filename,&__imgData,&__iw,&__ih);	
	gettimeofday(&stop,NULL);

	//readImg it takes 0.006s. More than half of the 0.00010
	double ft = (stop.tv_sec-start.tv_sec) + (stop.tv_usec-start.tv_usec)*1e-6;
	// printf("READ IMAGE: %lf\n", ft);

	// printf("%s\n",filename);	

	return Py_BuildValue("i",1);
}

static PyObject* finishUse(PyObject* self, PyObject* args){
	free(__imgData);
	return Py_None;
}

static PyObject* integral(PyObject* self, PyObject* args){

	ulong x,y,nwb,nbb;
	ulong* wb;
	ulong* bb;

	struct timeval stop,start;
	gettimeofday(&start,NULL);
	
	if(!parseArguments(args,&x,&y,&nwb,&nbb,&wb,&bb)){
		return NULL;
	}

	assert(wb[0]==0);	assert(wb[13]==11);		assert(wb[19]==10);
	assert(bb[0]==20);	assert(bb[13]==31);		assert(bb[19]==20);

	ulong sumWhite = computeBlock(__imgData,wb,nwb,x,y,__iw,__ih);
	ulong sumBlack = computeBlock(__imgData,bb,nbb,x,y,__iw,__ih);

	gettimeofday(&stop,NULL);

	ulong ft = (stop.tv_usec-start.tv_usec);
	// printf("INTEGRAL: %lu\n", ft);

	free(wb);
	free(bb);

	return Py_BuildValue("i",labs(sumBlack-sumWhite));
	// return Py_BuildValue("i",42);
};

static PyMethodDef IntegralImageMethods[] ={ 
	{"setImage",setImage,METH_VARARGS,"Set Image"},
	{"finishUse",setImage,METH_VARARGS,"Finish Use"},
	{"integral",integral,METH_VARARGS,"Integral Image"},
	{"silly",silly,METH_VARARGS,"Add 2 numbers"},
	{NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initintegral_image(void){
	(void) Py_InitModule("integral_image",IntegralImageMethods);
};

int main(int argc, char* argv[]){
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	initintegral_image();
};