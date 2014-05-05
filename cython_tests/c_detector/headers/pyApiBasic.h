#include <Python.h>

int pyToInt(PyObject* py, ulong* a);
int pyToStr(PyObject* py, char** a);
PyObject* pointerToPy(ulong* p,ulong size);
