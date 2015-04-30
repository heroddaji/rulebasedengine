/** \file
 * Hypothesis class definition.
 *
 * $Id: Hypothesis.hpp 1908 2010-12-07 16:38:20Z kris $
 */

#ifndef HYPOTHESIS_HPP
#define HYPOTHESIS_HPP

#include <ViNotion/Compat.hpp>
#include <Tracking/TrackedObject.hpp>

namespace Vi
{
  /**
   * Object type specification. Can be object types as inherited from the
   *  GenericObject class.
   */
  enum GenericObjectType
  {
    OBJTYPE_DETECTED, ///< Detected object.
    OBJTYPE_TRACKED   ///< Tracked object.
  }; 

  /**
   * The hypothesis object.
   */
  class EXPORT_TYPE Hypothesis
  {
    public:
    
    /**
     * Constructor.
     *
     * \param[in]    fType The type of object in this hypothesis (detected or tracked).
     * \param[in]      fID An identifier for the object.
     * \param[in]   fState The most likely state for this object.
     * \param[in]   fScore The hypothesis score.
     * \param[in] fMatchID The identifier of the other object in the hypothesis.
     *
     */
    Hypothesis(Vi::GenericObjectType fType = Vi::OBJTYPE_DETECTED,
               int fID = 0,
               Vi::HypothesisState fState = Vi::HYPSTATE_APPEAR,
               float fScore = 0.0f,
               int fMatchID = 0);
    
    /**
     * Equality test operator.
     */
    bool operator==(const Vi::Hypothesis &fRHS) const;

    /**
     * Inequality test operator.
     */
    bool operator!=(const Vi::Hypothesis &fRHS) const;

    GenericObjectType mType;  ///< Either a blob or a tracked object.
    unsigned int mID;         ///< Identifier of the object.
    HypothesisState mState;   ///< State : appear, disappear, tracked, split, merge.
    float mScore;             ///< The score of this hypothesis.
    int mMatchID;             ///< Matched ID (to which object should the current object be matched).
    int mSplitID;             ///< Split ID (identifier for the third object involved in a split).
    int mMergeID;             ///< Merge ID (identifier for the third object involved in a merge).

    bool mIsConsistent;        ///< Tells if the hypothesis is consistent w.r.t. to the others.
  };
}

#endif // HYPOTHESIS_HPP
