#ifndef __CLASSIFIER_H__
#define __CLASSIFIER_H__

#include "Feature.hpp" // CFeatureResponseList
#include <vector>
#include <list>

// BOOST Serialization.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/tracking.hpp> // because this class has derived classes.
#include <boost/serialization/vector.hpp>		// for serializing STL vectors.
#include <boost/serialization/list.hpp>		// for serializing STL lists.



//////////////////////////////////////////////////////////////////////////////

class CTrainingSet
{
public:

	//! Positive training samples.
	std::list< CFeatureResponseList > featureVectorsPositive;

	//! Negative training samples.
	std::list< CFeatureResponseList > featureVectorsNegative;

public:

	//! Constructor.
	CTrainingSet() {}

	//! Destructor.
	~CTrainingSet() {}


private:

	//! Boost serialization.
	friend class boost::serialization::access;
	
	//! Boost serialization.
	template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
	{
		ar & featureVectorsPositive;
		ar & featureVectorsNegative;
	}
};
	
//////////////////////////////////////////////////////////////////////////////

class IClassifierSVM
{
protected:
	int dimensions;

public:

  //! Creation of the classifier, checks which implementation should be used.
  //! This is specified in globalConfig.classifier.svm.
  static bool getClassifier(IClassifierSVM ** f_svm);

	//! Load classifier from file.
	virtual bool loadClassifer(char * f_file) = 0;

  //! Save classifier to file.
  virtual bool saveClassifer(char * f_file) = 0;

	//! Train the SVM.
	virtual bool train(
		CTrainingSet * f_trainSet) = 0;

	//! Test the SVM.
	virtual float test(
		float * f_sample) = 0;

	//! Gets number of feature dimensions.
	virtual bool getNrFeatures(int & f_nrFeatures) = 0;

	//! Copy SVM weight values for all dimensions.
	virtual bool getWeights(float * f_weights) = 0;

	//! Gets bias value for trained SVM.
	virtual bool getBias(float & f_bias) = 0;

	//! Set the number of dimensions (should only be called before testing the classifier
	//! since it is automatically extracted from the trainingset during training).
	void setDimensions(int f_dimensions) 
	{
		dimensions = f_dimensions;
	}

  //! Virtual destructor, so that implementations of IClassifierSVM can 
	//! be deleted using the IClassifierSVM interface.
	virtual ~IClassifierSVM() {};


private:

  //! Boost serialization.
  friend class boost::serialization::access;
	
	//! Boost serialization.
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) 
  {
    ar & dimensions;
  }

}; // class IClassifierSVM

// Makes sure that the serialized objects of IClassifierSVM are being tracked
// internally by the Boost library.
BOOST_CLASS_TRACKING(IClassifierSVM, track_always)

// BOOST knows that IFeature is an abstract class.
//BOOST_SERIALIZATION_ASSUME_ABSTRACT(IClassifierSVM)


#endif //__CLASSIFIER_H__
