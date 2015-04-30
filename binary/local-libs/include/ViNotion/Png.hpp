/** \file
 * PNG-file I/O header file.
 *
 * $Id: Png.hpp 1640 2010-09-02 15:24:27Z rick $
 */

#ifndef PNG_HPP
#define PNG_HPP

#include <string>

#include "Image.hpp"

namespace Vi
{
  /**
   * Read from a PNG image file.
   *
   * \param[out] fImage    Destination image.
   * \param[in]  fFilename File name of source file.
   */
  void readPngImage(Vi::Image<> &fImage, const std::string &fFilename);

  /**
   * Write image to a PNG image file.
   *
   * \param[in] fFilename File name of destination file.
   * \param[in] fImage    Source image.
   */
  void writePngImage(const std::string &fFilename, const Vi::Image<> &fImage);
}

#endif // PNG_HPP
