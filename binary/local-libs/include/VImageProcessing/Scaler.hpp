/** \file
 * Scaler class header file.
 *
 * $Id: Scaler.hpp 1891 2010-12-01 15:54:51Z kris $
 */

#ifndef SCALER_HPP
#define SCALER_HPP

// Primarily for performance analysis betweed fixed and float.
#define VI_FIXED

#ifdef VI_FIXED
#define VI_TYPE int16_t
#else
#define VI_TYPE double
#endif

#include <ViNotion/Array2D.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>

namespace Vi
{
  /**
   * Polyphase image scaler.
   *
   * This scaler uses polyphase filtering to up- and downsample images.
   * The vertical and horizontal scale can be different. Both scales are arbitrary rational numbers.
   *
   * The interpolation and low-pass filters are windowed sinc-filters.
   *
   * Reference:
   *  The Scientist and Engineer's Guide to Digital Signal Processing, Chapter 16.
   */
  class EXPORT_TYPE Scaler
  {
  public:

    /// Constructor.
    Scaler();

    /// Destructor.
    ~Scaler();
    
    /**
     * Set scale of the scaler.
     *
     * \param[in] hscalenum Numerator of horizontal scale.
     * \param[in] hscaleden Denominator of horizontal scale.
     * \param[in] vscalenum Numerator of vertical scale.
     * \param[in] vscaleden Denominator of vertical scale.
     */
    void setScale(int hscalenum, int hscaleden, int vscalenum, int vscaleden);

    /**
     * Scale the image by the set scale. Can only scale PF_YCC444P images.
     *
     * \param[in] in image.
     *
     * \return Scaled image.
     */
    Vi::Image<uint8_t> scaleImage(const Vi::Image<uint8_t> &in);

  private:

    /// Calculate the coefficients for a single phase.
    void calccoefs2(VI_TYPE* filter, int numtaps, int upscale, int phase, double cutoff);

    /// Scale image horizontally by the corresponding scale.
    void scalehor(Vi::Array2D<uint8_t> &frameout, Vi::Array2D<uint8_t> &framein);

    /// Scale image vertically by the corresponding scale.
    void scalever(Vi::Array2D<uint8_t> &frameout, Vi::Array2D<uint8_t> &framein);

    int hscalenum, hscaleden; ///< Numerator and denominator of horizontal scale.
    int hnumtaps;             ///< Number of horizontal filter taps.
    VI_TYPE *hcoeflut;        ///< Coefficient LUT of horizontal scaler.

    int vscalenum, vscaleden; ///< Numerator and denominator of vertical scale.
    int vnumtaps;             ///< Number of vertical filter taps.
    VI_TYPE *vcoeflut;        ///< Coefficient LUT of horizontal scaler.
  };
}

#endif // SCALER_HPP
