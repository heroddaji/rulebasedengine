/** \file
 * Combinatorial Multiple Hypothesis Tracking Parameters header file.
 *
 * $Id: CombiMultiHypoTrackingParams.hpp.example 2005 2011-01-24 16:02:59Z kris $
 */
#ifndef COMBI_MULTI_HYPO_TRACKING_PARAMS_HPP
#define COMBI_MULTI_HYPO_TRACKING_PARAMS_HPP

namespace Vi
{
  /**
   * Compile-time settings for the GMM Background Model
   */
  namespace CombiMultiHypoTrackingParams
  {
    // =============================================================
    // === COMBINATORIAL MULTIPLE HYPOTHESIS TRACKING PARAMETERS ===
    // =============================================================

    /// The number of frames before an object is in track state.
    const unsigned int appearanceToTrackThreshold = 5;
    
    /// The number of frames before an undetected object disappears. 
    const unsigned int trackToDisappearThreshold = 5;

    /// The 'K' constant - max number of hypotheses.
    const unsigned int maxNumberOfHypothesesPaths = 10;

		/// If you want to have some informations printed on stdout
		const bool verbose = false;

    /**
     * \todo Note that when an object is created, it instantiated with a Mean-Shift Tracker and a Kalman Filter.
     */
  }
}

#endif 
