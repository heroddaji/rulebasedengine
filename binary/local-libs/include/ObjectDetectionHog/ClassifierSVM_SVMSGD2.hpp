#ifndef __CLASSIFIERSVM_SVMSGD2__
#define __CLASSIFIERSVM_SVMSGD2__

#include "Classifier.hpp"
#include "SVMSGD2.hpp"

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>		// for serializing STL vectors.


class CClassifierSVM_svmSGD2 : public IClassifierSVM
{
public:

	//! Gets number of feature dimensions.
	bool getNrFeatures(int & f_nrFeatures);

	//! Gets pointer to the SVM weights.
	bool getWeights(float * f_weights);

	//! Gets bias value for trained SVM.
	bool getBias(float & f_bias);

private:

  //! The SGD svm model.
  SVMSGD2::SvmSgd * m_model;

  //! SGD learning rate.
  float m_lambda;

  //! Number of training iterations.
  int m_epochs;

  //! Number of training samples.
  int m_trainSamples;

  //! Classifier bias.
  float m_bias;

  //! SVM weights.
  std::vector< float > m_weights;


private:

	//! Boost serialization.
  friend class boost::serialization::access;
	
	//! Serialization function for saving objects.
	template<class Archive>
	  void save(Archive & ar, const unsigned int version) const
  {
    ar & dimensions;
    ar & m_model;
    ar & m_lambda;
    ar & m_epochs;
 		ar & m_trainSamples;
  }

	//! Serialization function for loading objects.
	template<class Archive>
		void load(Archive & ar, const unsigned int version)
  {
    ar & dimensions;
    ar & m_model;
    ar & m_lambda;
    ar & m_epochs;
 		ar & m_trainSamples;

    // Get weights and bias from model.
    m_weights.resize(dimensions);
    m_model->getWeights(&(m_weights[0]));
    m_model->getBias(m_bias);
  }

  //! Tell BOOST serialization that serialization function is split 
  //! in load/save functions.
  BOOST_SERIALIZATION_SPLIT_MEMBER()


	//! Allocates memory for the kernels and the SVMs.
	bool init();


public:

	//! Constructor.
	CClassifierSVM_svmSGD2();

	//! Load classifier from file.
	bool loadClassifer(char * f_file);

	//! Save classifier to file.
	bool saveClassifer(char * f_file);

	//! Train the SVM.
  //! Note that incremental (online) is allowed, in contrast to most 
  //! SVM implementations.
	bool train(
		CTrainingSet * f_trainSet);

	//! Test the SVM.
	float test(
		float * f_sample);

	//! Destructor.
  ~CClassifierSVM_svmSGD2() {}


}; // class CClassifierSVM_svmSGD2


#endif // __CLASSIFIERSVM_SVMSGD2__
