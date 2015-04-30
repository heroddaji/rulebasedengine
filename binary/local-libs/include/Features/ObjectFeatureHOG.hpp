/** \file
 * HOG object feature prototype.
 *
 * $Id: ObjectFeatureHOG.hpp 1959 2011-01-14 10:44:20Z kris $
 */

#include <GenericObject/ObjectFeature.hpp>

#include "FeatureSpaceHOG.hpp"

namespace Vi
{
  /**
   * HOG object feature class.
   */
  class EXPORT_TYPE ObjectFeatureHOG : public Vi::ObjectFeature
  {
  public:

    /**
     * Default constructor.
     */
    ObjectFeatureHOG(Vi::FeatureSpaceHOG *fFeatureSpace);

    Vi::ObjectFeature* clone() const;
    void update(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox);
    float compare(const Vi::ObjectFeature *fOtherFeature) const;

    /// The scale in which the feature is defined (w.r.t. the overall feature space).
    Vi::Frac<int> mFeatureScale;

  private:

    /// Pointer to the feature space object.
    Vi::FeatureSpaceHOG *mFeatureSpace;

    /// The bounding box of the object as it was matched to the feature space.
    Vi::Bbox<int> mMappedBboxPixel;

    /// HOG cell grid description of the bounding box (i.e. every integer unit is a cell).
    Vi::Bbox<int> mMappedBboxCell;
  };
}
