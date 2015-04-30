/** \file
 * Color space conversions.
 *
 * Based on CCIR 601 / ITU-R BT.601 and Charles Poynton's Color FAQ
 * http://www.poynton.com/ColorFAQ.html.
 *
 * Y'Cb Cr ranges:
 * - Y' 220 levels, start at 16 -> Range [16; 235]
 * - Cb 225 levels, centered around 128 -> Range [16; 240]
 * - Cr 225 levels, centered around 128 -> Range [16; 240]
 *
 * JFIF YCbCr uses a range of [0; 255] for each of its components, so
 * to convert between Y'CbCr and JFIF Y'CbCr linear scaling is
 * necessary. This also holds for converting between Y'CbCr and
 * grayscale.
 *
 * Because when converting between Y'CbCr and RGB the component values
 * are clipped, these conversions are not invertible.
 *
 * $Id: ColorSpaceConv.hpp 1706 2010-09-15 11:55:16Z rick $
 */

#ifndef COLOR_SPACE_CONV_HPP
#define COLOR_SPACE_CONV_HPP

#include <iostream>
#include <cmath>
#include <stdint.h>

#include "Compat.hpp"
#include "Misc.hpp"

namespace Vi
{
  /**
   * Convert from RGB to YUV colorspace.
   */
  ALWAYS_INLINE inline void RGB_2_YCbCr(uint8_t &Y, uint8_t &Cb, uint8_t &Cr, uint8_t R, uint8_t G, uint8_t B)
  {
    /*
    // Reference implementation below:
    double Yd, Cbd, Crd;

    Yd  = (  65.738 * R + 129.057 * G +  25.064 * B )/256 +  16;
    Cbd = ( -37.945 * R -  74.494 * G + 112.439 * B )/256 + 128;
    Crd = ( 112.439 * R -  94.154 * G -  18.285 * B )/256 + 128;

    // Clip values
    if( Yd < 16)  Y = 16; else if( Yd > 235) Y  = 235; else Y =  (uint8_t) round( Yd);
    if(Cbd < 16) Cb = 16; else if(Cbd > 240) Cb = 240; else Cb = (uint8_t) round(Cbd);
    if(Crd < 16) Cr = 16; else if(Crd > 240) Cr = 240; else Cr = (uint8_t) round(Crd);
    */

    // Max value for Y channel: (65.738 + 129.057 +  25.064) * 255 = 56064
    // int (32-bit): 2147483647/56065 = 38293 Largest power of two smaller than 38293 is 32768 = 2^15
    // 0x400000 = 2^22, for rounding.
    int Yi, Cbi, Cri;
  
    Yi  = ((  2154103 * R + 4228940 * G +  821297 * B + 0x400000) >> 23 ) +  16;
    Cbi = (( -1243382 * R - 2441019 * G + 3684401 * B + 0x400000) >> 23 ) + 128;
    Cri = ((  3684401 * R - 3085238 * G -  599163 * B + 0x400000) >> 23 ) + 128;
  
    // Clip values.
    Y  = Vi::clip(16, Yi , 235);
    Cb = Vi::clip(16, Cbi, 240);
    Cr = Vi::clip(16, Cri, 240);
  }

  /**
   * Convert from YUV to RGB colorspace.
   */
  ALWAYS_INLINE inline void YCbCr_2_RGB(uint8_t &R, uint8_t &G, uint8_t &B, uint8_t Y, uint8_t Cb, uint8_t Cr)
  {
    /*
    // Reference implementation below:
    double Rd, Gd, Bd;

    Rd = ( 298.082*(Y - 16)                        +  408.583*(Cr - 128.0) )/256;
    Gd = ( 298.082*(Y - 16) - 100.291*(Cb - 128.0) -  208.120*(Cr - 128.0) )/256;
    Bd = ( 298.082*(Y - 16) + 516.411*(Cb - 128.0)                         )/256;

    // Clip values to 0..255
    if(Rd < 0) R = 0; else if(Rd > 255) R = 255; else R = (uint8_t) round(Rd);
    if(Gd < 0) G = 0; else if(Gd > 255) G = 255; else G = (uint8_t) round(Gd);
    if(Bd < 0) B = 0; else if(Bd > 255) B = 255; else B = (uint8_t) round(Bd);
    */

    // Max value for B channel: 298.082*(255-16)+516.411*(255-128) = 136826.
    // int (32-bit): 2147483647/136826 = 15694 Largest power of two smaller than 15694 is 8192 = 2^13.
    // 0x100000 = 2^20, for rounding.

    int Ri, Gi, Bi;

    Ri = ( 2441888*(Y - 16)                      +  3347112*(Cr - 128) + 0x100000) >> 21;
    Gi = ( 2441888*(Y - 16) -  821584*(Cb - 128) -  1704919*(Cr - 128) + 0x100000) >> 21;
    Bi = ( 2441888*(Y - 16) + 4230439*(Cb - 128)                       + 0x100000) >> 21;

    // Clip values to 0..255.
    R = Vi::clip(0, Ri, 255);
    G = Vi::clip(0, Gi, 255);
    B = Vi::clip(0, Bi, 255);
  }

  /**
   * Convert from Y'CbCr to JFIF Y'CbCr color space.
   */
  ALWAYS_INLINE inline void YCbCr_2_JFIF(uint8_t &JY, uint8_t &JCb, uint8_t &JCr, uint8_t Y, uint8_t Cb, uint8_t Cr)
  {
    int JYi, JCbi, JCri;

    JYi  = (Y  - 16)*255/219;
    JCbi = (Cb - 16)*255/224;
    JCri = (Cr - 16)*255/224;

    // Clip values to 0..255.
    JY  = Vi::clip(0, JYi, 255);
    JCb = Vi::clip(0, JCbi, 255);
    JCr = Vi::clip(0, JCri, 255);
  }

  /**
   * Convert from JFIF Y'CbCr to Y'CbCr color space
   */
  ALWAYS_INLINE inline void JFIF_2_YCbCr(uint8_t &Y, uint8_t &Cb, uint8_t &Cr, uint8_t JY, uint8_t JCb, uint8_t JCr)
  {
    Y  = JY*219/255 + 16;
    Cb = JCb*224/255 + 16;
    Cr = JCr*224/255 + 16;
  }

  /**
   * Convert from Y' to grayscale.
   */
  ALWAYS_INLINE inline uint8_t Y_2_gray(uint8_t Y)
  {
    return Vi::clip(0, (Y - 16)*255/219, 255);
  }

  /**
   * Convert from grayscale to Y'.
   */
  ALWAYS_INLINE inline uint8_t gray_2_Y(uint8_t Y)
  {
    // Input value is inherently clipped by 0..255, so output is also clipped.
    return Y*219/255 + 16;
  }
}

#endif // COLOR_SPACE_CONV_HPP
