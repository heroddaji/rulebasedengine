/** \file
 * Header file for the webcam video source class.
 *
 * $Id: VideoInputCapture.hpp 1568 2010-08-06 17:22:26Z kris $
 */

#ifndef VIDEO_INPUT_CAPTURE_HPP
#define VIDEO_INPUT_CAPTURE_HPP

/// Select a video input derived class based on the operating system.
#if defined _WIN32
#include "VideoInputCaptureDShow.hpp"
#elif defined linux
#include "VideoInputCaptureV4L.hpp"
#else
#error No webcam driver available for this operating system.
#endif

#endif // VIDEO_INPUT_CAPTURE_HPP
