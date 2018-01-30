import _pyfreenect2
import numpy as np
from collections import namedtuple
from enum import Enum

ExtractedKinectFrame = namedtuple("ExtractedKinectFrame",
                                  ['RGB', 'BGR', 'IR', 'DEPTH'])


def swap_c0c2(a):
    a2 = a.copy()
    a2[:, :, 0] = a[:, :, 2]
    a2[:, :, 2] = a[:, :, 0]
    return a2


class Pipeline(Enum):
    Cpu = 0
    OpenCL = 1
    OpenGL = 2


class PyFreenect2Error(Exception):
    def __init__(self, message):
        super(PyFreenect2Error, self).__init__(message)


################################################################################
#                               GLOBAL FUNCTIONS                               #
################################################################################

def numberOfDevices():
    return _pyfreenect2.numberOfDevices()


def getDefaultDeviceSerialNumber():
    if numberOfDevices() == 0:
        raise PyFreenect2Error("Could not find a Kinect v2")
    else:
        return _pyfreenect2.getDefaultDeviceSerialNumber()


################################################################################
#                                Freenect2Device                               #
################################################################################

class Freenect2Device:
    def __init__(self, serialNumber, pipeline=Pipeline.OpenGL):
        if not isinstance(pipeline, Pipeline):
            raise RuntimeError("pipeline should be of enum class Pipeline, not {}".format(type(pipeline)))
        self._capsule = _pyfreenect2.Freenect2Device_new(serialNumber, pipeline.value)

    def start(self):
        return _pyfreenect2.Freenect2Device_start(self._capsule)

    def stop(self):
        _pyfreenect2.Freenect2Device_stop(self._capsule)

    def setColorFrameListener(self, listener):
        if not isinstance(listener, SyncMultiFrameListener):
            raise TypeError(
                "Argument to Freenect2Device.setColorFrameListener must be of type Freenect2Device.SyncMultiFrameListener")
        else:
            print("listener capsule : ", listener._capsule)
            _pyfreenect2.Freenect2Device_setColorFrameListener(self._capsule, listener._capsule)

    def setIrAndDepthFrameListener(self, listener):
        if not isinstance(listener, SyncMultiFrameListener):
            raise TypeError(
                "Argument to Freenect2Device.setIrAndDepthFrameListener must be of type Freenect2Device.SyncMultiFrameListener")
        else:
            _pyfreenect2.Freenect2Device_setIrAndDepthFrameListener(self._capsule, listener._capsule)

    @property
    def serial_number(self):
        return _pyfreenect2.Freenect2Device_getSerialNumber(self._capsule)

    @property
    def firmware_version(self):
        return _pyfreenect2.Freenect2Device_getFirmwareVersion(self._capsule)

    @property
    def color_camera_params(self):
        return _pyfreenect2.Freenect2Device_getColorCameraParams(self._capsule)

    @property
    def ir_camera_params(self):
        return _pyfreenect2.Freenect2Device_getIRCameraParams(self._capsule)


################################################################################
#                            SyncMultiFrameListener                            #
################################################################################

class SyncMultiFrameListener:
    def __init__(self, *args):
        types = 0
        for arg in args:
            types |= int(arg)
        self._capsule = _pyfreenect2.SyncMultiFrameListener_new(types)

    def waitForNewFrame(self):
        return FrameMap(_pyfreenect2.SyncMultiFrameListener_waitForNewFrame(self._capsule))

    def release(self, frames):
        _pyfreenect2.SyncMultiFrameListener_release(self._capsule, frames._capsule)


################################################################################
#                                   FrameMap                                   #
################################################################################

class FrameMap:
    def __init__(self, capsule):
        self._capsule = capsule

    def getFrame(self, frame_type):
        if not frame_type in (1, 2, 4):
            raise ValueError("frame_type must be one of Frame.COLOR, Frame.IR, or Frame.DEPTH")
        else:
            return Frame(_pyfreenect2.FrameMap_getFrame(self._capsule, frame_type))


################################################################################
#                                     Frame                                    #
################################################################################

class Frame:
    COLOR = 1
    IR = 2
    DEPTH = 4

    def __init__(self, capsule):
        self._capsule = capsule

    def getHeight(self):
        return _pyfreenect2.Frame_getHeight(self._capsule)

    def getWidth(self):
        return _pyfreenect2.Frame_getWidth(self._capsule)

    def getTimestamp(self):
        return _pyfreenect2.Frame_getTimestamp(self._capsule)

    def getRGBData(self):
        ## todo fix fliplr necessity
        ## todo fix BGR :/
        BGRA = np.fliplr(_pyfreenect2.Frame_getData(self._capsule))
        return BGRA

    def getDepthData(self):
        ## todo fix fliplr necessity
        return np.fliplr(_pyfreenect2.Frame_getDepthData(self._capsule))


################################################################################
#                                 Registration                                 #
################################################################################

class Registration:
    def __init__(self, device):
        self._capsule = _pyfreenect2.Registration_new(device._capsule)

    def apply(self, rgbFrame, depthFrame):
        rgb_capsule = rgbFrame._capsule
        depth_capsule = depthFrame._capsule
        (_undist, _reg, _bd) = _pyfreenect2.Registration_apply(self._capsule, rgb_capsule, depth_capsule)
        return (Frame(_undist), Frame(_reg), Frame(_bd))
