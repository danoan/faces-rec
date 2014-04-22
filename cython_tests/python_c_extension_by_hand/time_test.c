#include <Python.h>
#include <sys/time.h>

static PyObject* timetest(PyObject* self, PyObject* args){
	int i;
	double c = 0;
	double ft;

	struct timeval stop,start;

	gettimeofday(&start,NULL);
	for(i=0;i<500000;i++){
		c += (i*3)/2.0;
	}
	gettimeofday(&stop,NULL);

	ft = (stop.tv_sec-start.tv_sec) + (stop.tv_usec-start.tv_usec)*1e-6;

	printf("%lf\n", ft);
	printf("%lu\n", (stop.tv_usec-start.tv_usec));
	printf("%lf\n",c);	

	return Py_None;
};

static PyMethodDef IntegralImageMethods[] ={ 
	{"timetest",timetest,METH_VARARGS,"Time"},
	{NULL,NULL,0,NULL}
};

PyMODINIT_FUNC inittime_test(void){
	(void) Py_InitModule("time_test",IntegralImageMethods);
};

int main(int argc, char* argv[]){
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	inittime_test();
};