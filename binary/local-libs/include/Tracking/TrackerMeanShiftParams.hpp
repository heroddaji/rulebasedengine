/** \file
 * Mean shift tracker header file.
 *
 * $Id: TrackerMeanShiftParams.hpp.example 1859 2010-11-17 11:14:47Z kris $
 */

#ifndef TRACKER_MEAN_SHIFT_PARAMETERS_HPP
#define TRACKER_MEAN_SHIFT_PARAMETERS_HPP

/**
 * Uncomment for using color information in the histogram. Be sure the settings
 *  below correspond to the setting used here.
 */
#define TRACKER_MEAN_SHIFT_USE_COLOR_INFO

namespace Vi
{
  /**
   * Compile-time settings for the Mean Shift Tracker.
   */
  namespace TrackerMeanShiftParams
  {
    //
    // --- Histogram settings ---------------------------------------------------
    //
    
    /// Amount of color channels used in the histogram (choose 1 for Y only).
    const unsigned int histNumChannels = 3;

    /**
     * The amount of histogram bins per color channel (must be a power of two).
     *
     * \note By increasing the number of histogram bins, the histogram will
     *    approximate the PDF better, implicating also that possible image
     *    noise will be modeled (i.e. for noisy image data, use less bins).
     */
    const unsigned int histNumBins = 16;
    
    /// = histNumChannels * histNumBins.
    const unsigned int histNumBinsTotal = 48;
    
    /// = log(256 / histNumBins) / log(2), for internal use (log(2) = 1).
    const unsigned int histBinShift = 4;

    /// Histogram update factor (i.e. the target histogram `learning factor').
    const float histUpdateFactor = 0.001f;

    //
    // --- Tracker settings -----------------------------------------------------
    //

    /**
     * Defines how many scales should be searched. Each 10% scale change w.r.t.
     *  the original bounding box is considered. So for instance, a settings of
     *  '3' would result in the scales ({-3, -2, -1, 0, 1, 2, 3} * scaleFactor)
     *  to be searched.
     */
    const unsigned int numScales = 2;

    /**
     * The scaling factor used for searching different scales [%]. This value
     *  should be chosen such that it represents the nominal inter-frame object
     *  size change.
     *
     * \note Scaling is applied such that bounding box aspect ratio should in
     *    principle be left unchanged. But, as bounding boxes are represented
     *    by integer value coordinates, rounding errors occur and may result in
     *    aspect ratio changes. Especially for non-square small objects this
     *    can be a problem.
     */
    const float scaleFactor = 0.002f;

    /// The minimal prediction distance (Euclidian pixel distance).
    const unsigned int minPredDist = 1;

    /**
     * The maximum amount of tracking iterations, used to set bounds to the
     *  tracking processing time. Normally this value should not be reached
     *  (only in bad tracking cases).
     */
    const unsigned int maxTrackIters = 1000;
  }
}

#endif // TRACKER_MEAN_SHIFT_PARAMETERS_HPP
