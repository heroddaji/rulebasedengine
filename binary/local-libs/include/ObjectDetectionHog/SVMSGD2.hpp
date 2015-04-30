#ifndef __INCLUDE_SVMSGD2_H__
#define __INCLUDE_SVMSGD2_H__

//#include "timer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <map>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include <vector>

// BOOST Serialization.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/tracking.hpp> // because this class has derived classes.
#include <boost/serialization/vector.hpp>		// for serializing STL vectors.


//#define SGD_COUT_DEBUG //! Define to get debug output in std::cout.



// For shuffling the training data, we cannot loose our knowledge of the label
// of the training sample. Since the initial CTrainingSet only contains this 
// info indirectly, we need to store it explicitly.
class CTrainSample
{
public:
  std::vector<float> data;
  float label;
};


namespace SVMSGD2
{

// Available losses
#define HINGELOSS 1
#define SMOOTHHINGELOSS 2
#define SQUAREDHINGELOSS 3
#define LOGLOSS 10
#define LOGLOSSMARGIN 11

// Select loss
#define LOSS SMOOTHHINGELOSS

// Zero when no bias
// One when bias term
#define BIAS 1

// One when bias is regularized
#define REGULARIZEBIAS 0


inline float loss(float z);

inline float dloss(float z);

class SvmSgd
{
public:
  
  //! Constructor.
  SvmSgd() {}

  //! Initialization.
  void init(int f_dim, float f_lambda);
  
  //! Train the classifier on the provided samples.
  void train(
    int                           f_start, 
    int                           f_stop, 
    std::vector< CTrainSample > & f_samples);

  //! Test the classifier on the provided samples.
  void test(
    int                           f_start, 
    int                           f_stop, 
    std::vector< CTrainSample > & f_samples);

  //! Get bias value.
  void getBias(float & f_bias) 
  { 
    f_bias = m_bias; 
  }
  
  //! Copy SVM weights.
  void getWeights(float * f_weights);
  
private:

  //! Global time index.
  float               m_t;

  //! Number of dimensions.
  int                 m_dimensions;

  //! Lambda for decay factor.
  float               m_lambda;

  //! Classification function normal weights.
  std::vector<float>  m_w;

  //! Bias of current classification function.
  float               m_bias;

  //! Bias scale factor at t=0.
  float               m_bscale;

  //! Number of samples before weight vector is scaled.
  int                 m_skip;


private:

	//! Boost serialization.
	friend class boost::serialization::access;
	
	//! Boost serialization.
	template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
	{
    ar & m_dimensions;
		ar & m_lambda;
    ar & m_bias;
    ar & m_bscale;
    ar & m_skip;
    ar & m_w;
    ar & m_t;
	}

};

} // namespace SVMSGD2

#endif // __INCLUDE_SVMSGD2_H__