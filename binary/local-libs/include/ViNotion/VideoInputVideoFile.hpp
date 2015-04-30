/** \file
 * Header file for the video file input class. Uses FFMPEG (http://ffmpeg.org/).
 * 
 * $Id: VideoInputVideoFile.hpp 1847 2010-11-12 13:54:35Z rick $
 */

#ifndef VIDEO_INPUT_VIDEO_FILE_HPP
#define VIDEO_INPUT_VIDEO_FILE_HPP

#include "Image.hpp"
#include "VideoInput.hpp"
#include "Compat.hpp"
#include "Misc.hpp"
 
struct AVFormatContext;
struct AVCodecContext;
struct AVInputFormat;
struct AVFrame;

namespace Vi
{
  /**
   * Input video from file
   *
   * This class uses FFmpeg (ffmpeg.mplayerhq.hu) for its implementation.
   */
  class EXPORT_TYPE VideoInputVideoFile : public VideoInput
  {
  public:
    /**
     * Constructor.
     */
    VideoInputVideoFile();

    /**
     * Constructor.
     *
     * \param[in] fFileName Filename of the video file to open.     
     */
    VideoInputVideoFile(const std::string &fFileName);

    /**
     * Open video file.
     *
     * \param[in] fFileName Filename of the video file to open.     
     */
    void open(const std::string &fFileName);

    /**
     * Close video file.
     */
    void close();

    /// Destructor.
    ~VideoInputVideoFile();

    bool read(Image<> &fImage);
    uint64_t seek(int64_t fOffset, VideoPosId fWhence = VPI_SEEK_CUR);
    uint64_t tell() const { return mNextFramePts; }
    bool eof() const { return (mNextFramePts == getLength()); }
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    uint64_t getLength() const;
    Vi::VideoInputVideoFile &setLoop(bool fState = true);

    /**
     * Force input format.
     *
     * \param[in] fFormat Format string, use "ffmpeg -formats" at the
     *                     command line to see the different
     *                     formats. Omitting this argument or setting
     *                     it to "" switches on format auto-detecting
     *                     (default).
     */
    Vi::VideoInputVideoFile &setFormat(const std::string &fFormat = "");

    /**
     * Get the input video frame rate.
     *
     * \return The (fractional) framerate.
     */
    Vi::Frac<int> getFrameRate() const;

  private:
    AVFormatContext *mFormatCtx; ///< Format context pointer.
    AVCodecContext *mCodecCtx;   ///< Codec context pointer.
    AVInputFormat *mInputFormat; ///< Input format to use.
    int mVideoStream;            ///< Index of the video stream to decode.
    AVFrame *mFrame;             ///< Picture to store decodec frame in.
    Vi::Image<> mImage;          ///< ViNotion image alias for mFrame.
    uint64_t mNextFramePts;      ///< PTS of the next frame to read.
    bool mLoop;                  ///< Looping flag.
  };
}

#endif // VIDEO_INPUT_VIDEO_FILE_HPP
