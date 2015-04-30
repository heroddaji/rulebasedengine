/** \file
 * Object feature abstract base class definition.
 *
 * $Id: ObjectFeature.hpp 1972 2011-01-14 15:26:05Z kris $
 */

#ifndef OBJECT_FEATURE_HPP
#define OBJECT_FEATURE_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>
#include <ViNotion/Misc.hpp>

namespace Vi
{
  /**
   * Object feature abstract base class.
   */
  class EXPORT_TYPE ObjectFeature
  {
  public:

    /**
     * Default constructor.
     */
    ObjectFeature()
      : mFeatureScale(Vi::Frac<int>(1, 1)) // Pixel domain by default
    {
    }

    /**
     * Virtual desctructor to allow for destruction using base pointers.
     */
    virtual ~ObjectFeature() {}

    /**
     * Clone function.
     *
     * \return An instance of a derived implementation.
     */
    virtual Vi::ObjectFeature* clone() const = 0;

    /**
     * Update and calculate the feature state.
     *
     * \param[in] fFrame The image data for which the features must be calculated.
     * \param[in]  fBbox The feature bounding box in the image.
     */
    virtual void update(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox) = 0;

    /**
     * Compare two features.
     *
     * \param[in] fOtherFeature A pointer to the other object feature.
     *
     * \return A similarity value ranging from 0.0f to 1.0f where similarity
     *          value 0.0f means non-similar and 1.0f means similar.
     */
    virtual float compare(const Vi::ObjectFeature *fOtherFeature) const = 0;

    /// The scale in which the feature is defined (w.r.t. the overall feature space).
    Vi::Frac<int> mFeatureScale;
  };
}

#endif // OBJECT_FEATURE_HPP
