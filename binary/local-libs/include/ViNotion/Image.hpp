/** \file
 * Image container class header file.
 *
 * $Id: Image.hpp 1931 2010-12-30 12:27:19Z kris $
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>

#include "Array2D.hpp"

namespace Vi
{
  /// Image pixel formats used by Vi::Image.   
  enum PIXEL_FORMAT
  {
    PF_UNKNOWN,   ///< Unknown format.
    PF_YCC444P,   ///< Planar Y'CbCr 4:4:4 also known as Y'CbCr 6:6:6.
    PF_YCCA4444P, ///< Planar Y'CbCrA 4:4:4:4.
    PF_RGB,       ///< Packed RGB 4:4:4.
    PF_RGBA,      ///< Packed RGBA 4:4:4:4.
    PF_Y,         ///< Grayscale.
    PF_YAP,       ///< Planar grayscale with alpha.
    PF_JFIF,      ///< Packed JFIF Y'CbCr 4:4:4.
    PF_JFIF422P,  ///< Planar JFIF Y'CbCr 4:2:2.
    PF_YCC420P,   ///< Planar Y'CbCr 4:2:0.
    PF_BGR,       ///< Packed BGR.
    PF_YUY2,      ///< YUY2
    PF_SIZE       ///< Number of elements in enumeration.
  };

  /// ViNotion image container class.
  template<class T = uint8_t>
  class Image
  {
  public:

    // Pixel accessors / mutators.

    /// Y plane pixel accessor / mutator.
    T & Y(unsigned int fX, unsigned int fY) { return mPixData[0](fX, fY); };
    /// Y plane pixel accessor.
    const T & Y(unsigned int fX, unsigned int fY) const { return mPixData[0](fX, fY); };
    /// Cb plane pixel accessor / mutator.
    T & Cb(unsigned int fX, unsigned int fY) { return mPixData[1](fX, fY); };
    /// Cb plane pixel accessor.
    const T & Cb(unsigned int fX, unsigned int fY) const { return mPixData[1](fX, fY); };
    /// Cr plane pixel accessor / mutator.
    T & Cr(unsigned int fX, unsigned int fY) { return mPixData[2](fX, fY); };
    /// Cr plane pixel accessor.
    const T & Cr(unsigned int fX, unsigned int fY) const { return mPixData[2](fX, fY); };
    /// A plane pixel accessor / mutator.
    T & A(unsigned int fX, unsigned int fY) { return mPixData[3](fX, fY); };
    /// A plane pixel accessor.
    const T & A(unsigned int fX, unsigned int fY) const { return mPixData[3](fX, fY); };

    // Row accessors / mutators.

    /// Y plane row accessor / mutator.
    T * & Y(unsigned int fY) { return mPixData[0](fY); };
    /// Y plane row accessor.
    const T * Y(unsigned int fY) const { return mPixData[0](fY); };
    /// Cb plane row accessor / mutator.
    T * & Cb(unsigned int fY) { return mPixData[1](fY); };
    /// Cb plane row accessor.
    const T * Cb(unsigned int fY) const { return mPixData[1](fY); };
    /// Cr plane row accessor / mutator.
    T * & Cr(unsigned int fY) { return mPixData[2](fY); };
    /// Cr plane row accessor.
    const T * Cr(unsigned int fY) const { return mPixData[2](fY); };
    /// A plane row accessor / mutator.
    T * & A(unsigned int fY) { return mPixData[3](fY); };
    /// A plane row accessor.
    const T * A(unsigned int fY) const { return mPixData[3](fY); };

    /// Image accessor / mutator (packed formats)
    T & operator() (unsigned int fX, unsigned int fY){ return mPixData[0](fX, fY); };
    /// Image accessor (packed formats)
    const T & operator() (unsigned int fX, unsigned int fY) const { return mPixData[0](fX, fY); };

    /// Image row accessor / mutator (packed formats)
    T * & operator() (unsigned int fY){ return mPixData[0](fY); };
    /// Image row accessor (packed formats)
    const T * operator() (unsigned int fY) const { return mPixData[0](fY); };

    /// Pixel format accessor.
    Vi::PIXEL_FORMAT fmt() const { return mFormat; };

    /// Pixel format mutator.
    void fmt(Vi::PIXEL_FORMAT fFmt);

    /// Size in elements of a macropixel.
    unsigned int mps() const { return mMpixelSize; };

    /// Width accessor.
    unsigned int w() const { return mWidth; };
    /// Height accessor.
    unsigned int h() const { return mHeight; };
    
    /**
     * Constructor.
     *
     * \param[in] fFmt Pixel format to use. The default is PF_YCC444P.
     */
    Image(Vi::PIXEL_FORMAT fFmt = PF_YCC444P);

    /**
     * Resize of image. Because the pixel format is stored, the correct
     *  amount of memory is allocated.
     *
     * \param fW Image width.
     * \param fH Image height.
     *
     * \throw This function will throw an error if attempting to resize a
     * hasMem = false Array2D.
     */
    void resize(unsigned int fW, unsigned int fH);

    /**
     * Size of image. Because the pixel format is stored, the correct
     *  amount of memory is allocated.
     *
     * \param fW      Image width.
     * \param fH      Image height.
     * \param fHasMem True if the array should do its own memory allocation.
     */
    void size(unsigned int fW, unsigned int fH, bool fHasMem = true);

    /**
     * Extends data from subimage to fill entire image.
     */
    void extendSubimage();

    /**
     * Set subimage of an image.
     *
     * \param[in] fXo x-origin.
     * \param[in] fYo y-origin.
     * \param[in] fW  Width.
     * \param[in] fH  Height.     
     */
    void pushSubimage(int fXo, int fYo, unsigned int fW, unsigned int fH) const;

    /**
     * Copy subimage from other subimage.
     *
     * \param[in] fSrc Image containing subimage.
     * \param[in] fXo  x-origin in this image.
     * \param[in] fYo  y-origin in this image.
     *
     */
    void subcopy(const Vi::Image<T> & fSrc, int fXo, int fYo);

    /**
     * Pop subImage to previous subimage state
     */
    void popSubimage() const;

    /**
     * Clear image. Optionally, a fill color can be defined on a per-channel
     *  basis (default: black).
     * 
     * \param[in] fFill0 The fill color for channel 0.
     * \param[in] fFill1 The fill color for channel 1.
     * \param[in] fFill2 The fill color for channel 2.
     * \param[in] fFill3 The fill color for channel 3.
     */
    void clear(T fFill0 = 0,
               T fFill1 = 128,
               T fFill2 = 128,
               T fFill3 = 0);

    /// Set frame index.
    void setFrameIndex(uint64_t fIdx);

    /// Get frame index.
    uint64_t getFrameIndex() const;

    Array2D<T> mPixData[4];  ///< 3 component channels + alpha channel.

  private:
    Vi::PIXEL_FORMAT mFormat; ///< Pixel format.
    mutable unsigned int mWidth;  ///< Width.
    mutable unsigned int mHeight; ///< Height.

    /// Size in elements of a macropixel (packed formats).
    unsigned int mMpixelSize; 

    /// Number of pixels per macropixel. mMpixelSize / mPixelsPerMpixel = #elements per pixel.
    unsigned int mPixelsPerMpixel;

    bool mIsPlanar;       ///< Planar / packed format.
    unsigned int mHss[4]; ///< Horizontal subsampling factors for each channel.
    unsigned int mVss[4]; ///< Vertical subsampling factors for each channel.
    uint64_t mIndex;      ///< Frame index.
  };
}

template<class T>
void Vi::Image<T>::resize(unsigned int fW, unsigned int fH)
{
  mWidth = fW;
  mHeight = fH;

  if (mIsPlanar) // Planar format.
  {
    for (unsigned int i = 0; i < 4; i++)
      if (mHss[i]) mPixData[i].resize(mWidth / mHss[i], mHeight / mVss[i]);   
  }
  else // Packed format.
  {
    mPixData[0].resize(mWidth * mMpixelSize / mPixelsPerMpixel, mHeight);
  }  
}

template<class T>
void Vi::Image<T>::size(unsigned int fW, unsigned int fH, bool fHasMem)
{
  mWidth = fW;
  mHeight = fH;

  if (mIsPlanar) // Planar format.
  {
    for (unsigned int i = 0; i < 4; i++)
      if (mHss[i]) mPixData[i].size(mWidth / mHss[i], mHeight / mVss[i], fHasMem);
  }
  else // Packed format.
  {
    mPixData[0].size(mWidth * mMpixelSize / mPixelsPerMpixel, mHeight, fHasMem);
  }   
}

template<class T>
Vi::Image<T>::Image(Vi::PIXEL_FORMAT fFmt) :
  mFormat(PF_UNKNOWN), mWidth(0), mHeight(0), mMpixelSize(0),
  mPixelsPerMpixel(0), mIsPlanar(true)
{
  mHss[0] = mHss[1] = mHss[2] = mHss[3] = 0;
  mVss[0] = mVss[1] = mVss[2] = mVss[3] = 0;
  fmt(fFmt);
}

template<class T>
void Vi::Image<T>::fmt(Vi::PIXEL_FORMAT fFmt)
{  
  mFormat = fFmt;

  switch (mFormat)
  {
  case PF_YCCA4444P: case PF_YCC444P: case PF_Y: case PF_YAP: case PF_YCC420P: case PF_JFIF422P:
    mIsPlanar = true;
    break;
  case PF_RGB: case PF_BGR: case PF_RGBA: case PF_JFIF: case PF_YUY2:
    mIsPlanar = false;
    break;
  default:
    throw std::logic_error("[Image<T>::Image]: Unknown format.");
  }

  // Planar color formats.
  if (mIsPlanar)
  {
    mMpixelSize = 1;
    mPixelsPerMpixel = 1;

    // Y channel.
    switch (mFormat)
    {
    case PF_YCCA4444P: case PF_YCC444P: case PF_Y: case PF_YAP: case PF_YCC420P: case PF_JFIF422P:
      mHss[0] = mVss[0] = 1;
      break;
    default:
      throw std::logic_error("[Image<T>::fmt]: Unknown format");
    }
    // Cb and Cr channel.
    switch (mFormat)
    {
    case PF_YCCA4444P: case PF_YCC444P:
      mHss[1] = mHss[2] = 1;
      mVss[1] = mVss[2] = 1;
      break;
    case PF_YCC420P:
      mHss[1] = mHss[2] = 2;
      mVss[1] = mVss[2] = 2;
      break;
    case PF_Y: case PF_YAP:
      mHss[1] = mHss[2] = 0;
      mVss[1] = mVss[2] = 0;
    case PF_JFIF422P:
      mHss[1] = mHss[2] = 2;
      mVss[1] = mVss[2] = 1;
      break;
    default:
      throw std::logic_error("[Image<T>::fmt]: Unknown format");
    }
    // Alpha channel.
    switch (mFormat)
    {
    case PF_YCCA4444P: case PF_YAP:
      mHss[3] = mVss[3] = 1;
      break;
    case PF_YCC444P: case PF_Y: case PF_YCC420P: case PF_JFIF422P:
      mHss[3] = mVss[3] = 0;
      break;
    default:
      throw std::logic_error("[Image<T>::fmt]: Unknown format.");
    }
  }
  else
  {
    // Packed color formats.
    mHss[0] = mVss[0] = 1;
    mHss[1] = mHss[2] = mHss[3] = 0;
    mVss[1] = mVss[2] = mVss[3] = 0;

    switch (mFormat)
    {
    case PF_RGB: case PF_BGR: case PF_JFIF:
      mMpixelSize = 3;
      mPixelsPerMpixel = 1;
      break;
    case PF_RGBA:
      mMpixelSize = 4;
      mPixelsPerMpixel = 1;
      break;
    case PF_YUY2:
      mMpixelSize = 4;
      mPixelsPerMpixel = 2;
      break;
    default:
      throw std::logic_error("[Image<T>::fmt]: Unknown format");
    }
  }
}

template<class T>
void Vi::Image<T>::extendSubimage()
{
  for (unsigned int i = 0; i < 4; i++)
    if (mHss[i]) mPixData[i].extendSubarray();
}

template <class T>
void Vi::Image<T>::pushSubimage(int fXo, int fYo, unsigned int fW, unsigned int fH) const
{
  if (mIsPlanar) // Planar format.
  {
    for (unsigned int i = 0; i < 4; i++)
      if (mHss[i]) mPixData[i].pushSubarray(fXo / mHss[i], fYo / mVss[i], fW / mHss[i], fH / mVss[i]);
  }
  else // Packed format.
  {
    mPixData[0].pushSubarray(fXo * mMpixelSize / mPixelsPerMpixel, fYo, fW * mMpixelSize / mPixelsPerMpixel, fH);
  } 

  mWidth = fW;
  mHeight = fH;
}

template <class T>
void Vi::Image<T>::subcopy(const Vi::Image<T> & fSrc, int fXo, int fYo)
{
  if (mIsPlanar) // Planar format.
  {
    for (unsigned int i = 0; i < 4; i++)
      if (mHss[i]) mPixData[i].subcopy(fSrc.mPixData[i], fXo / mHss[i], fYo / mVss[i]);
  }
  else // Packed format.
  {
    mPixData[0].subcopy(fSrc.mPixData[0], fXo * mMpixelSize / mPixelsPerMpixel, fYo);
  } 
}

template <class T>
void Vi::Image<T>::popSubimage() const
{
  for (unsigned int i = 0; i < 4; i++)
    if (mHss[i]) mPixData[i].popSubarray();
 
  if(mIsPlanar) // Planar format.
    mWidth = mPixData[0].w();
  else // Packed format.
    mWidth = mPixData[0].w()*mPixelsPerMpixel/mMpixelSize;

  mHeight = mPixData[0].h();
}

template <class T>
void Vi::Image<T>::clear(T fFill0,
                         T fFill1,
                         T fFill2,
                         T fFill3)
{
  if (mHss[0]) mPixData[0].fill(fFill0);
  if (mHss[1]) mPixData[1].fill(fFill1);
  if (mHss[2]) mPixData[2].fill(fFill2);
  if (mHss[3]) mPixData[3].fill(fFill3);
}

template <class T>
void Vi::Image<T>::setFrameIndex(uint64_t fIdx)
{
  mIndex = fIdx;
}

template <class T>
uint64_t Vi::Image<T>::getFrameIndex() const
{
  return mIndex;
}

#endif // IMAGE_HPP
