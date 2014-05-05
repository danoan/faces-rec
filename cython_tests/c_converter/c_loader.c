#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include "c_converter.h"

Classifier __loader_cl;

void printBlock(Block* b, ulong len){
	int i,j;
	for(i=0;i<len;i++){
		printf("[ ");
		for(j=0;j<4;j++){
			printf("(%lu,%lu)",b[i].points[j].x,b[i].points[j].y);	
		}
		printf("] W: %lu H: %lu\n",b[i].w,b[i].h);		
	}	
	printf("\n");
}

void printFeatureMask(FeatureMask fm){
	printf("\tMask (%d):\n", fm.mask.maskType);
	printf("\t\tWhite:\n");
	printBlock(fm.mask.white,fm.mask.len_white);

	printf("\t\tBlack:\n");
	printBlock(fm.mask.white,fm.mask.len_white);	

	printf("Location: (%lu, %lu)\nOriginal_Location:(%lu,%lu)\nOriginal_Size:(%lu,%lu)\n",fm.location.x,fm.location.y,fm.__original_location.x,fm.__original_location.y,fm.__original_size.x,fm.__original_size.y);
}

void printHypothesy(Hypothesy h){
	printf("Threshold: %lu\nDirection: %ld\nAlpha: %f\nFeatureMask:\n",h.threshold,h.direction,h.alpha);
	printFeatureMask(h.fm);
}

void printClassifier(Classifier cl){
	int i;
	printf("Print Classificador\n\n Final:%lu\n Ardis:(%lu,%lu)\n\n",cl.final,cl.ardis.y,cl.ardis.x);
	for(i=0;i<cl.len_hypothesis;i++){
		printf("\n\nHypothesis (%d-%lu)\n\n",i,cl.len_hypothesis);
		printHypothesy(cl.hypothesis[i]);		
	}	
}

Classifier loadClassifier(char* filename){
	Classifier cl;
	
	FILE* f = fopen(filename,"rb");
	
	fread(&cl.final,sizeof(ulong),1,f);
	fread(&cl.ardis,sizeof(Point),1,f);
	fread(&cl.len_hypothesis,sizeof(ulong),1,f);

	cl.hypothesis = (Hypothesy*) malloc(sizeof(Hypothesy)*cl.len_hypothesis);
	int i,j;
	for(i=0;i<cl.len_hypothesis;i++){
		Hypothesy* h = &(cl.hypothesis[i]);
		fread(&h->threshold,sizeof(ulong),1,f);
		fread(&h->direction,sizeof(long),1,f);
		fread(&h->alpha,sizeof(double),1,f);
		
		fread(&h->fm.__original_location,sizeof(Point),1,f);
		fread(&h->fm.__original_size,sizeof(Point),1,f);
		fread(&h->fm.location,sizeof(Point),1,f);

		fread(&h->fm.mask.maskType,sizeof(int),1,f);
		fread(&h->fm.mask.size,sizeof(Point),1,f);

		fread(&h->fm.mask.len_white,sizeof(ulong),1,f);
		h->fm.mask.white = (Block*) malloc(sizeof(Block)*h->fm.mask.len_white);

		for(j=0;j<h->fm.mask.len_white;j++){
			fread(&h->fm.mask.white[j].points,sizeof(Point),4,f);
			fread(&h->fm.mask.white[j].w,sizeof(ulong),1,f);
			fread(&h->fm.mask.white[j].h,sizeof(ulong),1,f);
		}

		fread(&h->fm.mask.len_black,sizeof(ulong),1,f);
		h->fm.mask.black = (Block*) malloc(sizeof(Block)*h->fm.mask.len_black);

		for(j=0;j<h->fm.mask.len_black;j++){
			fread(&h->fm.mask.black[j].points,sizeof(Point),4,f);
			fread(&h->fm.mask.black[j].w,sizeof(ulong),1,f);
			fread(&h->fm.mask.black[j].h,sizeof(ulong),1,f);
		}		

		h->fm.__original_mask = h->fm.mask;
	}

	fclose(f);

	return cl;
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

PyObject* pointerToPy(ulong* p,ulong size){
	PyObject* list = PyList_New(size);

	int i=0;
	for(i=0;i<size;i++){
		PyList_SetItem( list, i, Py_BuildValue("k",p[i]) );	
	}

	return list;
}


static PyObject* load_classifier(PyObject* self, PyObject* args){
	char* filename;
	pyToStr( PyTuple_GetItem(args,0),&filename );

	__loader_cl = loadClassifier(filename);	

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* print_classifier(PyObject* self, PyObject* args){
	printClassifier(__loader_cl);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* get_attribute(PyObject* self, PyObject* args){
	char* attr;
	ulong hi=NULL;
	ulong bi=NULL;

	pyToStr( PyTuple_GetItem(args,0), &attr );
	if(PyTuple_Size(args)>=2){
		pyToInt( PyTuple_GetItem(args,1), &hi );	
	}

	if(PyTuple_Size(args)>=3){		
		pyToInt( PyTuple_GetItem(args,2), &bi );	
	}	

	// printf("%s %lu %lu\n",attr,hi,bi);

	if(strcmp(attr,"FINAL")==0){

		return Py_BuildValue("k",__loader_cl.final);

	}else if(strcmp(attr,"ARDIS")==0){
		ulong ardis[2] = {__loader_cl.ardis.y,__loader_cl.ardis.x};
		
		return Py_BuildValue("O",pointerToPy(ardis,2));

	}else if(strcmp(attr,"THRESHOLD")==0){

		return Py_BuildValue("k",__loader_cl.hypothesis[hi].threshold);

	}else if(strcmp(attr,"DIRECTION")==0){

		return Py_BuildValue("l",__loader_cl.hypothesis[hi].direction);

	}else if(strcmp(attr,"ALPHA")==0){

		return Py_BuildValue("d",__loader_cl.hypothesis[hi].alpha);

	}else if(strcmp(attr,"ORIGINAL_LOCATION")==0){
		ulong location[2] = {__loader_cl.hypothesis[hi].fm.__original_location.y, __loader_cl.hypothesis[hi].fm.__original_location.x};

		return Py_BuildValue("O", pointerToPy( location , 2 ) );
			
	}else if(strcmp(attr,"ORIGINAL_SIZE")==0){
		ulong size[2] = {__loader_cl.hypothesis[hi].fm.__original_size.y, __loader_cl.hypothesis[hi].fm.__original_size.x};

		return Py_BuildValue("O", pointerToPy( size , 2 ) );		
		
	}else if(strcmp(attr,"LOCATION")==0){
		ulong location[2] = {__loader_cl.hypothesis[hi].fm.location.y, __loader_cl.hypothesis[hi].fm.location.x};

		return Py_BuildValue("O", pointerToPy( location , 2 ) );		
	}else if(strcmp(attr,"WHITE_POINTS")==0){
		Block bw = __loader_cl.hypothesis[hi].fm.mask.white[bi];
		ulong points[8] = {bw.points[0].y,bw.points[0].x,bw.points[1].y,bw.points[1].x,bw.points[2].y,bw.points[2].x,bw.points[3].y,bw.points[3].x};
		
		return Py_BuildValue("O", pointerToPy(points,8));

	}else if(strcmp(attr,"WHITE_W")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.white[bi].w);
	}else if(strcmp(attr,"WHITE_H")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.white[bi].h);
	}else if(strcmp(attr,"BLACK_POINTS")==0){
		Block bb = __loader_cl.hypothesis[hi].fm.mask.black[bi];
		ulong points[8] = {bb.points[0].y,bb.points[0].x,bb.points[1].y,bb.points[1].x,bb.points[2].y,bb.points[2].x,bb.points[3].y,bb.points[3].x};
		
		return Py_BuildValue("O", pointerToPy(points,8));		
	}else if(strcmp(attr,"BLACK_W")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.black[bi].w);
	}else if(strcmp(attr,"BLACK_H")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.black[bi].h);
	}else if(strcmp(attr,"SIZE")==0){
		ulong size[2] ={__loader_cl.hypothesis[hi].fm.mask.size.y,__loader_cl.hypothesis[hi].fm.mask.size.x};

		return Py_BuildValue("O",pointerToPy(size,2));
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef CLoaderMethods[] ={ 
	{"load_classifier",load_classifier,METH_VARARGS,"Load Classifier"},
	{"get_attribute",get_attribute,METH_VARARGS,"Get Attibute"},
	{"print_classifier",print_classifier,METH_VARARGS,"Print Classifier"},
	{NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initc_loader(void){
	(void) Py_InitModule("c_loader",CLoaderMethods);
};

int main(int argc, char* argv[]){
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	initc_loader();
};