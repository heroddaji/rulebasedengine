/** \file
 * Helper classes and definitions.
 *
 * $Id: Misc.hpp 1940 2011-01-07 14:23:16Z kris $
 */

#ifndef MISC_HPP
#define MISC_HPP

#include <iostream>
#include <string>

namespace Vi
{
  /**
   * Calculate Greatest Common Divisor (gcd). Uses the euclidian
   *  algorithm. Modified from
   *  en.wikipedia.org/wiki/Euclidean_algorithm.
   *
   * \param fA
   * \param fB
   *
   * \return gcd of fA and fB
   */
  template <class T>
  T gcd(T fA, T fB)
  {
    T t;

    while (fB != 0)
    {
      t = fB;
      fB = fA % fB;
      fA = t;
    }
    return fA;
  }

  /// Square value.
  template <class T>
  inline T sqr(T x)
  {
    return x * x;
  }

  /// Fraction.
  template <class T = unsigned int>
  class Frac
  {
  public:

    Frac<T>(T fNum, T fDen)
      : mNum(fNum),
        mDen(fDen)
    {
      simplify();
    }

    Frac<T>(T fNum)
      : mNum(fNum),
        mDen(1)
    {
      simplify();
    }

    Frac<T>()
      : mNum(0),
        mDen(1) // Shouldn't be zero
    {
      simplify();
    }

    Frac<T> operator+(const Frac<T> &fOther) const
    {
      return Frac<T>(((this->mNum * fOther.mDen) + (fOther.mNum * this->mDen)), (this->mDen * fOther.mDen));
    }

    Frac<T> operator-(const Frac<T> &fOther) const
    {
      return Frac<T>(((this->mNum * fOther.mDen) - (fOther.mNum * this->mDen)), (this->mDen * fOther.mDen));
    }

    Frac<T> &operator+=(const Frac<T> &fOther)
    {
      mNum = (this->mNum * fOther.mDen) + (fOther.mNum * this->mDen);
      mDen = (this->mDen * fOther.mDen);

      simplify();

      return (*this);
    }

    Frac<T> &operator-=(const Frac<T> &fOther)
    {
      mNum = (this->mNum * fOther.mDen) - (fOther.mNum * this->mDen);
      mDen = (this->mDen * fOther.mDen);

      simplify();

      return (*this);
    }

    Frac<T> operator*(const Frac<T> &fOther) const
    {
      return Frac<T>((this->mNum * fOther.mNum), (this->mDen * fOther.mDen));
    }

    Frac<T> &operator*=(const Frac<T> &fOther)
    {
      mNum *= fOther.mNum;
      mDen *= fOther.mDen;

      simplify();

      return (*this);
    }

    Frac<T> operator/(const Frac<T> &fOther) const
    {
      return Frac<T>((this->mNum * fOther.mDen), (this->mDen * fOther.mNum));
    }

    Frac<T> &operator/=(const Frac<T> &fOther)
    {
      mNum *= fOther.mDen;
      mDen *= fOther.mNum;

      simplify();

      return (*this);
    }

    bool operator==(const Frac<T> &fOther) const
    {
      return ((this->mNum == fOther.mNum) && (this->mDen == fOther.mDen));
    }

    bool operator!=(const Frac<T> &fOther) const
    {
      return ((this->mNum != fOther.mNum) && (this->mDen != fOther.mDen));
    }

    bool operator>(const Frac<T> &fOther) const
    {  
      return ((this->mNum * fOther.mDen) > (fOther.mNum * this->mDen));
    }

    bool operator<(const Frac<T> &fOther) const
    {  
      return ((this->mNum * fOther.mDen) < (fOther.mNum * this->mDen));
    }

    bool operator>=(const Frac<T> &fOther) const
    {  
      return ((this->mNum * fOther.mDen) >= (fOther.mNum * this->mDen));
    }

    bool operator<=(const Frac<T> &fOther) const
    {  
      return ((this->mNum * fOther.mDen) <= (fOther.mNum * this->mDen));
    }

    /**
     * Convert the fraction into a floating point number.
     *
     * \return The converted floating point result.
     */
    float toFloat() const
    {
      return static_cast<float>(mNum) / static_cast<float>(mDen);
    }

    /**
     * Exchange the numerator with the denominator (i.e. 1/[fraction]).
     */
    void invert()
    {
      std::swap(mNum, mDen);
    }

    /**
     * Get a fraction with exchanged numerator/denominator (i.e. 1/[fraction]).
     *
     * \return An inverted version of the fraction.
     */
    Frac<T> inverted()
    {
      return Vi::Frac<T>(mDen, mNum);
    }

    friend std::ostream &operator<<(std::ostream &fOs, const Frac<T> &fFrac)
    {
      fOs << fFrac.mNum << "/" << fFrac.mDen;
      return fOs;
    }
    
    T mNum; ///< Numerator.
    T mDen; ///< Denominator.

  private:

    void simplify()
    {
      T theGcd = gcd(mNum, mDen);

      mNum /= theGcd;
      mDen /= theGcd;
    }
  };
  
  /// Point.
  template <class T>
  struct Point
  {
    Point<T>(T x_, T y_) : x(x_), y(y_) {}

    Point<T>() : x(0), y(0) {}

    Point<T> operator+(const Point<T> &fOther) const
    {
      Point<T> ret;
      ret.x = x + fOther.x;
      ret.y = y + fOther.y;

      return ret;
    }

    Point<T> &operator+=(const Point<T> &fOther)
    {
      x += fOther.x;
      y += fOther.y;

      return (*this);
    }

    Point<T> operator-(const Point<T> &fOther) const
    {
      Point<T> ret;
      ret.x = x - fOther.x;
      ret.y = y - fOther.y;

      return ret;
    }

    Point<T> &operator-=(const Point<T> &fOther)
    {
      x -= fOther.x;
      y -= fOther.y;

      return (*this);
    }

    /// Whack a Point object right into a cout stream.
    friend std::ostream &operator<<(std::ostream &fOs, const Point<T> &fPoint)
    {
      fOs << "[" << fPoint.x << ", " << fPoint.y << "]";
      return fOs;
    }

    T x;
    T y;
  };

  template <class T>
  T dist(const Point<T> &a, const Point<T> &b)
  {
    return sqrt( sqr(a.x - b.x) + sqr(a.y - b.y) );
  }

  /**
   * Clip value.
   *
   * \param[in] fLow  Lower bound.
   * \param[in] fVal  Value to clip.
   * \param[in] fHigh Higher bound.
   *
   * \return Clipped value.
   */
  template <class T>
  inline T clip(T fLow, T fVal, T fHigh)
  {
    if (fVal < fLow) fVal = fLow; else if (fVal > fHigh) fVal  = fHigh;
    return fVal;
  }

#ifdef _WIN32
  const char PATH_SEP = '\\';
#else
  const char PATH_SEP = '/';
#endif

  /// Get path portion of a file name.
  inline std::string getFileName(const std::string &file)
  {
    return file.substr(file.find_last_of(PATH_SEP)+1);
  }
  
  /// Get path portion of a file name.  
  inline std::string getPath(const std::string &file)
  {
    return file.substr(0, file.find_last_of(PATH_SEP)+1);
  }

  /// Get basename portion of a file name.
  inline std::string getBasename(const std::string &file)
  {
    return file.substr(file.find_last_of(PATH_SEP)+1);
  }

  /// Get path portion of a file name.
  inline std::string getBase(const std::string &file)
  {
    return file.substr(0, file.find_last_of('.'));
  }

  /// Convert the path separators in a path to the native format.
  inline std::string convPathSep2Native(std::string in)
  {
    for (unsigned int i = 0; i < in.size(); i++)
      if (in[i] == '\\' || in[i] == '/') in[i] = PATH_SEP;
    return in;
  }
}

#endif // MISC_HPP
