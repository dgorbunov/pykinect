#include "../pyfreenect2.hpp"
#include <iostream>

using libfreenect2::Freenect2Device;
using libfreenect2::FrameListener;

PyObject *py_Freenect2Device_new(PyObject *self, PyObject *args) {
	char *serialNumber = NULL;
	unsigned int pipeline_enum = 0;
	if(!PyArg_ParseTuple(args, "sI", &serialNumber, &pipeline_enum))
		return NULL;
    libfreenect2::PacketPipeline *pipeline = 0;
	if(pipeline_enum == 0){
        pipeline = new libfreenect2::CpuPacketPipeline();
	}
	//else if(pipeline_enum == 1){
	    //pipeline = new libfreenect2::OpenCLPacketPipeline();
	//}
	else{
        pipeline = new libfreenect2::OpenGLPacketPipeline();
	}
	Freenect2Device *device = getGlobalFreenect2().openDevice(serialNumber, pipeline);
	return PyCapsule_New(device, "Freenect2Device", py_Freenect2Device_destroy);
}
void py_Freenect2Device_destroy(PyObject *deviceCapsule) {
	((Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device"))->close();
}

PyObject *py_Freenect2Device_start(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	bool success = ((Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device"))->start();
	if (success){
	    Py_RETURN_TRUE;
	}

	Py_RETURN_FALSE;
}
PyObject *py_Freenect2Device_stop(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	((Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device"))->stop();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_Freenect2Device_setColorFrameListener(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	PyObject *listenerCapsule = NULL;
	if(!PyArg_ParseTuple(args, "OO", &deviceCapsule, &listenerCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");

	Py_INCREF(listenerCapsule);
	FrameListener *listener = (FrameListener*) PyCapsule_GetPointer(listenerCapsule, "SyncMultiFrameListener");
	device->setColorFrameListener(listener);
	Py_INCREF(Py_None);
	return Py_None;
}
PyObject *py_Freenect2Device_setIrAndDepthFrameListener(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	PyObject *listenerCapsule = NULL;
	if(!PyArg_ParseTuple(args, "OO", &deviceCapsule, &listenerCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");
	Py_INCREF(listenerCapsule);
	FrameListener *listener = (FrameListener*) PyCapsule_GetPointer(listenerCapsule, "SyncMultiFrameListener");
	device->setIrAndDepthFrameListener(listener);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_Freenect2Device_getSerialNumber(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");
	std::string serialNumber = device->getSerialNumber();
	return PyBytes_FromString(serialNumber.c_str());
}
PyObject *py_Freenect2Device_getFirmwareVersion(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");
	std::string firmwareVersion = device->getFirmwareVersion();
	return PyBytes_FromString(firmwareVersion.c_str());
}

PyObject *py_Freenect2Device_getColorCameraParams(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");
	Freenect2Device::ColorCameraParams colorCameraParams = device->getColorCameraParams();
	return PyCapsule_New(&colorCameraParams, "ColorCameraParams", py_Freenect2Device_ColorCameraParams_destroy);
}
PyObject *py_Freenect2Device_getIRCameraParams(PyObject *self, PyObject *args) {
	PyObject *deviceCapsule = NULL;
	if(!PyArg_ParseTuple(args, "O", &deviceCapsule))
		return NULL;
	Freenect2Device *device = (Freenect2Device*) PyCapsule_GetPointer(deviceCapsule, "Freenect2Device");
	Freenect2Device::IrCameraParams irCameraParams = device->getIrCameraParams();
	return PyCapsule_New(&irCameraParams, "IRCameraParams", py_Freenect2Device_IRCameraParams_destroy);
}

void py_Freenect2Device_ColorCameraParams_destroy(PyObject *colorCameraParamsCapsule) {
	//delete ((Freenect2Device::ColorCameraParams*) PyCapsule_GetPointer(colorCameraParamsCapsule, "ColorCameraParams"));
}
void py_Freenect2Device_IRCameraParams_destroy(PyObject *irCameraParamsCapsule) {
	//delete ((Freenect2Device::IrCameraParams*) PyCapsule_GetPointer(irCameraParamsCapsule, "IRCameraParams"));
}