/** \file
 * Compatibility macros to use the same code on different platforms/compilers.
 *
 * $Id: Compat.hpp 1963 2011-01-14 13:05:35Z kris $
 */

#ifndef COMPAT_HPP
#define COMPAT_HPP

#ifdef __GNUC__
#define ALWAYS_INLINE __attribute__((always_inline))
#define DEPRECATED __attribute__((deprecated))
#else
#define ALWAYS_INLINE
#define DEPRECATED
#endif

// Please clean up.
#if defined(LIBVINOTION_DLL) || defined(USING_DLL)
#ifdef DLL_EXPORT
#define EXPORT_TYPE __declspec(dllexport)
#else
#define EXPORT_TYPE __declspec(dllimport)
#endif
#else
#define EXPORT_TYPE
#endif

// Stuff for Visual C++ only.
#ifdef _MSC_VER
// Call _snprintf() for VC++ version of snprintf() function.
#define snprintf _snprintf

namespace std
{
  namespace tr1
  {
    inline long lround(double d)
    {
      return long(d >= 0 ? d + 0.5 : d - 0.5);
    }
  
    inline double round(double d)
    {
      return double(long((d >= 0 ? d + 0.5 : d - 0.5)));
    }
  }
}
#endif // _MSC_VER

#ifdef _WIN32
#define M_PI_2 1.57079632679489661923 /* pi/2 */
#else
#include <tr1/cmath>
#endif

#endif // COMPAT_HPP
