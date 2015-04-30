/** \file
 * Histogram object feature compile-time settings.
 *
 * $Id: ObjectFeatureHistogramParams.hpp.example 1954 2011-01-13 17:05:20Z kris $
 */

#ifndef OBJECT_FEATURE_HISTOGRAM_PARAMS_HPP
#define OBJECT_FEATURE_HISTOGRAM_PARAMS_HPP

/**
 * Specifies whether to use color info in the histogram or not. Be sure to have
 *  consistent settings for the amount of histogram bins as well.
 */
#define OBJECT_FEATURE_HISTOGRAM_USE_COLOR_INFO

namespace Vi
{
  namespace ObjectFeatureHistogramParams
  {
    /// The number of color channels in the histogram.
    const unsigned int histNumChannels = 3;

    /// The number of histogram bins per color channel.
    const unsigned int histNumBinsPerChannel = 8;

    /// = mNbBinsPerChannels * histNumChannels
    const unsigned int histNumBinsTotal = 24;

    /// = log(256 / histNumBinsPerChannel) / log(2).
    const unsigned int histBinShift = 5;
  }
}

#endif // OBJECT_FEATURE_HISTOGRAM_PARAMS_HPP
