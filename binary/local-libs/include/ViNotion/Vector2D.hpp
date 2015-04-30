/** \file
 * Simple 2D vector class.
 *
 * $Id: Vector2D.hpp 1963 2011-01-14 13:05:35Z kris $
 */

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <cmath>

#include "Misc.hpp"

namespace Vi
{
  /**
   * Simple 2D vector datatype. This class implements a vector starting at the
   *  origin pointing at a position in 2D space.
   */
  template <class T>
  struct Vector2D
  {
    /// Default constructor.
    Vector2D<T>() : p() {}

    /// Constructor.
    Vector2D<T>(Vi::Point<T> p_) : p(p_) {}

    /// Constructor.
    Vector2D<T>(T x, T y) : p(Vi::Point<T>(x, y)) {}

    /// Operator '+'.
    Vector2D<T> operator+(const Vector2D<T> &fRhs) const
    {
      return Vector2D<T>(p + fRhs.p);
    }

    /// Operator '+='.
    Vector2D<T> &operator+=(const Vector2D<T> &fRhs)
    {
      p += fRhs.p;

      return (*this);
    }

    /// Operator '-'.
    Vector2D<T> operator-(const Vector2D<T> &fRhs) const
    {
      return Vector2D<T>(p - fRhs.p);
    }

    /// Operator '-='.
    Vector2D<T> &operator-=(const Vector2D<T> &fRhs)
    {
      p -= fRhs.p;

      return (*this);
    }

    /// Operator '*' (constant factor multiplication).
    Vector2D<T> operator*(const float &fFactor) const
    {
      return Vector2D<T>(p.x * fFactor, p.y * fFactor);
    }

    /// Operator '*=' (constant factor multiplication).
    Vector2D<T> &operator*=(const float &fFactor)
    {
      p.x *= fFactor;
      p.y *= fFactor;

      return (*this);
    }

    /**
     * Get a point in 2D space from the vector.
     *
     * \return The point in 2D space.
     */
    Vi::Point<T> toPoint() const
    {
      return p;
    }

    /**
     * Get the vector length.
     *
     * \return The vector length.
     */
    float length() const
    {
      return sqrtf(p.x * p.x + p.y * p.y);
    }

    /**
     * Get the vector direction.
     *
     * \return The vector direction in [radians].
     */
    float direction() const
    {
      return atan2f(p.y, p.x);
    }

    Vi::Point<T> p; ///< Vector end point.
  };
}

#endif // VECTOR2D_HPP
