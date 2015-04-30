/** \file
 * JPEG-file I/O header file.
 *
 * $Id: Jpeg.hpp 1690 2010-09-13 09:36:08Z kris $
 */

#ifndef JPEG_HPP
#define JPEG_HPP

#include <string>

#include "Image.hpp"

namespace Vi
{
  /**
   * Read from a JPEG image file.
   *
   * \param[out] fImage    Destination image.
   * \param[in]  fFilename File name of source file.
   */
  void readJpegImage(Vi::Image<> &fImage, const std::string &fFilename);

  /**
   * Write image to a JPEG image file
   *
   * \param[in] fFilename File name of destination file.
   * \param[in] fImage    Source image.
   */
  void writeJpegImage(const std::string &fFilename, const Vi::Image<> &fImage);
}

#endif // JPEG_HPP
