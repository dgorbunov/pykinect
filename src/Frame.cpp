#include "../pyfreenect2.hpp"
#include <iostream>
#include "SmartFrame.h"


using libfreenect2::Frame;

void py_Frame_destroy(PyObject *frameCapsule) {
	delete ((SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame"));
}

PyObject *py_Frame_getHeight(PyObject *self, PyObject *args) {
	PyObject *frameCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &frameCapsule))
		return NULL;
	SPFrame *spFrame = (SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame");
	Frame *frame = spFrame->acquire();
	int height = frame->height;
	spFrame->release();
	return PyLong_FromLong(height);
}

PyObject *py_Frame_getWidth(PyObject *self, PyObject *args) {
	PyObject *frameCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &frameCapsule))
		return NULL;
	SPFrame *spFrame = (SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame");
	Frame *frame = spFrame->acquire();
	int width = frame->width;
	spFrame->release();
	return PyLong_FromLong(width);
}

PyObject *py_Frame_getData(PyObject *self, PyObject *args) {


	PyObject *frameCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &frameCapsule))
		return NULL;
	SPFrame *spFrame = (SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame");
	Frame *frame = spFrame->acquire();

	// frames are apparently 4 channel (4 bytes per pixel)
	npy_intp dims[] = {frame->height, frame->width, 4 };

	// this should be elsewhere, however, fails without it.
	import_array();

	PyArrayObject *array = (PyArrayObject*) PyArray_SimpleNewFromData(3, 
									  dims, 
									  NPY_UINT8,
									  frame->data);
    spFrame->release();
	return (PyObject*) array;
}

PyObject *py_Frame_getTimestamp(PyObject *self, PyObject *args) {

	PyObject *frameCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &frameCapsule))
		return NULL;
	SPFrame *spFrame = (SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame");
	Frame *frame = spFrame->acquire();

	int timestamp = frame->timestamp;
	spFrame->release();
	return PyLong_FromLong(timestamp);
}

PyObject *py_Frame_getDepthData(PyObject *self, PyObject *args)
{

	PyObject *frameCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &frameCapsule))
		return NULL;
	SPFrame *spFrame = (SPFrame*) PyCapsule_GetPointer(frameCapsule, "Frame");
	Frame *frame = spFrame->acquire();

	npy_intp dims[] = {frame->height, frame->width, 2};

	import_array();

	PyArrayObject *array = (PyArrayObject*) PyArray_SimpleNewFromData(2,
									  dims, 
									  NPY_FLOAT,
									  frame->data);
	spFrame->release();
	return (PyObject*) array;
}
