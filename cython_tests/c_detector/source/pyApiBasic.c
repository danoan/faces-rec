#include "../headers/pyApiBasic.h"

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
