/** \file
 * Template class for fast 2D array implementation.
 *
 * $Id: Array2D.hpp 1899 2010-12-03 12:45:50Z kris $
 */

#ifndef ARRAY2D_HPP
#define ARRAY2D_HPP

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace Vi
{
  /**
   * Fast 2D array. This template class omits all bounds checking. By
   * using the subarray interface subviews can be specified which, for
   * instance, can come in handy for filtering operations.
   */
  template<class T>
  class Array2D
  {
  public:
    /// Constructor.
    Array2D();

    /** 
     * Copy constructor. The copy constructor always produces a copy
     * with hasMem = true.
     */
    Array2D(const Vi::Array2D<T> &fCopy);    

    /// Destructor.
    ~Array2D();

    /// Get the array width.
    int w() const { return mSubarrays.back().width; };

    /// Get the array height.
    int h() const { return mSubarrays.back().height; };

    /// Get the bufferwidth (stride).
    int stride() const { return mBufferWidth; };

    // Parentheses operators.
    T& operator()(int x, int y)             { return mRowp[y][x]; };
    const T& operator()(int x, int y) const { return mRowp[y][x]; };
    T* & operator()(int y)                  { return mRowp[y];    };
    const T* operator()(int y) const        { return mRowp[y];    };

    /**
     * Assignment operator.
     *
     * \throw This function will throw if attempting to
     *  resize a left-hand side mHasMem = false Array2D.
     */
    Array2D<T>& operator=(const Vi::Array2D<T> &fRhs);

    /**
     * Set size of array and reset subarrays. Setting size will always
     *  be done, even if this discards manually set row pointers.
     *
     * \param[in] fW      Array width.
     * \param[in] fH      Array height.
     * \param[in] fHasMem True if the array should do its own memory allocation.
     */
    void size(unsigned int fW, unsigned int fH, bool fHasMem);

    /**
     * Set size of array and reset subarrays.
     *
     * \param[in] fW Array width.
     * \param[in] fH Array height.
     *
     * \throw This function will throw an error if attempting to
     *  resize a mHasMem = false Array2D.
     */
    void resize(unsigned int fW, unsigned int fH);

    /**
     * Create a new subarray in the current array. The new subarray
     *  settings are copied to a subarray settings stack, and by
     *  invoking popSubarray(), previous subarray settings can be
     *  retrieved. New settings are relative to the original array
     *  coordinate system.
     *
     * \param[in] fXo Subarray offset for the X-coordinate.
     * \param[in] fYo Subarray offset for the Y-coordinate.
     * \param[in] fW Width of the subarray, relative to its X-, Y-positions.
     * \param[in] fH Height of the subarray, relative to its X-, Y-positions.
     *
     * \throw This functions throws if the subarray falls out of the
     *  buffer bounds.
     */
    void pushSubarray(int fXo, int fYo, unsigned int fW, unsigned int fH) const;

    /**
     * Pop current subarray settings from the subarray settings stack,
     *  thereby retrieving the previous subarray settings. If current
     *  subarray settings are the last ones on the stack, no subarray
     *  can be popped and an exception is thrown.
     */
    void popSubarray() const;

    /**
     * Copy array data (from another array) into this array.
     *
     * \param[in] fSrc The source array from which the data is copied.
     * \param[in] fXo  New array data X-coordinate offset in this array.
     * \param[in] fYo  New array data Y-coordinate offset in this array.
     *
     * \throw This functions throws if the copy position falls out of
     *  the buffer bounds.
     */
    void subcopy(const Vi::Array2D<T> &fSrc, int fXo, int fYo);

    /**
     * Extend the borders from the subarray to cover the entire buffer.     
     */
    void extendSubarray();
    
    /**
     * Fill all elements of the array.
     *
     * \param[in] fFiller Filler value.
     */
    void fill(T fFiller);

  private:
    /// Structure to store subarray locations.
    struct Subarray
    {
      unsigned int width;
      unsigned int height;
      int xOrg;
      int yOrg;
    };

    unsigned int mBufferWidth;  ///< Buffer width.
    unsigned int mBufferHeight; ///< Buffer height.

    mutable std::vector<Subarray> mSubarrays; ///< Subarray stack within this array.
    mutable T **mRows;    ///< Row pointer array.
    mutable T **mRowp;    ///< Pointer inside mRows at subarray position y = 0.
    T *mData;     ///< Actual array data.
    bool mHasMem; ///< Does the object do its own memory allocation?
  
    /**
     * New row and data arrays.
     *
     * \param[in] fW Width of array.
     * \param[in] fH Height of array.
     * \param[in] fHasMem True if the array should do its own memory allocation.
     */
    void newData(unsigned int fW, unsigned int fH, bool fHasMem);

    /**
     * Adjust row pointers.
     *
     * \param fOffset Offset to add to each row pointer.
     */
    void adjustRows(int fOffset) const;

    /**
     * Initialize subarray vector with the root subarray and
     *  initialize the 0th row pointer.
     */
    void initSubarrays();
  };
}

template <class T>
void Vi::Array2D<T>::initSubarrays()
{
  Subarray sa;
  sa.width = mBufferWidth;
  sa.height = mBufferHeight;
  sa.xOrg = 0;
  sa.yOrg = 0;

  mSubarrays.clear();
  mSubarrays.push_back(sa);
  
  mRowp = mRows; 
}

template <class T>
void Vi::Array2D<T>::resize(unsigned int fW, unsigned int fH)
{
  if(mHasMem)
  {
    newData(fW, fH, true);
    initSubarrays();
  }
  else
  {
    if (mBufferWidth != fW && mBufferHeight != fH)
      throw std::logic_error("[Array2D<T>::resize]: Noalloc and trying to resize.");
  }
}

template <class T>
void Vi::Array2D<T>::size(unsigned int fW, unsigned int fH, bool fHasMem)
{
  newData(fW, fH, fHasMem);
  initSubarrays();
}

template <class T>
Vi::Array2D<T>::Array2D() :
  mBufferWidth(0),
  mBufferHeight(0),
  mRows(NULL),
  mRowp(NULL),
  mData(NULL),
  mHasMem(true)
{
  initSubarrays();
}

template <class T>
Vi::Array2D<T>::Array2D(const Vi::Array2D<T> &fCopy) :
  mBufferWidth(0),
  mBufferHeight(0),
  mRows(NULL),
  mRowp(NULL),
  mData(NULL),
  mHasMem(true)
{
  mSubarrays = fCopy.mSubarrays;
  newData(fCopy.mBufferWidth, fCopy.mBufferHeight, true);

  for (unsigned int y = 0; y < fCopy.mBufferHeight; y++)
    std::memcpy(mRows[y], fCopy.mRows[y] - fCopy.mSubarrays.back().xOrg, fCopy.mBufferWidth * sizeof(T));
  
  adjustRows(mSubarrays.back().xOrg);
}

template <class T>
void Vi::Array2D<T>::newData(unsigned int fW, unsigned int fH, bool fHasMem)
{
  if (fW == 0 || fH == 0) // Special case, allocate no memory.
  {
    delete [] mData;
    mData = NULL;
    delete [] mRows;
    mRows = NULL;
    return;
  }

  // Allocate row array memory.
  delete [] mRows;
  mRows = new T*[fH];

  if (fHasMem)
  {
    //if (!mHasMem || fW * fH > mBufferHeight * mBufferWidth)      
    {
      // Allocate data memory if previous state was mHasMem = false or
      // if the requested size is larger than the currently allocated
      // size.
      delete [] mData;
      mData = new T[fW * fH];

      for (unsigned int i = 0; i < fH; i++)
        mRows[i] = mData + i * fW;      
    }
  }
  else
  {
    // Do not allocate data memory.
    delete [] mData;
    mData = NULL;
    std::memset(mRows, NULL, sizeof(T*) * fH); // Clear row pointer array.
  }

  mBufferWidth = fW;
  mBufferHeight = fH;
  mHasMem = fHasMem;
}
  
template <class T>
void Vi::Array2D<T>::fill(T fFiller)
{
  // Does not use memcpy, because the type of T is unknown.
  for (unsigned int y = 0; y < mBufferHeight; y++)
    for (unsigned int x = 0; x < mBufferWidth; x++)
      mRows[y][static_cast<int>(x)-mSubarrays.back().xOrg] = fFiller;
}

template <class T>
Vi::Array2D<T>::~Array2D()
{ 
  delete [] mRows;
  delete [] mData;
}

template <class T>
Vi::Array2D<T> & Vi::Array2D<T>::operator=(const Vi::Array2D<T> &fRhs)
{
  // Prevent self-assignment.
  if (this == &fRhs)
    return *this;
  
  // Copy subarray settings.
  mSubarrays = fRhs.mSubarrays;

  if (mHasMem)
  {
    newData(fRhs.mBufferWidth, fRhs.mBufferHeight, true);
  }
  else
  {
    if (mBufferWidth != fRhs.mBufferWidth || mBufferHeight != fRhs.mBufferHeight)
      throw std::logic_error("[Array2D<T>::operator=]: Noalloc and trying to resize.");
  }

  for (unsigned int y = 0; y < mBufferHeight; y++)
    std::memcpy(mRows[y], fRhs.mRows[y] - fRhs.mSubarrays.back().xOrg, mBufferWidth * sizeof(T));
  
  adjustRows(mSubarrays.back().xOrg);
  
  return *this;
}

template <class T>
void Vi::Array2D<T>::extendSubarray()
{
  Subarray &sa = mSubarrays.back();

  // Top.
  for (int y = 0; y < sa.yOrg; y++)
    std::memcpy(mRows[y], mRows[sa.yOrg], sa.width*sizeof(T));

  // Bottom.
  for (int y = sa.yOrg + sa.height; y < mBufferHeight; y++)
    std::memcpy(mRows[y], mRows[sa.yOrg + sa.height - 1], sa.width*sizeof(T));

  // Left.
  for (unsigned int y = 0; y < mBufferHeight; y++)
    for (int x = 1; x <= sa.xOrg; x++)
      mRows[y][-x] = mRows[y][0];

  // Right.
  for (unsigned int y = 0; y < mBufferHeight; y++)
    for (int x = sa.width; x < mBufferWidth - sa.xOrg; x++)
      mRows[y][x] = mRows[y][sa.width - 1];
}

template <class T>
void Vi::Array2D<T>::pushSubarray(int fXo, int fYo, unsigned int fW, unsigned int fH) const
{
  Subarray sa = mSubarrays.back();
  sa.width = fW;
  sa.height = fH;
  sa.xOrg += fXo;
  sa.yOrg += fYo;

  // Bounds check.
  if (sa.xOrg < 0)
    throw std::logic_error("[Array2D<T>::pushSubarray]: Subarray left side out of bounds.");
  if (sa.yOrg < 0)
    throw std::logic_error("[Array2D<T>::pushSubarray]: Subarray top side out of bounds.");
  if (sa.xOrg + sa.width > mBufferWidth)
    throw std::logic_error("[Array2D<T>::pushSubarray]: Subarray right side out of bounds.");
  if (sa.yOrg + sa.height > mBufferHeight)
    throw std::logic_error("[Array2D<T>::pushSubarray]: Subarray bottom side out of bounds.");
  
  mSubarrays.push_back(sa);
  adjustRows(fXo);
}

template <class T>
void Vi::Array2D<T>::subcopy(const Vi::Array2D<T> &fSrc, int fXo, int fYo)
{
  // Bounds check.
  if (mSubarrays.back().xOrg + fXo < 0)
    throw std::logic_error("[Array2D<T>::subcopy]: Copy position left side out of bounds.");
  if (mSubarrays.back().yOrg + fYo < 0)
    throw std::logic_error("[Array2D<T>::subcopy]: Copy position top side out of bounds.");
  if (static_cast<unsigned int>(mSubarrays.back().xOrg + fXo + fSrc.w()) > mBufferWidth)
    throw std::logic_error("[Array2D<T>::subcopy]: Copy position right side out of bounds.");
  if (static_cast<unsigned int>(mSubarrays.back().yOrg + fYo + fSrc.h()) > mBufferHeight)
    throw std::logic_error("[Array2D<T>::subcopy]: Copy position bottom side out of bounds.");

  for (int y = 0; y < fSrc.h(); y++)
    std::memcpy((*this)(y+fYo) + fXo, fSrc(y), fSrc.w() * sizeof(T));
}

template <class T>
void Vi::Array2D<T>::popSubarray() const
{
  if (mSubarrays.size() <= 1)
    throw std::logic_error("[Array2D<T>::popSubarray]: Nothing to pop.");

  int last = mSubarrays.back().xOrg;
  mSubarrays.pop_back();

  adjustRows(mSubarrays.back().xOrg - last);
}

template <class T>
void Vi::Array2D<T>::adjustRows(int fOffset) const
{
  for (unsigned int i = 0; i < mBufferHeight; i++)
    mRows[i] += fOffset;

  if (mRows != NULL)
    mRowp = &mRows[mSubarrays.back().yOrg];
}

#endif // ARRAY2D_HPP
