/** \file
 * Histogram object feature prototype.
 *
 * $Id: ObjectFeatureHistogram.hpp 1975 2011-01-17 10:27:31Z kris $
 */

#ifndef OBJECT_FEATURE_HISTOGRAM_HPP
#define OBJECT_FEATURE_HISTOGRAM_HPP

#include <ViNotion/Compat.hpp>
#include <GenericObject/ObjectFeature.hpp>

#include "ObjectFeatureHistogramParams.hpp"

namespace Vi
{
  /**
   * Histogram object feature.
   */
  class EXPORT_TYPE ObjectFeatureHistogram : public Vi::ObjectFeature
  {
  public:

    /**
     * Constructor.
     */
    ObjectFeatureHistogram();

    Vi::ObjectFeatureHistogram* clone() const;
    void update(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox);

    /**
     * Compute the bhattacharyya coefficient between two histogram features.
     *
     * \param[in] fOtherFeature Pointer to the other object feature.
     *
     * \return The bhattacharyya coefficient as a similarity measure. 
     */
    float compare(const Vi::ObjectFeature *fOtherFeature) const;

    /**
     * Debug function, prints the histogram.
     */
    void printHistogram() const;

  private:

    /// The normalized histogram.
    float mHistogram[Vi::ObjectFeatureHistogramParams::histNumBinsTotal];
  };
}

#endif // OBJECT_FEATURE_HISTOGRAM_HPP
