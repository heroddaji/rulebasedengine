/** \file
 * Image flipping function prototypes.
 *
 * $Id: ImageFlip.hpp 1655 2010-09-07 09:21:08Z rick $
 */

#ifndef IMAGEFLIP_HPP
#define IMAGEFLIP_HPP

#include "Image.hpp"
#include "Compat.hpp"

namespace Vi
{
  /**
   * Horizontal flipping (around vertical axis).
   *
   * \param[out]  fOutImg Output image (flipped).
   * \param[in]   fInImg Input image.
   */
  void EXPORT_TYPE flipHor(Vi::Image<> &fOutImg, const Vi::Image<> &fInImg);

  /**
   * Horizontal flipping (around vertical axis).
   *
   * \param[in,out] fImg In-/output image.
   */
  void EXPORT_TYPE flipHor(Vi::Image<> &fImg);

  /**
   * Vertical flipping (around horizontal axis).
   *
   * \param[out]  fOutImg Output image (flipped).
   * \param[in]   fInImg Input image.
   */
  void EXPORT_TYPE flipVer(Vi::Image<> &fOutImg, const Vi::Image<> &fInImg);

  /**
   * Vertical flipping (around horizontal axis).
   *
   * \param[in,out] fImg In-/output image.
   */
  void EXPORT_TYPE flipVer(Vi::Image<> &fImg);
}

#endif // IMAGEFLIP_HPP
