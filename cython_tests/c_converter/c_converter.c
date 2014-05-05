#include <Python.h>
#include <stdio.h>
#include <string.h>
#include "c_converter.h"

#define NDEBUG
#include <assert.h>

Block* __c_white_cursor;
Block* __c_black_cursor;
Hypothesy* __c_hypothesy_cursor;
Classifier __conv_cl;

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

int printBlocks(Block* bl, ulong i, ulong j){
	ulong x=0;
	ulong y;	
	while(x<i){
		y=0;
		while(y<j){
			// printf("(%lu,%lu) ",bl[x].points[y].x,bl[x].points[y].y);
			y++;
		}
		x++;
		// printf("\n");
	}
}

static PyObject* init(PyObject* self, PyObject* args){

	PyObject* py_final = PyTuple_GetItem(args,0);
	PyObject* py_ardis = PyTuple_GetItem(args,1);

	if(!PyBool_Check(py_final)){
		__conv_cl.final = 1;
	}else{
		if(py_final==Py_True){
			__conv_cl.final = 1;
		}else{
			__conv_cl.final = 0;
		}
	}

	if(!PyTuple_Check(py_ardis)){
		__conv_cl.ardis.x=0;
		__conv_cl.ardis.y=1;
	}else{
		pyToInt( PyTuple_GetItem(py_ardis,0),&(__conv_cl.ardis.y) );
		pyToInt( PyTuple_GetItem(py_ardis,1),&(__conv_cl.ardis.x) );
	}

	pyToInt( PyTuple_GetItem(args,2), &__conv_cl.len_hypothesis);
	__conv_cl.hypothesis = (Hypothesy*) malloc(sizeof(Hypothesy)*__conv_cl.len_hypothesis);

	__c_hypothesy_cursor = __conv_cl.hypothesis;
	
	// printf("\n%lu %lu %lu\n",cl.final,cl.ardis.x,cl.ardis.y);
	Py_DECREF(py_final);
	Py_DECREF(py_ardis);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_hypothesis(PyObject* self, PyObject* args){

	if(!PyArg_ParseTuple(args,"kkd",&(__c_hypothesy_cursor[0].threshold),&(__c_hypothesy_cursor[0].direction),&(__c_hypothesy_cursor[0].alpha))){
		return NULL;
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_feature(PyObject* self, PyObject* args){
	PyObject* py_location = PyTuple_GetItem(args,0);

	if(!PyTuple_Check(py_location)){
		return NULL;
	}

	FeatureMask* fm = &(__c_hypothesy_cursor[0].fm);

	pyToInt( PyTuple_GetItem(py_location,0),&(fm->location.y) );
	pyToInt( PyTuple_GetItem(py_location,1), &(fm->location.x) );

	Py_DECREF(py_location);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_mask(PyObject* self, PyObject* args){
	char* factoryName;
	Mask* m = &(__c_hypothesy_cursor[0].fm.mask);

	pyToStr(PyTuple_GetItem(args,0),&factoryName);
	if(strcmp(factoryName,"MaskTwoHorizontalFactory")==0){
		m->maskType=0;
	}else if(strcmp(factoryName,"MaskTwoVerticalFactory")==0){
		m->maskType=1;
	}else if(strcmp(factoryName,"MaskThreeHorizontalFactory")==0){
		m->maskType=2;
	}else if(strcmp(factoryName,"MaskThreeVerticalFactory")==0){
		m->maskType=3;
	}else if(strcmp(factoryName,"MaskDiagonalFactory")==0){
		m->maskType=4;
	}

	PyObject* py_size = PyTuple_GetItem(args,1);
	if(!PyTuple_Check(py_size)){
		return NULL;
	}

	pyToInt(PyTuple_GetItem(py_size,0),&(m->size.y));
	pyToInt(PyTuple_GetItem(py_size,1),&(m->size.x));

	pyToInt(PyTuple_GetItem(args,2),&m->len_white);
	pyToInt(PyTuple_GetItem(args,3),&m->len_black);

	int i = 0;

	m->white = (Block*) malloc(sizeof(Block)*m->len_white);
	m->black = (Block*) malloc(sizeof(Block)*m->len_black);	

	__c_white_cursor = m->white;
	__c_black_cursor = m->black;

	// printf("%lu %lu\n",m->len_white,m->len_black);
	Py_DECREF(py_size);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* add_block(PyObject* self, PyObject* args){
	char* block_type;
	pyToStr( PyTuple_GetItem(args,0), &block_type);
	
	PyObject* py_points = PyTuple_GetItem(args,1);
	if(!PyList_Check(py_points)){
		return NULL;
	}

	Block** block_cursor;
	if(strcmp(block_type,"white")==0){
		block_cursor = &__c_white_cursor;
	}else{
		block_cursor = &__c_black_cursor;
	}

	int i;
	for(i=0;i<4;i++){
		pyToInt( PyList_GetItem(py_points,i*2), &((*block_cursor)[0].points[i].y) );
		pyToInt( PyList_GetItem(py_points,i*2+1), &((*block_cursor)[0].points[i].x) );
	}
	pyToInt( PyList_GetItem(py_points,8), &((*block_cursor)[0].w) );
	pyToInt( PyList_GetItem(py_points,9), &((*block_cursor)[0].h) );

	(*block_cursor)++;
	Py_DECREF(py_points);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* close_mask(PyObject* self, PyObject* args){
	Mask* m = &(__c_hypothesy_cursor[0].fm.mask);

	// printf("WHITE(%lu)\n\n",m->len_white);
	// printBlocks(m->white,m->len_white,4);

	// printf("BLACK(%lu)\n\n",m->len_black);
	// printBlocks(m->black,m->len_black,4);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* close_feature(PyObject* self, PyObject* args){
	FeatureMask* fm = &__c_hypothesy_cursor[0].fm;
	
	fm->__original_mask = fm->mask;
	fm->__original_location = fm->location;
	fm->__original_size = fm->mask.size;	
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* close_hypothesis(PyObject* self, PyObject* args){
	__c_hypothesy_cursor++;

	Py_INCREF(Py_None);
	return Py_None;
}

void saveClassifier(Classifier cl,char* filename){
	FILE* f = fopen(filename,"wb");
	fwrite(&__conv_cl.final,sizeof(ulong),1,f);
	fwrite(&__conv_cl.ardis,sizeof(Point),1,f);
	fwrite(&__conv_cl.len_hypothesis,sizeof(ulong),1,f);

	int i,j;
	for(i=0;i<__conv_cl.len_hypothesis;i++){
		Hypothesy h = __conv_cl.hypothesis[i];
		fwrite(&h.threshold,sizeof(ulong),1,f);
		fwrite(&h.direction,sizeof(long),1,f);
		fwrite(&h.alpha,sizeof(double),1,f);
		
		fwrite(&h.fm.__original_location,sizeof(Point),1,f);
		fwrite(&h.fm.__original_size,sizeof(Point),1,f);
		fwrite(&h.fm.location,sizeof(Point),1,f);
		
		fwrite(&h.fm.mask.maskType,sizeof(int),1,f);		
		fwrite(&h.fm.mask.size,sizeof(Point),1,f);		

		fwrite(&h.fm.mask.len_white,sizeof(ulong),1,f);
		for(j=0;j<h.fm.mask.len_white;j++){
			fwrite(&h.fm.mask.white[j].points,sizeof(Point),4,f);
			fwrite(&h.fm.mask.white[j].w,sizeof(ulong),1,f);
			fwrite(&h.fm.mask.white[j].h,sizeof(ulong),1,f);
		}

		fwrite(&h.fm.mask.len_black,sizeof(ulong),1,f);
		for(j=0;j<h.fm.mask.len_black;j++){
			fwrite(&h.fm.mask.black[j].points,sizeof(Point),4,f);
			fwrite(&h.fm.mask.black[j].w,sizeof(ulong),1,f);
			fwrite(&h.fm.mask.black[j].h,sizeof(ulong),1,f);
		}		
	}

	fclose(f);
}

static PyObject* close_classifier(PyObject* self, PyObject* args){
	char* filename;
	pyToStr( PyTuple_GetItem(args,0), &filename );

	saveClassifier(__conv_cl,filename);

	Py_INCREF(Py_None);
	return Py_None;
}


static PyMethodDef CConverterMethods[] ={ 
	{"init",init,METH_VARARGS,"Init Classifier"},
	{"init_hypothesis",init_hypothesis,METH_VARARGS,"Init Hypothesis"},
	{"init_feature",init_feature,METH_VARARGS,"Init Feature"},
	{"init_mask",init_mask,METH_VARARGS,"Init init_mask"},
	{"add_block",add_block,METH_VARARGS,"Add Block"},
	{"close_mask",close_mask,METH_VARARGS,"Close Mask"},
	{"close_feature",close_feature,METH_VARARGS,"Close Feature"},
	{"close_hypothesis",close_hypothesis,METH_VARARGS,"Close Hypothesis"},
	{"close_classifier",close_classifier,METH_VARARGS,"Close Classifier"},
	{NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initc_converter(void){
	(void) Py_InitModule("c_converter",CConverterMethods);
};

int main(int argc, char* argv[]){
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	initc_converter();
};