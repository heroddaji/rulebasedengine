/** \file
 * Running average based background model implementation.
 *
 * $Id: BackgroundModelRunAvg.cpp 1922 2010-12-21 16:21:26Z kris $
 */

#include "BackgroundModelRunAvg.hpp"
#include "BackgroundModelRunAvgParams.hpp"

Vi::BackgroundModelRunAvg::BackgroundModelRunAvg(unsigned int fFrameWidth,
                                                 unsigned int fFrameHeight)
  : mFrameWidth(fFrameWidth),
    mFrameHeight(fFrameHeight),
    mFrameCounter(0),
    mBgLearnFrames(100),
    mFgLearnRate(1000),
    mPixDiffThreshold(75)
{
  // Initialize the background mask in YUV - binary mask
  mBackgroundMaskYUV.fmt(Vi::PF_YCC444P);
  mBackgroundMaskYUV.size(mFrameWidth, mFrameHeight);
  mBackgroundMaskYUV.clear();

  // Initialize the background image for display
  mBackgroundImage.fmt(Vi::PF_YCC444P);
  mBackgroundImage.size(mFrameWidth, mFrameHeight);
  mBackgroundImage.clear();

  // Initialize the background image (32-bit integer) for display
  mBackgroundImageInt32.fmt(Vi::PF_YCC444P);
  mBackgroundImageInt32.size(mFrameWidth, mFrameHeight);
  mBackgroundImageInt32.clear();
}

void Vi::BackgroundModelRunAvg::process(const Vi::Image<> &fFrame,
                                        const Vi::Image<> &fObjectMaskYUV)
{
  // First, increment the frame counter
  if (mFrameCounter < mBgLearnFrames)
    mFrameCounter++;

  // Build the background model
  for (unsigned int y = 0; y < mFrameHeight; y++)
  {
    // Get current frame row pointers
    const uint8_t *fFrameYp = fFrame.Y(y);
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
    const uint8_t *fFrameCbp = fFrame.Cb(y);
    const uint8_t *fFrameCrp = fFrame.Cr(y);
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

    // Get background model row pointers
    int *bgModelYp = mBackgroundImageInt32.Y(y);
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
    int *bgModelCbp = mBackgroundImageInt32.Cb(y);
    int *bgModelCrp = mBackgroundImageInt32.Cr(y);
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

    // Get background mask Y row pointer
    uint8_t *bgMaskYp = mBackgroundMaskYUV.Y(y);

    unsigned int x = 0;
    while (bgModelYp < mBackgroundImageInt32.Y(y) + mFrameWidth)
    {
      // Calculate the division factor
      int divFactor = (fObjectMaskYUV.Y(x, y) == 255 ?
          static_cast<int>(mFgLearnRate) : static_cast<int>(mFrameCounter));

      //
      // --- (1) Update the background model ----------------------------------
      //
      *bgModelYp += ((*fFrameYp << 22) - *bgModelYp) / divFactor;
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
      *bgModelCbp += ((*fFrameCbp << 22) - *bgModelCbp) / divFactor;
      *bgModelCrp += ((*fFrameCrp << 22) - *bgModelCrp) / divFactor;
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

      //
      // --- (2) Update the background mask (and update the pointers) ---------
      //
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
      *bgMaskYp++ = (std::abs(((*fFrameYp++ << 22) - *bgModelYp++) +
                              ((*fFrameCbp++ << 22) - *bgModelCbp++) +
                              ((*fFrameCrp++ << 22) - *bgModelCrp++))
          > mPixDiffThreshold << 22 ? 255 : 0);
#else
      *bgMaskYp++ = (std::abs((*fFrameYp++ << 22) - *bgModelYp++)
          > mPixDiffThreshold << 22 ? 255 : 0);
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

      x++;
    }
  }
}

void Vi::BackgroundModelRunAvg::buildBackgroundImage()
{
  // Build the background image
  for (unsigned int y = 0; y < mFrameHeight; y++)
  {
    // Get background image row pointers
    uint8_t *bgImageYp = mBackgroundImage.Y(y);
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
    uint8_t *bgImageCbp = mBackgroundImage.Cb(y);
    uint8_t *bgImageCrp = mBackgroundImage.Cr(y);
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

    // Get background model row pointers
    const int *bgModelYp = mBackgroundImageInt32.Y(y);
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
    const int *bgModelCbp = mBackgroundImageInt32.Cb(y);
    const int *bgModelCrp = mBackgroundImageInt32.Cr(y);
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO

    while (bgModelYp < mBackgroundImageInt32.Y(y) + mFrameWidth)
    {
      *bgImageYp++ = *bgModelYp++ >> 22;
#ifdef BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
      *bgImageCbp++ = *bgModelCbp++ >> 22;
      *bgImageCrp++ = *bgModelCrp++ >> 22;
#endif // BACKGROUND_MODEL_RUN_AVG_USE_COLOR_INFO
    }
  }
}

void Vi::BackgroundModelRunAvg::configure(const Vi::Settings &fConfig)
{
  Vi::Parameter tmpParam;

  // Set background learning frames
  if (fConfig.getParameter("BgSubRunAvg_bgLearnFrames", tmpParam))
    tmpParam.getValue(mBgLearnFrames);

  // Set foreground learning rate
  if (fConfig.getParameter("BgSubRunAvg_fgLearnRate", tmpParam))
    tmpParam.getValue(mFgLearnRate);

  // Set pixel difference threshold
  if (fConfig.getParameter("BgSubRunAvg_pixDiffThreshold", tmpParam))
    tmpParam.getValue(mPixDiffThreshold);
}
