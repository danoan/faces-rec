#ifndef __PY_API_BASIC__
#define __PY_API_BASIC__

#include <Python.h>

int pyToInt(PyObject* py, ulong* a);
int pyToStr(PyObject* py, char** a);
PyObject* pointerToPy(ulong* p,ulong size);

#endif