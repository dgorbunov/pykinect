#ifndef _SMARTFRAM_H_
#define _SMARTFRAM_H_

#include "../pyfreenect2.hpp"
#include "SmartPtr.hpp"

//Credit to wangqiang1588 https://github.com/remexre/pyfreenect2/commit/0799e55d9f3acc16731253db64436a7a0d07a041#diff-25d902c24283ab8cfbac54dfa101ad31

using libfreenect2::Frame;
using libfreenect2::FrameMap;

typedef SmartPtr<Frame> SPFrame;
typedef std::map<Frame::Type, SPFrame> SPFrameMap;

SPFrameMap* getSPFrameMapFromFrameMap(FrameMap* frames);

#endif