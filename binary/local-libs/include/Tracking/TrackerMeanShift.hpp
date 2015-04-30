/** \file
 * Mean shift tracker header file.
 *
 * $Id: TrackerMeanShift.hpp 1985 2011-01-18 10:20:51Z kris $
 */

#ifndef TRACKER_MEAN_SHIFT_HPP
#define TRACKER_MEAN_SHIFT_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>

#include "Tracker.hpp"
#include "TrackerMeanShiftParams.hpp"

namespace Vi
{
  /**
   * Mean shift tracker. Implementation based on:
   *
   *  Based on: D. Comanciu, V. Ramesh, P. Meer 'Kernel-Based Object Tracking'.
   */
  class EXPORT_TYPE TrackerMeanShift : public Vi::Tracker
  {
  public:

    Vi::TrackerMeanShift* clone() const;
    void init(const Vi::Image<uint8_t> &fFrame, const Vi::Bbox<int> &fBbox);
    void update(const Vi::Image<uint8_t> &fFrame, Vi::Bbox<int> &fBbox);

  private:

    /**
     * Track the object to the next frame.
     *
     * \param[in]      fFrame Frame image data.
     * \param[in] fBboxTarget Target bounding box.
     * \param[out]  fBboxPred Predicted target bounding box.
     *
     * \return The Bhattacharyya coefficient between the current and predicted
     *          location histogram.
     */
    float track(const Vi::Image<uint8_t> &fFrame,
                const Vi::Bbox<int> &fBboxTarget,
                Vi::Bbox<int> &fBboxPred) const;

    /**
     * Get a mean shift vector.
     *
     * \note The negated derivative of the kernel function g(x) = -k(x) (which
     *  is the Epanechnikov kernel as described below) becomes a constant
     *  function. Therefore, it is not considered in the mean shift vector cal-
     *  culation.
     *
     * \param[in] fFrame The input image.
     * \param[in]  fHist The histogram of the old target position.
     * \param[out] fBbox The bounding box of the new mean shifted position.
     */
    void doMeanShift(const Vi::Image<uint8_t> &fFrame,
                     const float *fHist,
                     Vi::Bbox<int> &fBbox) const;

    /**
     * Get the histogram of the given bounding box in the given image.
     *
     * \param[in] fFrame The input image.
     * \param[in]  fBbox The bounding box within the image.
     * \param[out] fHist The histogram that will be calculated.
     */
    void getHistogram(const Vi::Image<uint8_t> &fFrame,
                      const Vi::Bbox<int> &fBbox,
                      float *fHist) const;

    /**
     * Epanechnikov kernel function.
     *
     * \note The derivative of the kernel is a constant function.
     *
     * \param[in] fX X-offset between 0.0f and 1.0f.
     * \param[in] fY Y-offset between 0.0f and 1.0f.
     *
     * \return The kernel value.
     */
    inline float kernelFunction(float fX, float fY) const
    {
      float distSquared = fX * fX + fY * fY;

      if (distSquared > 1.0f)
        return 0.0f; // Value outside of kernel
    
      //
      // k(x) = 1 / 2 c_d^(-1) (d + 2)(1 - x^2)
      //
      //  with d = 2 and c_d = M_PI
      //
      return ((1.0f - distSquared) * M_2_PI); // Note: M_2_PI = 2 / pi
    }

    /**
     * Get the Bhattacharyya coefficient between two histograms.
     *
     * \param[in] fHist1 Histogram 1.
     * \param[in] fHist2 Histogram 2.
     *
     * \return The Bhattacharyya coefficient between two histograms.
     */
    float bhattacharyyaCoeff(const float *fHist1, const float *fHist2) const
    {
      float bhatCoeff = 0.0f;

      for (unsigned int i = 0; i < Vi::TrackerMeanShiftParams::histNumBinsTotal; i++)
        bhatCoeff += sqrt(fHist1[i] * fHist2[i]);
      
      return bhatCoeff;
    }

    /// The target histogram.
    float mHistTarget[Vi::TrackerMeanShiftParams::histNumBinsTotal];
  };
}

#endif // TRACKER_MEAN_SHIFT_HPP
