#include "../pyfreenect2.hpp"
#include <iostream>

using libfreenect2::FrameMap;
using libfreenect2::SyncMultiFrameListener;

FrameMap g_frameMap;

void py_SyncMultiFrameListener_destroy(PyObject *listenerCapsule) {
	delete ((SyncMultiFrameListener*) PyCapsule_GetPointer(listenerCapsule, 
							       "SyncMultiFrameListener"));
}

PyObject *py_SyncMultiFrameListener_new(PyObject *self, PyObject *args) {
	unsigned int frame_types = 0;
	if(!PyArg_ParseTuple(args, "I", &frame_types))
		return NULL;
	SyncMultiFrameListener *listener = new SyncMultiFrameListener(frame_types);
	std::cout << "listener in ext: " << listener << std::endl;
	return PyCapsule_New(listener, 
			     "SyncMultiFrameListener", 
			     py_SyncMultiFrameListener_destroy);
}

PyObject *py_SyncMultiFrameListener_waitForNewFrame(PyObject *self, PyObject *args) {
	PyObject *listenerCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &listenerCapsule))
		return NULL;
	SyncMultiFrameListener *listener = (SyncMultiFrameListener*) PyCapsule_GetPointer(listenerCapsule, "SyncMultiFrameListener");
	listener->waitForNewFrame(g_frameMap);
	return PyCapsule_New(&g_frameMap, "FrameMap", py_FrameMap_destroy);
}

PyObject *py_SyncMultiFrameListener_release(PyObject *self, PyObject *args) {
    PyObject *listenerCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &listenerCapsule))
		return NULL;
	SyncMultiFrameListener *listener = (SyncMultiFrameListener*) PyCapsule_GetPointer(listenerCapsule, "SyncMultiFrameListener");
	listener->release(g_frameMap);
    Py_INCREF(Py_None);
	return Py_None;

}
