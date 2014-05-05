#ifndef __PY_TO_C_CLASSIFIER
#define __PY_TO_C_CLASSIFIER

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include "c_basic.h"
#include "pyApiBasic.h"
#include "c_loader.h"

#define NDEBUG
#include <assert.h>

static PyObject* init(PyObject* self, PyObject* args);
static PyObject* init_hypothesis(PyObject* self, PyObject* args);
static PyObject* init_mask(PyObject* self, PyObject* args);
static PyObject* add_block(PyObject* self, PyObject* args);
static PyObject* close_mask(PyObject* self, PyObject* args);
static PyObject* close_feature(PyObject* self, PyObject* args);
static PyObject* close_hypothesis(PyObject* self, PyObject* args);
static PyObject* close_classifier(PyObject* self, PyObject* args);

static PyObject* load_classifier(PyObject* self, PyObject* args);
static PyObject* print_classifier(PyObject* self, PyObject* args);
static PyObject* get_attribute(PyObject* self, PyObject* args);


#endif