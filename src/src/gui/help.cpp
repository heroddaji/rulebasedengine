
#include <QTextStream>
namespace helper
{
void writeDefautlVirtualFecingIni(QTextStream &stream)
{
  
  stream << "[General]\n";

  stream << "General_enableRecording = false\n";
  stream << "General_enableRecording = bool\n";

  stream << "General_recordProcessed = false \n";
  stream << "General_recordProcessed = bool\n";

  stream << "; the base filename of the recorded raw sequences around events\n";
  stream << "General_recordFileNameRaw = test \n";
  stream << "General_recordFileNameRaw = string\n";
  stream << "; the base filename of the recorded processed sequences around events\n";
  stream << "General_recordFileNameProcessed = test_processed\n";
  stream << "General_recordFileNameProcessed = string\n";

  stream << "[BackgroundModelGMM]\n";

  stream << "; build background image\n";
  stream << "buildBackgroundImage = false\n";
  stream << "buildBackgroundImage = bool\n";

  stream << "; number of frames to learn the background before performing detection / tracking\n";
  stream << "BgSubGMM_nbLearningFrames = 0\n";
  stream << "BgSubGMM_nbLearningFrames = uint32_t\n";

  stream << "; The maximum number of components in the mixture.\n";
  stream << "BgSubGMM_MaxNbComponents = 5\n";
  stream << "BgSubGMM_MaxNbComponents = uint32_t\n";

  stream << "; The background update factor alpha.\n";
  stream << "BgSubGMM_Alpha = 0.0005\n";
  stream << "BgSubGMM_Alpha = float\n";

  stream << "; The threshold (i.e. factor on the variance) for including a pixel on the background.\n";
  stream << "BgSubGMM_ThresholdModeBackground = 100.0\n";
  stream << "BgSubGMM_ThresholdModeBackground = float\n";

  stream << "; The threshold to tell if a mode should be considered as part of the background model.\n";
  stream << "BgSubGMM_ThresholdBackgroundModel = 0.9\n";
  stream << "BgSubGMM_ThresholdBackgroundModel = float\n";

  stream << "; The threshold (i.e. factor on the variance) for the mode update.\n";
  stream << "BgSubGMM_ThresholdModeUpdate = 9.0\n";
  stream << "BgSubGMM_ThresholdModeUpdate = float\n";

  stream << "; The prior cT.\n";
  stream << "BgSubGMM_CT = 0.05\n";
  stream << "BgSubGMM_CT = float\n";

  stream << "; The intial value for the variance of the gaussians in the GMM.\n";
  stream << "BgSubGMM_InitialVariance = 11.0\n";
  stream << "BgSubGMM_InitialVariance = float\n";

  stream << "[Morphology]\n";

  stream << "; X-radius of the structuring element\n";
  stream << "Morphology_stucturingElementRadiusX = 2 \n";
  stream << "Morphology_stucturingElementRadiusX = uint32_t\n";

  stream << "; Y-radius of the structuring element\n";
  stream << "Morphology_stucturingElementRadiusY = 2\n";
  stream << "Morphology_stucturingElementRadiusY = uint32_t\n";

  stream << "[TrackedObjectVirtualFencing]\n";

  stream << "; the minimum allowed speed before an object is stopped as flagged\n";
  stream << "TOVF_minSpeedThreshold = 0.1\n";
  stream << "TOVF_minSpeedThreshold = float\n";

  stream << "[ContextFilter]\n";

  stream << "; minimum width of a detection to be considered\n";
  stream << "ContextFilter_minWidth = 10\n";
  stream << "ContextFilter_minWidth = uint32_t\n";

  stream << "; maximum width of a detection to be considered\n";
  stream << "ContextFilter_maxWidth = 200\n";
  stream << "ContextFilter_maxWidth = uint32_t\n";

  stream << "; minimum height of a detection to be considered\n";
  stream << "ContextFilter_minHeight = 10\n";
  stream << "ContextFilter_minHeight = uint32_t\n";

  stream << "; maximum height of a detection to be considered\n";
  stream << "ContextFilter_maxHeight = 200\n";
  stream << "ContextFilter_maxHeight = uint32_t\n";

  stream << "; if an object is a detection/object is that close to the border, it will be discarded\n";
  stream << "ContextFilter_borderOffset = 5\n";
  stream << "ContextFilter_borderOffset = uint32_t\n";

  stream << "; the number of frames before a stopped object is flushed into the background\n";
  stream << "ContextFilter_maxNbFramesBeforeFlush = 1000\n";
  stream << "ContextFilter_maxNbFramesBeforeFlush = uint32_t\n";

  stream << "[TripWire]\n";

  stream << "; the number of tripwires\n";
  stream << "numberOfTripWires = 2 \n";
  stream << "numberOfTripWires = uint32_t\n";

  stream << "; tripwire 1 point 1\n";
  stream << "TripWire0_point1 = 100,180\n";
  stream << "TripWire0_point1 = point \n";

  stream << "; tripwire 1 point2 \n";
  stream << "TripWire0_point2 = 200,120\n";
  stream << "TripWire0_point2 = point \n";

  stream << "; tripwire 2 point 2\n";
  stream << "TripWire1_point1 = 110,200\n";
  stream << "TripWire1_point1 = point \n";

  stream << "; tripwire 2 point 2\n";
  stream << "TripWire1_point2 = 250,240\n";
  stream << "TripWire1_point2 = point \n";

  stream << "[RuleBasedEngine]\n";

  stream << "; name of the mask where the restricted areas are defined\n";
  stream << "RuleBasedEngine_maskFileName = /home/dai/Dropbox/Graduation/rulebasedengine/trunk/data/mask.png\n";
  stream << "RuleBasedEngine_maskFileName = string \n";

  stream << "[ResultFile]\n";

  stream << "; name of the result text file to write events\n";
  stream << "ResultFile_FileName = /home/dai/Dropbox/Graduation/rulebasedengine/trunk/data/object_output.xml\n";
  stream << "ResultFile_FileName = string\n";

  stream << "[Recording]\n";

  stream << "; the number of seconds to record around the event\n";
  stream << "Recording_nbSecondsAroundEvent = 4 \n";
  stream << "Recording_nbSecondsAroundEvent = uint32_t\n";

  stream << "; the frame rate of the input\n";
  stream << "Recording_fpsInput = 25\n";
  stream << "Recording_fpsInput = uint32_t\n";

  stream << "; the bit rate of the ouptut video\n";
  stream << "Recording_bitRate = 5000000\n";
  stream << "Recording_bitRate = uint32_t\n";

  stream << "[IoBox]\n";

  stream << "; IP adress of the IObox\n";
  stream << "IOBox_host = 176.22.66.51\n";

  stream << "IOBox_DOAddress = 0\n";
  stream << "IOBox_DOAddress = uint32_t\n";

  stream << "IOBox_alarmState = true\n";
  stream << "IOBox_alarmState = bool\n";

  stream << "IOBox_hysteresis = 2\n";
  stream << "IOBox_hysteresis = uint32_t\n";
}
}
