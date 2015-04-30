/** \file
 * Hypothesis path class definition.
 *
 * $Id: HypothesisPath.hpp 1776 2010-10-04 13:24:29Z kris $
 */

#ifndef HYPOTHESIS_PATH_HPP
#define HYPOTHESIS_PATH_HPP

#include <vector>

#include "Hypothesis.hpp"

namespace Vi
{
  /**
   * Hypothesis path class. This is a wrapper around the Hypothesis class to
   *  work with collections of Hypotheses.
   */
  class HypothesisPath
  {
    public:

      /**
       * Constructor.
       */
      HypothesisPath();
      
      /**
       * Equality test operator.
       */
      bool operator==(const Vi::HypothesisPath &fRHS) const;

      /**
       * Inequality test operator.
       */
      bool operator!=(const Vi::HypothesisPath &fRHS) const;
        
      /**
       * Test if the object sent in parameter is already in the path.
       *
       * \param[in] fType the type of the object. 
       * \param[in]   fID the identifier of the object.
       *
       * \return true if the object is , false otherwise.
       */ 
      bool isObjectAlreadyInPath(Vi::GenericObjectType fType, unsigned int fID) const;
    
      /**
       * Set the path's score.
       */
      void setScore();

      std::vector<Hypothesis> mHypotheses;  ///< The set of hypothesis currently in the path.
      float mScore;                         ///< The score of the path.
  };
}

#endif // HYPOTHESIS_PATH_HPP
