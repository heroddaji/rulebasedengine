/** \file
 * PNM format file I/O function prototypes.
 *
 * $Id: Pnm.hpp 1650 2010-09-07 07:08:54Z rick $
 */

#ifndef PNM_HPP
#define PNM_HPP

#include <string>

#include "Image.hpp"

namespace Vi
{
  // http://netpbm.sourceforge.net/doc/pgm.html

  /**
   * Read image from a PGM image file.
   *
   * \param[out] fImage    Destination image.
   * \param[in]  fFilename File name of source file.
   */
  void readPgmImage(Vi::Image<> &fImage, const std::string &fFilename);

  /**
   * Write image to a PGM image file.
   *
   * \param[in] fFilename File name of destination file.
   * \param[in] fImage    Source image.
   */
  void writePgmImage(const std::string &fFilename, const Vi::Image<> &fImage);

  // http://netpbm.sourceforge.net/doc/ppm.html

  /**
   * Read image from a PPM image file.
   *
   * \param[out] fImage    Destination image.
   * \param[in]  fFilename File name of source file.
   */
  void readPpmImage(Vi::Image<> &fImage, const std::string &fFilename);

  /**
   * Write image to a PPM image file.
   *
   * \param[in] fFilename File name of destination file.
   * \param[in] fImage    Source image.
   */
  void writePpmImage(const std::string &fFilename, const Vi::Image<> &fImage);
}

#endif // PNM_HPP
