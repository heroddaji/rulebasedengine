/** \file
 * Video file output class header file.
 *
 * $Id: VideoOutputVideoFile.hpp 2015 2011-01-27 14:28:13Z rick $
 */

#ifndef VIDEO_OUTPUT_VIDEO_FILE_HPP
#define VIDEO_OUTPUT_VIDEO_FILE_HPP

#include <string.h>
#include <vector>
#include <deque>

//multithreading stuff
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>

#include "VideoOutput.hpp"
#include "Image.hpp"
#include "ThreadedBuffer.hpp"
#include "Compat.hpp"
#include "Misc.hpp"

struct AVCodec;
struct AVCodecContext;
struct AVStream;
struct AVFormatContext;
struct AVFrame;

namespace Vi
{
  /**
   * Write video to a file
   *
   * This class uses FFmpeg (ffmpeg.mplayerhq.hu) for its implementation.
   */
  class EXPORT_TYPE VideoOutputVideoFile : public VideoOutput
  {
  public:

    /// Constructor.
    VideoOutputVideoFile();

    /// Destructor.
    ~VideoOutputVideoFile();

    /**
     * Constructor.
     *
     * \param[in] fWidth    Width in pixels.
     * \param[in] fHeight   Height in pixels.
     * \param[in] fFilename File name of the output video file.
     * \param[in] fFps      Frame rate in frames per second.
     * \param[in] fBitrate  Bit rate of the video stream in bits per second.
     */
    VideoOutputVideoFile(unsigned int fWidth,
                         unsigned int fHeight,
                         const std::string &fFilename,
                         Vi::Frac<> fFps = Vi::Frac<>(25),
                         unsigned int fBitrate = 5000000);

    /**
     * Open video file for writing.
     *
     * \param[in] fWidth    Width in pixels.
     * \param[in] fHeight   Height in pixels.
     * \param[in] fFilename File name of the output video file.
     * \param[in] fFps      Frame rate in frames per second.
     * \param[in] fBitrate  Bit rate of the video stream in bits per second.
     */
    void open(unsigned int fWidth,
              unsigned int fHeight,
              const std::string &fFilename,
              Vi::Frac<> fFps = Vi::Frac<>(25),
              unsigned int fBitrate = 5000000);

    /**
     * Set encoder.
     *
     * \param[in] fCodecId  Encoder string, use <TT>ffmpeg -codecs</TT> at the
     *                       command line to see the different codecs.
     */
    VideoOutputVideoFile &setEncoder(const std::string &fCodecId);

    /**
     * Set interlaced video coding optimizations.
     *
     * \param[in] fInterlace True to enable, false to disable false.
     */
    VideoOutputVideoFile &setInterlace(bool fInterlace);

    /**
     * Set the size of the frame queue.
     *
     * \param[in] fQueueSize Size to set.
     */   
    VideoOutputVideoFile &setQueueSize(unsigned int fQueueSize);

    /**
     * Get opened state of the instance.
     *
     * \return True if a file is already opened, false if not.
     */
    bool isOpened();

    void close();
    bool write(const Vi::Image<> &image);

  private:

    /**
     * Write a video frame
     *
     * \param[in] fImage The image to be written to the video file.
     *
     * \return true on succes, false of failure     
     */
    bool frameWrite(const Vi::Image<> &fImage);

    /**
     * This thread continuously writes frames to disk when they become
     * available in the queue.
     */
    void writerThread();

    /**
     * Add a video output stream.
     *
     * \param[in] fWidth    Width in pixels.
     * \param[in] fHeight   Height in pixels.
     * \param[in] fBitrate  Bit rate of the video stream in bits per second.
     * \param[in] fFps      Frame rate in frames per second.     
     */
    void addVideoStream(unsigned int fWidth, unsigned int fHeight, unsigned int fBitrate, Vi::Frac<> fFps);

    /**
     * Open the video codec and and allocate the encode buffers.
     */
    void openVideo();

    AVFormatContext *mFormatCtx; ///< Format context.
    AVCodecContext *mCodecCtx;   ///< Codec context.

    uint8_t *mVideoOutbuf;      ///< Buffer to store compressed packet in.
    size_t mVideoOutbufSize;    ///< Size of mVideoOutbuf buffer.

    AVCodec *mEncoderToUse;     ///< Encoder to use for encoding the video.
    unsigned int mEncoderFlags; ///< Encoder flags.

    boost::mutex mCoutMutex; ///< Cout console output mutex.
    boost::thread mThrd;     ///< Video file writer thread.

    Vi::Image<> mImage; ///< ViNotion image alias for mFrame.
    AVFrame *mFrame;    ///< FFmpeg frame.

    unsigned int mQueueSize;                  ///< Number of frames the queue holds.
    ThreadedBuffer<Vi::Image<> > mFrameQueue; ///< Queue to store frames that are to be written.
    unsigned int mNumberOfFramesWritten;      ///< Number of frames written by the thread.
  };
}
#endif // VIDEO_OUTPUT_VIDEO_FILE_HPP
