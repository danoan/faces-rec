#include "../../headers/pyToC/pyToCClassifier.h"

C_Block* __c_white_cursor;
C_Block* __c_black_cursor;
C_Hypothesy* __c_hypothesy_cursor;
C_Classifier __conv_cl;

C_Classifier __loader_cl;

ulong c_len_white_blocks;
ulong c_len_black_blocks;

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
	__conv_cl.hypothesis = (C_Hypothesy*) malloc(sizeof(C_Hypothesy)*__conv_cl.len_hypothesis);

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

	C_FeatureMask* fm = &(__c_hypothesy_cursor[0].fm);

	pyToInt( PyTuple_GetItem(py_location,0),&(fm->location.y) );
	pyToInt( PyTuple_GetItem(py_location,1), &(fm->location.x) );

	Py_DECREF(py_location);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_mask(PyObject* self, PyObject* args){
	char* factoryName;
	C_Mask* m = &(__c_hypothesy_cursor[0].fm.mask);

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

	m->white = (C_Block*) malloc(sizeof(C_Block)*m->len_white);
	m->black = (C_Block*) malloc(sizeof(C_Block)*m->len_black);	

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

	C_Block** block_cursor;
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
	C_Mask* m = &(__c_hypothesy_cursor[0].fm.mask);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* close_feature(PyObject* self, PyObject* args){
	C_FeatureMask* fm = &__c_hypothesy_cursor[0].fm;
	
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

void saveC_Classifier(C_Classifier cl,char* filename){
	FILE* f = fopen(filename,"wb");
	fwrite(&__conv_cl.final,sizeof(ulong),1,f);
	fwrite(&__conv_cl.ardis,sizeof(C_Point),1,f);
	fwrite(&__conv_cl.len_hypothesis,sizeof(ulong),1,f);

	int i,j;
	for(i=0;i<__conv_cl.len_hypothesis;i++){
		C_Hypothesy h = __conv_cl.hypothesis[i];
		fwrite(&h.threshold,sizeof(ulong),1,f);
		fwrite(&h.direction,sizeof(long),1,f);
		fwrite(&h.alpha,sizeof(double),1,f);
		
		fwrite(&h.fm.__original_location,sizeof(C_Point),1,f);
		fwrite(&h.fm.__original_size,sizeof(C_Point),1,f);
		fwrite(&h.fm.location,sizeof(C_Point),1,f);
		
		fwrite(&h.fm.mask.maskType,sizeof(int),1,f);		
		fwrite(&h.fm.mask.size,sizeof(C_Point),1,f);		

		fwrite(&h.fm.mask.len_white,sizeof(ulong),1,f);
		for(j=0;j<h.fm.mask.len_white;j++){
			fwrite(&h.fm.mask.white[j].points,sizeof(C_Point),4,f);
			fwrite(&h.fm.mask.white[j].w,sizeof(ulong),1,f);
			fwrite(&h.fm.mask.white[j].h,sizeof(ulong),1,f);
		}

		fwrite(&h.fm.mask.len_black,sizeof(ulong),1,f);
		for(j=0;j<h.fm.mask.len_black;j++){
			fwrite(&h.fm.mask.black[j].points,sizeof(C_Point),4,f);
			fwrite(&h.fm.mask.black[j].w,sizeof(ulong),1,f);
			fwrite(&h.fm.mask.black[j].h,sizeof(ulong),1,f);
		}		
	}

	fclose(f);
}

static PyObject* close_classifier(PyObject* self, PyObject* args){
	char* filename;
	pyToStr( PyTuple_GetItem(args,0), &filename );

	saveC_Classifier(__conv_cl,filename);

	Py_INCREF(Py_None);
	return Py_None;
}



static PyObject* load_classifier(PyObject* self, PyObject* args){
	char* filename;
	pyToStr( PyTuple_GetItem(args,0),&filename );

	__loader_cl = loadC_Classifier(filename);	

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* print_classifier(PyObject* self, PyObject* args){
	printC_Classifier(__loader_cl);

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
		C_Block bw = __loader_cl.hypothesis[hi].fm.mask.white[bi];
		ulong points[8] = {bw.points[0].y,bw.points[0].x,bw.points[1].y,bw.points[1].x,bw.points[2].y,bw.points[2].x,bw.points[3].y,bw.points[3].x};
		
		return Py_BuildValue("O", pointerToPy(points,8));

	}else if(strcmp(attr,"WHITE_W")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.white[bi].w);
	}else if(strcmp(attr,"WHITE_H")==0){
		return Py_BuildValue("k",__loader_cl.hypothesis[hi].fm.mask.white[bi].h);
	}else if(strcmp(attr,"BLACK_POINTS")==0){
		C_Block bb = __loader_cl.hypothesis[hi].fm.mask.black[bi];
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


static PyMethodDef PyTOCClassifierMethods[] ={ 
	{"init",init,METH_VARARGS,"Init Classifier"},
	{"init_hypothesis",init_hypothesis,METH_VARARGS,"Init Hypothesis"},
	{"init_feature",init_feature,METH_VARARGS,"Init Feature"},
	{"init_mask",init_mask,METH_VARARGS,"Init init_mask"},
	{"add_block",add_block,METH_VARARGS,"Add Block"},
	{"close_mask",close_mask,METH_VARARGS,"Close Mask"},
	{"close_feature",close_feature,METH_VARARGS,"Close Feature"},
	{"close_hypothesis",close_hypothesis,METH_VARARGS,"Close Hypothesis"},
	{"close_classifier",close_classifier,METH_VARARGS,"Close Classifier"},
	{"load_classifier",load_classifier,METH_VARARGS,"Load Classifier"},
	{"get_attribute",get_attribute,METH_VARARGS,"Get Attibute"},
	{"print_classifier",print_classifier,METH_VARARGS,"Print Classifier"},	
	{NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initpyToCClassifier(void){
	(void) Py_InitModule("pyToCClassifier",PyTOCClassifierMethods);
};

int main(int argc, char* argv[]){
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	initpyToCClassifier();
};