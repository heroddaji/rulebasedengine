/** \file
 * Alpha beta tracker filter header.
 *
 * $Id: TrackerFilterAlphaBeta.hpp 1917 2010-12-17 09:43:57Z guillaume $
 */

#ifndef TRACKER_FILTER_ALPHA_BETA_HPP
#define TRACKER_FILTER_ALPHA_BETA_HPP

#include <ViNotion/Compat.hpp>

#include "TrackerFilter.hpp"

namespace Vi
{
  /**
   * Alpha beta tracker filter class.
   */
  class EXPORT_TYPE TrackerFilterAlphaBeta : public Vi::TrackerFilter
  {
  public:

    /**
     * Constructor.
     */
    TrackerFilterAlphaBeta();

    Vi::TrackerFilterAlphaBeta* clone() const;
    void init(const Vi::Bbox<int> &fBbox) {}
    void predict(Vi::Bbox<int> &fBbox);
    void correct(const Vi::Bbox<int> &fPredictedBbox, const Vi::Bbox<int> &fMeasuredBbox);

  private:

    int mVelX;    ///< X-direction velocity.
    int mVelY;    ///< Y-direction velocity.
    float mAlpha; ///< Position update correction factor.
    float mBeta;  ///< Velocity correction factor.
  };
}

#endif // TRACKER_FILTER_ALPHA_BETA_HPP
