/** \file
 * Image file I/O header file.
 *
 * $Id: ImageFile.hpp 1641 2010-09-03 13:48:44Z rick $
 */

#ifndef IMAGE_FILE_HPP
#define IMAGE_FILE_HPP

#include <string>

#include "Image.hpp"
#include "Compat.hpp"

namespace Vi
{
  /**
   * Read from an image file. The file format is determined using the
   *  extension of the filename. Currently, this function supports the
   *  PGM, PPM, JPEG and PNG formats.
   *
   * \param[out] fImage    Destination image.
   * \param[in]  fFilename File name of source file.
   */
  void EXPORT_TYPE readImage(Vi::Image<uint8_t> &fImage, const std::string &fFilename);

  /**
   * Write image to an image file. The file format is determined using
   *  the extension of the filename. Currently, this function supports
   *  the PGM, PPM, JPEG and PNG formats.
   *
   * \param[in] fFilename File name of destination file.
   * \param[in] fImage    Source image.
   */
  void EXPORT_TYPE writeImage(const std::string &fFilename, const Vi::Image<uint8_t> &fImage);
}

#endif // IMAGE_FILE_HPP
