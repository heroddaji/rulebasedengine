/** \file
 * Bounding box template class implementation.
 *
 * $Id: Bbox.hpp 1963 2011-01-14 13:05:35Z kris $
 */

#ifndef BBOX_HPP
#define BBOX_HPP

#include <iostream>

#include "Misc.hpp"

namespace Vi
{
  /**
   * Bounding box class.
   *
   * <PRE>
   *  p1 -- p3
   *   |    |
   *  p4 -- p2
   * </PRE>
   */
  template <class T = int>
  class Bbox
  {
  public:

    Vi::Point<T> p1;  ///< Top-left corner point.
    Vi::Point<T> p2;  ///< Bottom-right corner point (inside the bounding box).

    /// Default constructor.
    Bbox() : p1(), p2() {}

    /// Copy constructor.
    Bbox(const Vi::Bbox<T> &fOther) : p1(fOther.p1), p2(fOther.p2) {}

    /**
     * Constructor.
     *
     * \param[in] fX0 Top-left corner X-coordinate.
     * \param[in] fY0 Top-left corner Y-coordinate.
     * \param[in] fX1 Bottom-right corner X-coordinate.
     * \param[in] fY1 Bottom-right corner Y-coordinate.
     */
    Bbox(T fX0, T fY0, T fX1, T fY1) : p1(Vi::Point<T>(fX0, fY0)), p2(Vi::Point<T>(fX1, fY1)) {}

    /**
     * Constructor.
     *
     * \param[in] fP1 Top-left corner point.
     * \param[in] fP2 Bottom-right corner point.
     */
    Bbox(const Vi::Point<T> &fP1, const Vi::Point<T> &fP2) : p1(fP1), p2(fP2) {}

    /**
     * Get top-left corner point.
     *
     * \return Top-left corner point.
     */
    Vi::Point<T> topLeft() const
    {
      return p1;
    }

    /**
     * Get bottom-right corner point.
     *
     * \return Bottom right corner point.
     */
    Vi::Point<T> bottomRight() const
    {
      return p2;
    }

    /**
     * Get the top-right corner point.
     *
     * \return Top-right corner point.
     */
    Vi::Point<T> topRight() const
    {
      return Vi::Point<T>(p2.x, p1.y);
    }

    /**
     * Get bottom-left corner point.
     *
     * \return Bottom-left corner point.
     */
    Vi::Point<T> bottomLeft() const
    {
      return Vi::Point<T>(p1.x, p2.y);
    }
  
    /**
     * Get the midpoint of bounding box.
     *
     * \return The midpoint of the bounding box.
     */
    Vi::Point<T> mid() const
    {
      return Vi::Point<T>((p2.x + p1.x) / 2, (p2.y + p1.y) / 2);
    }

    /**
     * Returns the bounding box width.
     *
     * \return The width of the bounding box.
     */
    T w() const
    {
      return p2.x - p1.x;
    }

    /**
     * Returns the bounding box height.
     *
     * \return The height of the bounding box.
     */
    T h() const
    {
      return p2.y - p1.y;
    }

    /**
     * Returns the bounding box center x-coordinate.
     *
     * \return The center x-coordinate of the bounding box.
     */
    T x() const
    {
      return (p2.x + p1.x) / 2;
    }

    /**
     * Returns the bounding box center y-coordinate.
     *
     * \return The center y-coordinate of the bounding box.
     */
    T y() const
    {
      return (p2.y + p1.y) / 2;
    }

    /**
     * Returns the area of the bounding box.
     *
     * \return The area of the bounding box.
     */
    T area() const
    {
      return (w() * h());
    }

    /**
     * Returns the radius of the bounding box in the X-direction.
     *
     * \return The radius of the bounding box in the X-direction.
     */
    T radiusX() const
    {
      return (w() / 2);
    }

    /**
     * Returns the radius of the bounding box in the Y-direction.
     *
     * \return The radius of the bounding box in the Y-direction.
     */
    T radiusY() const
    {
      return (h() / 2);
    }

    /**
     * Returns the union of the bounding box with another bounding box.
     *
     * \param[in] fOtherBbox The other bounding box.
     *
     * \return The union of the two bounding boxes.
     */
    Bbox<T> united(const Bbox<T> &fOtherBbox) const
    {
      Bbox<T> retBbox;
      retBbox.p1.x = std::min(p1.x, fOtherBbox.p1.x); // Left side
      retBbox.p1.y = std::min(p1.y, fOtherBbox.p1.y); // Top side
      retBbox.p2.x = std::max(p2.x, fOtherBbox.p2.x); // Right side
      retBbox.p2.y = std::max(p2.y, fOtherBbox.p2.y); // Bottom side

      return retBbox;
    }

    /**
     * Returns the intersection of the bounding box with another bounding box.
     *
     * \param[in] fOtherBbox The other bounding box.
     *
     * \return The intersection of the two bounding boxes.
     */
    Bbox<T> intersected(const Bbox<T> &fOtherBbox) const
    {
      Bbox<T> retBbox;
      retBbox.p1.x = std::max(p1.x, fOtherBbox.p1.x); // Left side
      retBbox.p1.y = std::max(p1.y, fOtherBbox.p1.y); // Top side
      retBbox.p2.x = std::min(p2.x, fOtherBbox.p2.x); // Right side
      retBbox.p2.y = std::min(p2.y, fOtherBbox.p2.y); // Bottom side

      // Check whether there is an intersection
      if ((retBbox.p1.x < retBbox.p2.x) && (retBbox.p1.y < retBbox.p2.y))
        return retBbox;   // Return the intersection
      else
        return Bbox<T>(); // Return an empty bounding box
    }

    /**
     * Set bounding box preserving the width and height.
     *
     * \param[in] fX The X-coordinate of the bounding box position.
     * \param[in] fY The Y-coordinate of the bounding box position.
     */
    void setPos(const T &fX, const T &fY)
    {
      p2.x = fX + (p2.x - p1.x);
      p2.y = fY + (p2.y - p1.y);
      p1.x = fX;
      p1.y = fY;
    }

    /**
     * Adjust bounding box position, preserves width and height.
     *
     * \param[in] fOffX The X-coordinate offset of the bounding box position.
     * \param[in] fOffY The Y-coordinate offset of the bounding box position.
     */
    void adjustPos(const T &fOffX, const T &fOffY)
    {
      this->adjust(fOffX, fOffY, fOffX, fOffY);
    }

    /**
     * Adjust the bounding box using offset values.
     *
     * \param[in] fOffX1 The offset of the left side.
     * \param[in] fOffY1 The offset of the top side.
     * \param[in] fOffX2 The offset of the right side.
     * \param[in] fOffY2 The offset of the bottom side.
     */
    void adjust(const T &fOffX1, const T &fOffY1, const T &fOffX2, const T &fOffY2)
    {
      p1.x += fOffX1; // Left side
      p1.y += fOffY1; // Top side
      p2.x += fOffX2; // Right side
      p2.y += fOffY2; // Bottom side
    }

    /**
     * Returns an adjusted bounding box using offset values.
     *
     * \param[in] fOffX1 The offset of the left side.
     * \param[in] fOffY1 The offset of the top side.
     * \param[in] fOffX2 The offset of the right side.
     * \param[in] fOffY2 The offset of the bottom side.
     * \return An adjusted version of the bounding box.
     */
    Bbox<T> adjusted(const T &fOffX1, const T &fOffY1, const T &fOffX2, const T &fOffY2) const
    {
      Bbox<T> retBbox;
      retBbox.p1.x = p1.x + fOffX1; // Left side
      retBbox.p1.y = p1.y + fOffY1; // Top side
      retBbox.p2.x = p2.x + fOffX2; // Right side
      retBbox.p2.y = p2.y + fOffY2; // Bottom side

      return retBbox;
    }

    /**
     * Expand the bounding box.
     *
     * \param[in] fOffset The offset value with which to expand the bounding box.
     */
    void expand(const T &fOffset)
    {
      this->adjust(-fOffset, -fOffset, fOffset, fOffset);
    }

    /**
     * Expand the bounding box.
     *
     * \param[in] fOffX The X-offset value with which to expand.
     * \param[in] fOffY The Y-offset value with which to expand.
     */
    void expand(const T &fOffX, const T &fOffY)
    {
      this->adjust(-fOffX, -fOffY, fOffX, fOffY);
    }

    /**
     * Returns an expanded a bounding box.
     *
     * \param[in] fOffset The offset value with which to expand the bounding box.
     *
     * \return An expanded version of the bounding box.
     */
    Bbox<T> expanded(const T &fOffset) const
    {
      return this->adjusted(-fOffset, -fOffset, fOffset, fOffset);
    }

    /**
     * Shrink the bounding box.
     *
     * \param[in] fOffset The offset value with which to shrink the bounding box.
     */
    void compress(const T &fOffset)
    {
      this->adjust(fOffset, fOffset, -fOffset, -fOffset);
    }

    /**
     * Returns a shrunk bounding box.
     *
     * \param[in] fOffset The offset value with which to shrink the bounding box.
     *
     * \return A compressed version of the bounding box.
     */
    Bbox<T> compressed(const T &fOffset) const
    {
      return this->adjusted(fOffset, fOffset, -fOffset, -fOffset);
    }

    /**
     * Dice measure.
     *
     * \note This function implements the "dice" measure for two bounding boxes.
     * \note Return (2*overlapping_area)/(area box1 + area box2).
     * 
     * \param[in] fOtherBbox The other bounding box.
     *
     * \return The dice measure (range [0.0f..1.0f]).
     */
    float dice(const Bbox<T> &fOtherBbox) const
    {
      float thisArea = this->area();
      float otherArea = fOtherBbox.area();

      if ((thisArea == 0) && (otherArea == 0))
        return 0.0f;

      return (2.0f * (this->intersected(fOtherBbox).area()) / (thisArea + otherArea));
    }

    /**
     * Clip a bounding box to image boundaries.
     *
     * \param[in] fW The input width to clip to.
     * \param[in] fH The input height to clip to.
     */
    void clip(const T &fW, const T &fH)
    {
      if (p1.x < 0)  p1.x = 0;
      if (p1.y < 0)  p1.y = 0;
      if (p1.x > fW) p1.x = fW;
      if (p1.y > fH) p1.y = fH;
      if (p2.x < 0)  p2.x = 0;
      if (p2.y < 0)  p2.y = 0;
      if (p2.x > fW) p2.x = fW;
      if (p2.y > fH) p2.y = fH;
    }

    /**
     * Clip a bounding box to image boundaries.
     *
     * \param[in] fW The input width to clip to.
     * \param[in] fH The input height to clip to.
     *
     * \return A clipped version of the bounding box.
     */
    Bbox<T> clipped(const T &fW, const T &fH) const
    {
      Bbox<T> retBbox(*this);
      retBbox.clip(fW, fH);

      return retBbox;
    }

    /**
     * Check whether the current bounding box contains the given bounding box.
     *
     * \param[in] fOtherBbox The other bounding box.
     *
     * \return True if the current bounding box contains the given bounding box. Otherwise false.
     */
    bool contains(const Bbox<T> &fOtherBbox) const
    {
      if ((fOtherBbox.p1.x >= p1.x) &&
          (fOtherBbox.p1.y >= p1.y) &&
          (fOtherBbox.p2.x <= p2.x) &&
          (fOtherBbox.p2.y <= p2.y))
        return true;
      else
        return false;
    }

    /**
     * Check whether the current bounding box is contained in the given bounding box.
     *
     * \param[in] fOtherBbox The other bounding box.
     *
     * \return True if the current bounding box is contained in the given bounding box. Otherwise false.
     */
    bool isContained(const Bbox<T> &fOtherBbox) const
    {
      if ((p1.x >= fOtherBbox.p1.x) &&
          (p1.y >= fOtherBbox.p1.y) &&
          (p2.x <= fOtherBbox.p2.x) &&
          (p2.y <= fOtherBbox.p2.y))
        return true;
      else
        return false;
    }

    /**
     * Scale a bounding box, keeping the bounding box center static.
     *
     * \param[in] fScaleFactor The scale factor with which to scale the bounding box.
     */
    void scale(const Vi::Frac<T> &fScaleFactor)
    {
      T widthAdjust = (w() * fScaleFactor.mNum) / (fScaleFactor.mDen * 2);
      T heightAjust = (h() * fScaleFactor.mNum) / (fScaleFactor.mDen * 2);

      this->adjust(-widthAdjust, -heightAjust, widthAdjust, heightAjust);
    }

    /**
     * Return a scaled bounding box, keeping the bounding box center static.
     *
     * \param[in] fScaleFactor The scale factor with which to scale the bounding box.
     *
     * \return A scaled version of the bounding box.
     */
    Bbox<T> scaled(const Vi::Frac<T> &fScaleFactor) const
    {
      T widthAdjust = (w() * fScaleFactor.mNum) / (fScaleFactor.mDen * 2);
      T heightAjust = (h() * fScaleFactor.mNum) / (fScaleFactor.mDen * 2);

      return this->adjusted(-widthAdjust, -heightAjust, widthAdjust, heightAjust);
    }

    /// Whack a Bbox object right into a cout stream.
    friend std::ostream &operator<<(std::ostream &fOs, const Bbox<T> &fBbox)
    {
      fOs << fBbox.p1 << ":" << fBbox.p2;
      return fOs;
    }
  };
}

#endif // BBOX_HPP
