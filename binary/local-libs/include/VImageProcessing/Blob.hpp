/** \file
 * Blob prototype.
 *
 * $Id: Blob.hpp 1989 2011-01-18 15:41:31Z kris $
 */

#ifndef BLOB_HPP
#define BLOB_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>

#include <GenericObject/GenericObject.hpp>

namespace Vi
{
  /**
   * The blob class.
   */
  class EXPORT_TYPE Blob : public Vi::GenericObject
  {
  public:

    /// The blob mask image. This is a binary image (i.e. value 0 or 255 only).
    Vi::Image<> mMask;
  };
}

#endif // BLOB_HPP
