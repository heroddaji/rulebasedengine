/** \file
 * Generic object class definition.
 *
 * $Id: GenericObject.hpp 1989 2011-01-18 15:41:31Z kris $
 */

#ifndef GENERIC_OBJECT_HPP
#define GENERIC_OBJECT_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Bbox.hpp>
#include <ViNotion/Image.hpp>

#include "ObjectFeature.hpp"

namespace Vi
{
  /// Generic object class
  class EXPORT_TYPE GenericObject
  {
  public:
   
    /**
     * Constructor.
     *
     * \param[in] fFeature The feature instantiation.
     * \param[in]    fBbox The bounding box of the object.
     */
    GenericObject(Vi::ObjectFeature *fFeature = NULL,
									const Vi::Bbox<> &fBbox = Vi::Bbox<>());

    /**
     * Copy constructor.
     */
    GenericObject(const Vi::GenericObject &fOther);

    /**
     * Operator =.
     */
    Vi::GenericObject &operator=(const Vi::GenericObject &fOther);

    /**
     * Destructor.
     */
    ~GenericObject();

    /**
     * Calculate and update the object feature state.
     *
     * \param[in] fFrame The image data for which the features must be calculated.
     */
    void calculateFeature(const Vi::Image<> &fFrame);

    /**
     * Compare two objects.
     *
     * \param[in] fOther The other object.
     *
     * \return A similarity value ranging from 0.0f to 1.0f where similarity
     *  value 0.0f means non-similar and 1.0f means similar, respectively.
     */
    float compare(const Vi::GenericObject &fOther);

    /// The object bounding box.
    Vi::Bbox<> mBbox; 

    /// The object feature description.
    Vi::ObjectFeature *mFeature;

    /// Tells if the object has to be removed (context filtered)  
    bool mShouldBeRemoved;
  };
}

#endif // GENERIC_OBJECT_HPP
