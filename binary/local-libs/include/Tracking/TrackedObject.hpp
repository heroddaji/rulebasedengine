/** \file
 * Tracked object abstract base class definition.
 *
 * $Id: TrackedObject.hpp 1985 2011-01-18 10:20:51Z kris $
 */

#ifndef TRACKED_OBJECT_HPP
#define TRACKED_OBJECT_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Bbox.hpp>
#include <ViNotion/Image.hpp>

#include <GenericObject/GenericObject.hpp>
#include <Features/ObjectFeatureHistogram.hpp>

#include "Tracker.hpp"
#include "TrackerFilter.hpp"

namespace Vi
{
  /**
   * Object tracking sub-state type. 
   *
   * This state is governed by the detection/tracking hypothesis algorithm.
   *
   * The state transition normally is: APPEAR --> TRACK --> DISAPPEAR.
   */
  enum ObjectTrackingSubState
  {
    OBJSTATE_APPEAR,   ///< Appear state (not yet tracked).
    OBJSTATE_TRACK,    ///< Tracking state (fully acknowledged object (by Chuck Norris)).
    OBJSTATE_DISAPPEAR ///< Disappear state (soon to be roundhouse kicked).
  };

  /**
   * Hypothesis state. Extends the object tracking substates with split and
   *  merge states.
   */
  enum HypothesisState
  {
    HYPSTATE_APPEAR,
    HYPSTATE_TRACK,
    HYPSTATE_DISAPPEAR,
    HYPSTATE_SPLIT,
    HYPSTATE_MERGE,
  };

  /// Tracked object abstract base class.
  class EXPORT_TYPE TrackedObject : public Vi::GenericObject
  {
  public:

    /**
     * Constructor.
     *
     * \param[in]       fTracker The tracker implementation.
     * \param[in] fTrackerFilter The tracker filter implementation.
     * \param[in] fObjectFeature The new object feature. ObjectFeatureHistogram
     *                            by default.
     */
    TrackedObject(Vi::Tracker *fTracker,
                  Vi::TrackerFilter *fTrackerFilter,
                  Vi::ObjectFeature *fObjectFeature = new Vi::ObjectFeatureHistogram);

    /**
     * Copy constructor.
     */
    TrackedObject(const Vi::TrackedObject &fObject);

    /**
     * Operator =.
     */
    Vi::TrackedObject &operator=(const Vi::TrackedObject &fOther);

    /**
     * Destructor.
     */
    ~TrackedObject();

    /**
     * Tracker initialization.
     *
     * \param[in] fFrame The input frame.
     * \param[in]  fBbox The tracked entity initial bounding box.
     */
    void init(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox);
    
    /**
     * Tracker reinitialization.
     *
     * \note  This happens when an object has been match to one (and only one)
     *    detection. The Bbox is set to the one of the detection and the histo-
     *    gram is recomputed, but the characteristics of the filter remains the
     *    same.
     *
     * \param[in] fFrame The input frame.
     * \param[in]  fBbox The tracked entity initial bounding box.
     */
    void reinit(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox);
    
    /**
     * Perform a tracking update. This function will call the predict/update/
     *  correct sequence for this object.
     *
     * \param[in] fFrame The input frame.
     */
    void update(const Vi::Image<> &fFrame);

    /**
     * Perform a position prediction using the filter and the tracker. 
     *
     * \param[in] fFrame The input frame.
     */
    void predictPosition(const Vi::Image<> &fFrame);

    /**
     * Perform the correction to the filter state (Kalman). 
     */
    void correctFilterState();
    
    /// The object tracker (for refining prediction).
    Vi::Tracker *mTracker;

    /// The object tracker filter (for predict/correct).
    Vi::TrackerFilter *mFilter;

    /// The tracking substate.
    Vi::ObjectTrackingSubState mTrackingSubState;

    /// The number of frames in the current tracking substate.
    unsigned int mNbFramesInCurrentSubState;

    /// The object hypothesis state.
    Vi::HypothesisState mHypothesisState;

    /// The number of frames in the current hypothesis state.
    unsigned int mNbFramesInHypothesisState;

    /// The object ID.
    unsigned int mID;

    /// The predicted bounding box.
    Vi::Bbox<int> mPredictedBbox;
  };
}

#endif // TRACKED_OBJECT_HPP
