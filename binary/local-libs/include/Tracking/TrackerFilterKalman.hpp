/** \file
 * Kalman tracker filter header.
 *
 * $Id: TrackerFilterKalman.hpp 1985 2011-01-18 10:20:51Z kris $
 */

#ifndef TRACKER_FILTER_KALMAN_HPP
#define TRACKER_FILTER_KALMAN_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Bbox.hpp>

#include "TrackerFilter.hpp"

// Matrix stuff
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// Namespace alias for convenience
namespace ub = boost::numeric::ublas;

namespace Vi
{
  /**
   * Kalman tracker filter class.
   *
   * This is an implementation of the Kalman filter for object tracking.
   * Note however that the learning is not implemented (i.e. no EM algorithm).
   * Hence, this means in particular that the <EM> process noise matrix Q </EM>
   * and the <EM> measurement noise matrix R </EM> are "guessed" and remain
   * constant.
   *
   * \todo Should find a stable way to update them as the algorithm goes. 
   */
  class EXPORT_TYPE TrackerFilterKalman : public Vi::TrackerFilter
  {
  public:

    /**
     * Constructor.
     */
    TrackerFilterKalman();

    Vi::TrackerFilterKalman* clone() const;
    void init(const Vi::Bbox<int> &fBbox);
    void predict(Vi::Bbox<int> &fBbox);
    void correct(const Vi::Bbox<int> &fPredictedBbox, const Vi::Bbox<int> &fMeasuredBbox); 

  private:

    /**
     * The estimate of the measurement noise covariance matrix.
     *
     * \note  The estimate is given by : 
     *        R = (1/N-1) * sum{j=1 to N}[(rj - r)(rj - r)^T - (N-1/N) HjPjHj^T]
     */
    void estimateMeasurementNoiseCovariance();
    
    /**
     * Estimate a covariance matrix based on samples.
     * \note currently not used
     */
    ub::matrix<float> estimateCovariance(std::vector<ub::vector<float> > &fX);

    /**
     * Hidden state vector.
     * 
     * \note The hidden state vector is [pos(x), pos(y), vel(x), vel(y), acc(x), acc(y)]
     */
    ub::vector<float> mX;
    
    /**
     * State transition matrix. Given by Newton's law of movement
     *
     * \note A is:  1  0  1  0 0.5 0
     *              0  1  0  1  0 0.5
     *              0  0  1  0  1  0
     *              0  0  0  1  0  1
     *              0  0  0  0  1  0
     *              0  0  0  0  0  1
     */
    ub::matrix<float> mA; 
    
    ub::matrix<float> mP; ///< Hidden state covariance matrix.
    ub::matrix<float> mQ; ///< Process noise covariance matrix.
    ub::matrix<float> mR; ///< Measurement noise covariance matrix.
    ub::matrix<float> mC; ///< Obervation generation matrix.

    /**
     * Residual or innovation: error between measurement and prediction.
     *
     * \note  This value is used to estimate the measurent noise covariance
     *    matrix. Note however that there is still a problem with the
     *    measurement error of the speed and the acceleration, since we don't
     *    have measurement for these values...  
     *
     *  \note Used in the estimate measurement covariance -> currently not used.
     */
    ub::vector<float> mResidualFrame;

    /**
     * Accumulation of the residual at each frame.
     *
     *  \note Used in the estimate measurement covariance -> currently not used.
     */
    ub::vector<float> mResidualAccumulator;

    /**
     * The estimation of the measurement noise covariance matrix.
     *
     *  \note Used in the estimate measurement covariance -> currently not used.
     */
    ub::matrix<float> mEstimateR;

    /**
     * The frame counter (i.e. the 'time' counter). 
     *
     *  \note Used in the estimate measurement covariance -> currently not used.
     */
    unsigned int mCounter;
  };
}

#endif // TRACKER_FILTER_KALMAN_HPP
