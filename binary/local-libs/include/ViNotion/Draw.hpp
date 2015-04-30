/** \file
 * Drawing routine prototypes.
 *
 * $Id: Draw.hpp 1864 2010-11-18 10:07:25Z kris $
 */

#ifndef DRAW_HPP
#define DRAW_HPP

#include <cmath>

#include "Compat.hpp"
#include "Misc.hpp"
#include "Image.hpp"
#include "Bbox.hpp"
#include "Vector2D.hpp"

namespace Vi
{
  /// YCbCr color space color.
  struct YCCColor
  {
    uint8_t Y;
    uint8_t Cb;
    uint8_t Cr;
  };

  /// RGB color space color.
  struct RGBColor
  {
    uint8_t R;
    uint8_t G;
    uint8_t B;
  };

  /// Jet color map from MatLab.
  const uint8_t jetMapY[100] =
  {
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 46, 51, 56, 62, 67, 72, 77,
    82, 87, 92, 98,103,108,113,118,123,128,134,139,144,149,154,159,164,170,171,173,
    174,176,178,179,181,182,184,186,187,189,191,192,194,195,197,199,200,202,204,205,
    207,208,210,205,200,195,189,184,179,174,169,164,159,153,148,143,138,133,128,123,
    117,112,107,102, 97, 92, 87, 81, 79, 76, 74, 71, 68, 66, 63, 61, 58, 55, 53, 50
  };

  /// Jet color map from MatLab.
  const uint8_t jetMapCb[100] =
  {
    186,191,195,200,204,209,213,218,222,227,231,236,240,237,234,231,228,225,222,219,
    216,213,210,207,204,201,198,195,193,190,187,184,181,178,175,172,169,166,160,154,
    148,142,136,130,124,118,112,106,100, 94, 88, 82, 76, 70, 64, 58, 52, 46, 40, 34,
    28, 22, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 49, 52, 55, 58, 61, 63, 66,
    69, 72, 75, 78, 81, 84, 87, 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,108
  };

  /// Jet color map from MatLab.
  const uint8_t jetMapCr[100] =
  {
    119,118,117,116,116,115,114,113,113,112,111,111,110,106,102, 99, 95, 91, 87, 84,
    80, 76, 72, 69, 65, 61, 57, 54, 50, 46, 42, 39, 35, 31, 27, 24, 20, 16, 21, 26,
    32, 37, 42, 47, 52, 58, 63, 68, 73, 79, 84, 89, 94, 99,105,110,115,120,125,131,
    136,141,146,150,154,157,161,165,169,172,176,180,184,187,191,195,199,202,206,210,
    214,217,221,225,229,232,236,240,236,231,227,222,218,213,209,204,200,195,191,186
  };

  const YCCColor YCC_GREEN   = {145,54,34};
  const YCCColor YCC_BLUE    = {41,240,110};
  const YCCColor YCC_RED     = {81,90,240};
  const YCCColor YCC_YELLOW  = {210,16,146};
  const YCCColor YCC_MAGENTA = {106,202,222};
  const YCCColor YCC_CYAN    = {170,166,16};
  const YCCColor YCC_WHITE   = {235,128,128};
  const YCCColor YCC_GRAY    = {128,128,128};
  const YCCColor YCC_BLACK   = {16,128,128};
  const YCCColor YCC_PURPLE  = {75,195,181};
  const YCCColor YCC_ORANGE  = {133,61,203};
  // Reservoir dogs.
  const YCCColor YCC_BLONDE  = {210,16,146};
  const YCCColor YCC_PINK    = {179,135,169};
  const YCCColor YCC_BROWN   = {73,120,150};

  const RGBColor RGB_GREEN   = {0,255,0};
  const RGBColor RGB_BLUE    = {0,0,255};
  const RGBColor RGB_RED     = {255,0,0};
  const RGBColor RGB_YELLOW  = {255,255,0};
  const RGBColor RGB_MAGENTA = {255,0,255};
  const RGBColor RGB_CYAN    = {0,255,255};
  const RGBColor RGB_WHITE   = {255,255,255};
  const RGBColor RGB_BLACK   = {0,0,0};
  const RGBColor RGB_PURPLE  = {153,0,204};
  const RGBColor RGB_ORANGE  = {255,102,0};
  const RGBColor RGB_PINK    = {255,153,204};
  const RGBColor RGB_BROWN   = {102,51,51};
  const RGBColor RGB_GRAY    = {128,128,128};

  /**
   * Bresenham line drawing algorithm, modified from
   *  http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm .
   *
   * \param[in,out] fImg The in-/output image.
   * \param[in]     fX0 The start point X-coordinate of the line.
   * \param[in]     fY0 The start point Y-coordinate of the line.
   * \param[in]     fX1 The start point X-coordinate of the line.
   * \param[in]     fY1 The start point Y-coordinate of the line.
   * \param[in]     fCol The color of the line.
   */
  void EXPORT_TYPE drawLine(Vi::Image<uint8_t> &fImg,
                            int fX0,
                            int fY0,
                            int fX1,
                            int fY1,
                            Vi::YCCColor fCol);

  /**
   * Bresenham line drawing algorithm, modified from
   *  http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   *  Overloaded function.
   *
   * \param[in,out] fImg The in-/output image.
   * \param[in]    fPos0 The start point of the line.
   * \param[in]    fPos1 The end point of the line.
   * \param[in]     fCol The color of the line.
   */
  void EXPORT_TYPE drawLine(Vi::Image<uint8_t> &fImg,
                            Vi::Point<int> fPos0,
                            Vi::Point<int> fPos1,
                            Vi::YCCColor fCol);

  /**
   * Draw a bounding box.
   *
   * \param[in,out] fImg The image on which the bounding box is drawn.
   * \param[in]    fBbox The bounding box.
   * \param[in]     fCol The drawing color.
   * \param[in]    fFill Fill the bounding box?
   */
  void EXPORT_TYPE drawBbox(Vi::Image<> &fImg,
                            const Vi::Bbox<int> &fBbox,
                            const Vi::YCCColor &fCol,
                            bool fFill = false);

  /**
   * Draw a 2D vector.
   *
   * \note Replace this with a nice rotation matrix implementation.
   *
   * \param[in,out] fImg The image on which the bounding box is drawn.
   * \param[in]  fVector The bounding box.
   * \param[in]  fOffset The offset position for the vector.
   * \param[in]   fWidth The width of the arrow head.
   * \param[in]     fCol The drawing color.
   */
  void EXPORT_TYPE drawVector2D(Vi::Image<> &fImg,
                                const Vi::Vector2D<int> &fVector,
                                const Vi::Point<int> &fOffset,
                                unsigned int fWidth,
                                const Vi::YCCColor &fCol);

  /**
   * Draw a 2D vector (overloaded function).
   *
   * \param[in,out] fImg The image on which the bounding box is drawn.
   * \param[in]  fVector The bounding box.
   * \param[in]  fOffset The offset position for the vector.
   * \param[in]   fWidth The width of the arrow head.
   * \param[in]     fCol The drawing color.
   */
  void EXPORT_TYPE drawVector2D(Vi::Image<> &fImg,
                                const Vi::Vector2D<float> &fVector,
                                const Vi::Point<int> &fOffset,
                                unsigned int fWidth,
                                const Vi::YCCColor &fCol);
}

#endif // DRAW_HPP
