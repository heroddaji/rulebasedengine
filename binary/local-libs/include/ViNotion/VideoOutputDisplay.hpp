/** \file
 * Video and keyboard I/O header file.
 *
 * $Id: VideoOutputDisplay.hpp 1697 2010-09-13 12:25:53Z kris $
 */

#ifndef VIDEO_OUTPUT_DISPLAY_HPP
#define VIDEO_OUTPUT_DISPLAY_HPP

#include <string>
#include <fstream>

#include "Image.hpp"
#include "VideoOutput.hpp"

class SDL_Surface;
class SDL_Overlay;
class SDL_Rect;

namespace Vi
{
  /**
   * Output video to the screen.
   *
   * This class uses SDL (www.libsdl.org) for video output and uses its YUV
   *  overlay interface to benefit from hardware YUV-to-RGB conversion and
   *  hardware scaling.
   */
  class EXPORT_TYPE VideoOutputDisplay : public VideoOutput
  {
  public:
    /**
     * Constructor.
     *
     * \param[in] fWinWidth     The window width in pixels.
     * \param[in] fWinHeight    The window height in pixels.
     * \param[in] fImageWidth   The displayed image width in pixels.
     * \param[in] fImageHeight  The displayed image height in pixels.
     * \param[in] fFs           Full screen mode state (optional).
     */
    VideoOutputDisplay( unsigned int fWinWidth,
                        unsigned int fWinHeight,
                        unsigned int fImageWidth,
                        unsigned int fImageHeight,
                        bool fFs = false);

    /**
     * Destructor.
     */
    ~VideoOutputDisplay();

    /**
     * Write the output image.
     *
     * \note Odd image dimensions are not handled very well.
     *
     * \param[in] fImg The input image.
     *
     * \return True if success, otherwise false.
     */
    bool write(const Vi::Image<> &fImg);

    /**
     * Set SDL window caption.
     *
     * \param[in] fStr The caption string.
     */
    void setCaption(const std::string &fStr);

    /**
     * Toggle fullscreen mode.
     *
     * \param[in] fState The new fullscreen state (true = fullscreen).
     */
    void toggleFullscreen(bool fState);

    /**
     * Get quit signal from the SDL window.
     *
     * \return True if the quit signal was given. Otherwise false.
     */
    bool getQuit();

    /**
     * Get quit signal from the SDL window.
     *
     * \return True if the quit signal was given. Otherwise false.
     */
    bool getPaused();

    /**
     * Set the video display to paused mode.
     *
     * \param[in] fState Pause state (optional).
     */
    void setPaused(bool fState = true);

  private:

    /// Set video mode.
    void setMode();

    /// Poll for video events and process them.
    void videoPollEvents();

    SDL_Surface *mSurface;  ///< SDL surface.
    SDL_Overlay *mScreen;   ///< SDL screen.
    SDL_Rect *mRectangle;   ///< SDL rectangle.

    unsigned int mWidth;    ///< Image width.
    unsigned int mHeight;   ///< Image width.
    unsigned int mOvw;      ///< Image width.
    unsigned int mOvh;      ///< Image width.

    unsigned int mWidthFS;  ///< Fullscreen width.
    unsigned int mHeightFS; ///< Fullscreen height.

    bool mFullscreen;       ///< Full screen flag.

    bool mQuit;     ///< Quit signal flag.
    bool mOnestep;  ///< One-step mode.
    bool mPaused;   ///< Pause mode.
  };
}

#endif // VIDEO_OUTPUT_DISPLAY_HPP
