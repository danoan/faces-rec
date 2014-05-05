#include <Python.h>
#include <sys/time.h>
#include <wand/MagickWand.h>

#define NDEBUG
#include <assert.h>


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

static PyObject* integral(PyObject* self, PyObject* args){
	char* cl_filename;
	char* img_filename;

	pyToStr( PyTuple_GetIem(args,0), &cl_filename ); 
	pyToStr( PyTuple_GetIem(args,1), &img_filename );

	Classifier cl = loadClassifier(cl_filename);
	Detector d;
	
	Point wr;
	wr.x=8;
	wr.y=8;

	Point ref_mask;
	ref_mask.x=64;
	ref_mask.y=64;

	newDetector(&d,cl,5, wr, ref_mask, 1, 0.5);
	detectFaces(&d,img_filename);

	Py_INCREF(Py_None);
	return Py_None;
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