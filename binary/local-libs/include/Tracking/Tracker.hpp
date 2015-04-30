/** \file
 * Object tracker abstract base class definition.
 *
 * $Id: Tracker.hpp 1908 2010-12-07 16:38:20Z kris $
 */

#ifndef TRACKER_HPP
#define TRACKER_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>

namespace Vi
{
  /**
   * Tracker abstract base class.
   */
  class EXPORT_TYPE Tracker
  {
  public:

    /**
     * Virtual desctructor to allow for destruction using base pointers.
     */
    virtual ~Tracker() {}

    /**
     * Clone function.
     *
     * \return An instance of a derived implementation.
     */
    virtual Vi::Tracker* clone() const = 0;

    /**
     * Initialize the tracker.
     *
     * \param[in] fFrame Input frame.
     * \param[in]  fBbox Initialization bounding box (position and size).
     */
    virtual void init(const Vi::Image<> &fFrame, const Vi::Bbox<> &fBbox) = 0;

    /**
     * Update the tracker.
     *
     * \param[in]    fFrame Input frame.
     * \param[in,out] fBbox Current/predicted bounding box location.
     */
    virtual void update(const Vi::Image<> &fFrame, Vi::Bbox<> &fBbox) = 0;
  };
}

#endif // TRACKER_HPP
