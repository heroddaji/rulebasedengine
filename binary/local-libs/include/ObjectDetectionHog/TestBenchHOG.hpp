#ifndef __TESTBENCH_HOG_HPP__
#define __TESTBENCH_HOG_HPP__

#define TB_HOG_FILENAME_PIXELVALUES	  "tb_hog_img.txt"
#define TB_HOG_FILENAME_GRADIENT_RAW	"tb_hog_gradient_raw.txt"
#define TB_HOG_FILENAME_GRADIENT_PROC	"tb_hog_gradient_proc.txt"
#define TB_HOG_FILENAME_FEAT			    "tb_hog_feat.txt"
#define TB_HOG_FILENAME_FEAT_NORM		  "tb_hog_feat_norm.txt"
#define TB_HOG_FILENAME_SLID_WIN		  "tb_hog_slidwin.txt"

#include <fstream>


//! Testbench writing functionality.
class CTestBenchHOG
{
private:

  //! Output filestream for input pixel values.
  std::ofstream * m_osInputPixel;

  //! Output filestream for raw gradient values.
  //! The gradients are the raw gradients from atan2().
  //! The processed gradients are stored in m_osGradientProc.
  std::ofstream * m_osGradientRaw;

  //! Output filestream for gradient values.
  //! Processed gradients, raw values are stored m_osGradientRaw.
  std::ofstream * m_osGradientProc;

  //! Output filestream for HOG feature values.
  std::ofstream * m_osHOG;

  //! Output filestream for Normalized HOG feature values.
  std::ofstream * m_osHOGNorm;

  //! Output filestream for output of the sliding window detector.
  std::ofstream * m_osHOGSlidWin;

public:
  
  //! Value of previous sample, for vertical alignment.
  int m_yInputPixel;
  //! Value of previous sample, for vertical alignment.
  int m_yGradient;
  //! Value of previous sample, for vertical alignment.
  int m_yHOG;
  //! Value of previous sample, for vertical alignment.
  int m_yHOGNorm;
  //! Value of previous sample, for vertical alignment.
  int m_yHOGSlidWin;


private:

  //! Open files for writing testbench data.
  void open();

  //! Close files for writing.
  void close();

public:

  //! Constructor.
  CTestBenchHOG();

  //! Input pixel values.
  void addSampleInputPixel(
    int f_y, int f_x, int f_channel, unsigned char f_value);

  //! Gradient raw values.
  void addSampleGradientRaw(
    int f_y, int f_x, float f_gradOri, float f_gradMag);

  //! Gradient processed values.
  void addSampleGradientProc(
    int f_y, int f_x, float f_gradOri, float f_gradMag);

  //! Unnormalized HOG features, no block normalization applied yet.
  void addSampleHOG(
    int f_y, int f_x, int f_orientationBin, float f_value);

  //! HOG features with block normalization applied.
  void addSampleHOGNorm(
    int f_y, int f_x, int f_orientationBin, int f_blockNorm, float f_value);

  //! Sliding window detection scores.
  void addSampleHOGSlidWin(
    int f_y, int f_x, float f_value);

  //! Destructor.
  ~CTestBenchHOG();

};

extern CTestBenchHOG gTbHOG; // static global object.


#endif//__TESTBENCH_HOG_HPP__
