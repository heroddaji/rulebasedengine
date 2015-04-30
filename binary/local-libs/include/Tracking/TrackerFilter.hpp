/** \file
 * Tracker state filter abstract base class.
 *
 * $Id: TrackerFilter.hpp 1975 2011-01-17 10:27:31Z kris $
 */

#ifndef TRACKER_FILTER_HPP
#define TRACKER_FILTER_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Bbox.hpp>

namespace Vi
{
  /**
   * The tracker filter abstract base class.
   */
  class EXPORT_TYPE TrackerFilter
  {
  public:

    /**
     * Virtual desctructor to allow for destruction using base pointers.
     */
    virtual ~TrackerFilter() {}

    /**
     * Clone function.
     *
     * \return An instance of a derived implementation.
     */
    virtual Vi::TrackerFilter* clone() const = 0;

    /** 
     * Init. 
     *
     * \param[in] fBbox The previous object bounding box / the predicted bounding box.
     */
    virtual void init(const Vi::Bbox<int> &fBbox) = 0;

    /** 
     * Time update (predict). Predict the new position of the object using the
     *  object model.
     *
     * \param[in,out] fBbox The previous object bounding box / the predicted bounding box.
     */
    virtual void predict(Vi::Bbox<int> &fBbox) = 0;

    /**
     * Measurement Update (correct). Correct the filter inner state using the
     *  measured bounding box.
     *
     * \param[in]    fPredictedBbox The predicted object bounding box. 
		 																	Result of the *full* prediction (i.e. filter + mean-shift).
     * \param[in,out] fMeasuredBbox The measured object bounding box (could be equal to prediction)
     */
    virtual void correct(const Vi::Bbox<int> &fPredictedBbox, const Vi::Bbox<int> &fMeasuredBbox) = 0;
  };
}

#endif // TRACKER_FILTER_HPP
