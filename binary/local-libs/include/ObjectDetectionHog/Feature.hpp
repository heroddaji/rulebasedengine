#ifndef __FEATURE_HPP__
#define __FEATURE_HPP__

#include <cstdio>

#include <vector>

// BOOST Serialization includes.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/vector.hpp>		// For serializing STL vectors.


#include <boost/shared_ptr.hpp>

// Forward declarations.
class CInterestPoint;
class CInterestPointsList;
class CImageSource;
class CImageSourceList;
class CFeatureResponseList;


//! Feature description interface.
/*!
 *	IFeature object can be stored in a CFeatureList object, and are extracted 
 *	by a IFeatureGenerator. When comparing IFeature objects, the response 
 *	values (distances) can be stored in a CFeatureResponseList object.
 *
 *	When calculating the average, first call resetMemory(), then add all 
 *	samples using the operator+=() and then use normalize().
 */
class IFeature
{
private:

	//! Boost serialization.
    friend class boost::serialization::access;
	
	//! Boost serialization.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {}


public:

	//! Creates a copy of the derived object and returns its IFeature pointer.
	virtual IFeature * getCopy() = 0;


	//! Addition operator, used for average calculation.
	virtual IFeature & operator+=(const IFeature &patch) = 0;


	//! Calculates the distance between two features (actually, the matching 
	//! score is calculated).
	/*!
	 *	Distance value 0 means worst possible match, distance 1 means perfect 
	 *	match.
	 *
	 *	@param[in] f_data is the feature that is compared with the this object.
	 *	@param[out]	f_distance is the calculated distance value.
	 */
	virtual bool distance(
		IFeature  * f_data, 
		float	  & f_distance) = 0;


	//! Scaling, used for average calculation.
	virtual void scale(unsigned int f_number) = 0;


	//! Gets the size of the feature related to the input image (the width/
	//! height of the feature, so not the radius).
	virtual bool getInputSize(
		unsigned int & f_inputSize) = 0;


	//! Get scaling factor between two scales.
	/*!
	 *	If the scale of f_scale2 is larger than f_scale1, a value larger than 
	 *	one1 is stored in f_scalingFactor.
	 *
	 *	@param[in]	f_scale1 is the scale of the feature that should be 
	 *				compared to the f_scale2. 
	 *	@param[in]	f_scale2 is the scale of the feature that should be 
	 *				compared to the f_scale1. 
	 *	@param[in]	f_scalingFactor is the resulting scaling factor between
	 *				the two scales. Note that this truely depends on the 
	 *				feature implementation.
	 */
	virtual bool getRelativeScalingFactor(
		double			f_scale1,
		double			f_scale2,
		float		  & f_scalingFactor) = 0;


	//! Returns false if the feature does not contain any useful information
	//! and true if it does.
	virtual bool isValid() = 0;


	//! Normalization to zero mean, unit variance.
	virtual void normalize() = 0;


	//! Memory reset, used in average calculation.
	virtual void resetMemory() = 0;


	//! Removes the data that is related to the input image, 
	//! like the raw input pixels, position, etc.
	virtual void removeInputReferences() = 0;


	//! Virtual destructor, so that implementations of IFeature can be deleted 
	//! using the IFeature interface.
	virtual ~IFeature() {};


};

// Makes sure that the serialized objects of IFeature are being tracked
// internally by the Boost library.
BOOST_CLASS_TRACKING(IFeature, track_always)

// BOOST knows that IFeature is an abstract class.
BOOST_SERIALIZATION_ASSUME_ABSTRACT(IFeature)



//! Feature list, to store the final clustered features.
class CFeatureList
{
private:

	//! Boost serialization.
    friend class boost::serialization::access;
	
	//! Boost serialization.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & members;
		ar & objectClass;
	}


public:

	//! The actual features.
	std::vector< IFeature * > members;

	//! The object class (used when the CFeatureList represents a dictionary
	//! and per object class dictionaries are used.
	int objectClass;


public:

	//! Constructor.
	CFeatureList() : objectClass(-1) {};

	//! Destructor.
	~CFeatureList() 
	{
		while(!members.empty())
		{
			delete(*(members.begin()));
			members.erase(members.begin());
		}
	};

	//! Function to clear the list without performing deallocation
	void clearList()
	{
		members.clear();
	}

}; // class CFeatureList.



//! Set of multiple CFeatureList objects (dictionaries), used for storing 
//! multiple per class dictionaries into a single object.
class CFeatureListSet
{
private:

	//! Boost serialization.
    friend class boost::serialization::access;
	
	//! Boost serialization.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & members;
		printf("   +-- Serialized %d features.\n", members.size());
	}


public:

	std::vector< CFeatureList * > members;


public:

	//! constructor.
	CFeatureListSet() {}


	//! Searches for the visual dictionary belonging to the given object class.
	static bool getDictionary(
		CFeatureListSet	  * f_dictionarySet,
		CFeatureList	 ** f_dictionary,
		int					f_objectClass);


	//! Destructor.
	~CFeatureListSet() 
	{
		while(!members.empty())
		{
			delete *(members.begin());
			members.erase(members.begin());
		}
	};


}; // class CFeatureListSet


//! Feature response list, where each object corresponds to the IFeature on the
//! CFeatureList.
class CFeatureResponseList
{
private:

	//! Boost serialization.
  friend class boost::serialization::access;
	
	//! Boost serialization.
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
		ar & members;
	}


public:

	//! The actual response values.
	std::vector< float > members;


public:

	//! Constructor.
  CFeatureResponseList() {}

	//! Copy constructor.
	CFeatureResponseList(const CFeatureResponseList & a)
  {
	  // Resize to same feature dimension.
	  int size = static_cast<int>(a.members.size());
	  members.resize(size);
  	
	  // Create iterators.
	  std::vector< float >::const_iterator iterA;
	  std::vector< float >::iterator iter;

	  // Copy all feature dimensions.
	  iter = members.begin();
	  for(iterA  = a.members.begin();
		  iterA != a.members.end();
		  iterA++, iter++)
	  {
		  *iter = *iterA;
	  }
  }

	//! Destructor.
  ~CFeatureResponseList() {}

}; // class CFeatureResponseList.


#endif //__FEATURE_HPP__
