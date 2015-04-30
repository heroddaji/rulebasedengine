/** \file
 * Wrapper around OpenCV scaler (prototype).
 *
 * $Id: ScalerOpenCV.hpp 2030 2011-02-04 16:04:06Z kris $
 */

#ifndef SCALER_OPENCV_HPP
#define SCALER_OPENCV_HPP

#include <ViNotion/Image.hpp>

namespace Vi
{
  /**
   * Scale an image.
   *
   * \param[in]   fInputImage The input image.
   * \param[out] fOutputImage The output image (with output size set!).
   */
  void scalerOpenCV(Vi::Image<> &fInputImage, Vi::Image<> &fOutputImage);
}

#endif // SCALER_OPENCV_HPP
