/** \file
 * Pixel format conversion header file. 
 *
 * $Id: ConvertIm.hpp 1654 2010-09-07 08:53:08Z rick $
 */

#ifndef CONVERTIM_HPP
#define CONVERTIM_HPP

#include "Compat.hpp"
#include "Image.hpp"

namespace Vi
{
  /**
   * Convert from one pixel format to another pixel format.
   *
   * \param[out] fDst Destination image.
   * \param[in]  fSrc Source image.
   *
   * Currently supported conversions:
   * - PF_YCC444P to PF_YCC420P
   * - PF_YCC444P to PF_RGB
   * - PF_YCC444P to PF_RGBA
   * - PF_YCC444P to PF_JFIF
   * - PF_YCC444P to PF_Y
   * - PF_YCC420P to PF_YCC444P
   * - PF_RGB     to PF_YCC444P
   * - PF_JFIF    to PF_YCC444P
   * - PF_Y       to PF_YCC444P
   *
   * \todo Add fallback conversions if direct conversion is not
   *  supported. For example if conversion from PF_x to PF_y is not
   *  supported: first convert to Vi::PF_x to Vi::PF_RGB, than convert
   *  Vi::PF_RGB to Vi::PF_y.
   */
  void EXPORT_TYPE convertIm(Vi::Image<> &fDst, const Vi::Image<> &fSrc);
}

#endif // CONVERTIM_HPP
