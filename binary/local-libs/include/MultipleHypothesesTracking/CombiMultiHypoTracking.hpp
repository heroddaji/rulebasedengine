/** \file
 * Combinatorial Multiple Hypothesis Tacking class definition.
 *
 * $Id: CombiMultiHypoTracking.hpp 1975 2011-01-17 10:27:31Z kris $
 */

#ifndef COMBI_MULTI_HYPO_TRACKING_HPP
#define COMBI_MULTI_HYPO_TRACKING_HPP

#include <vector>
#include <string>
#include <iostream>

#include <ViNotion/Compat.hpp>
#include <ViNotion/Bbox.hpp>
#include <GenericObject/GenericObject.hpp>
#include <Tracking/TrackedObject.hpp>

#include "Hypothesis.hpp"
#include "HypothesisPath.hpp"

#include "CombiMultiHypoTrackingParams.hpp"

namespace Vi
{
  /**
   * Combinatorial Multiple Hypothesis Tracking class.
   */
  class EXPORT_TYPE CombiMultiHypoTracking
  {
    public:

      /**
       * Constructor.
       */
      CombiMultiHypoTracking();
      
      /**
       * Main processing function: create the matrices and perform the combinatorial
       *  multiple hypothesis algorithm to find the best hypothesis for each objects.
       *
       * \param[in] fDetectedObjects The list of detected objects.
       * \param[in]  fTrackedObjects The list of pointer to the tracked objects.
       * \param[in]           fFrame The current frame.
       * \param[in,out]  fNewObjects The list of bounding boxes that should become new tracked objects.
       */
      void process(std::vector<Vi::GenericObject> &fDetectedObjects, 
                   std::vector<Vi::TrackedObject*> &fTrackedObjects,
                   Vi::Image<> &fFrame,
									 std::vector<Vi::Bbox<int> > &fNewObjects);

      HypothesisPath mBestPath; ///< The best set of hypothesis.
      
    private:

      /**
       * Processing in the case where only detection are present.
       * 
       * \param[in] fDetectedObjects The list of detected objects.
       */
      void processDetectionsOnly(std::vector<GenericObject> &fDetectedObjects);
      
      /**
       * Processing in the case where only detection are present.
       * 
       * \param[in] fTrackedObjects The list of pointer to the tracked objects.
       */
      void processTracksOnly(std::vector<Vi::TrackedObject*> &fTrackedObjects);

      /**
       * Compute the similarity matrix between detected and tracked objects.
       *
       * \param[in] fDetections The list of detected objects.
       * \param[in]     fTracks The list of pointer to the tracked objects.
       */
      void createResemblanceMatrix(std::vector<Vi::GenericObject> &fDetections,
                                   std::vector<Vi::TrackedObject*> &fTracks);

      /**
       * Clip the value of a the entries in the different matrix between 0 and 1.
       *
       * \param[in] fValue The value to be clipped.
       */
      float clipValue(float fValue);
      
      /**
       * Create appearance matrix.
       *  
       * \note The appearance likelihood for a detected object is given by:
       *    (1 - the resemblance with tracked objects).
       */
      void createAppearanceMatrix();

      /**
       * Create disappearance matrix.
       *  
       * \note The disapperance likelihood for a tracked object is given by:
       *    (1 - the resemblance with detected objects).
       */
      void createDisappearanceMatrix();

      /**
       * Create track matrix.
       *  
       * \note The track likelihood is given by the similarity, where similarity 
       *    with other pairs are retrieved.
       */
      void createTrackMatrix();

      /**
       * Create split matrix.
       *  
       * \note The split likelihood between a tracked object and two detected objects.  
       */
      void createSplitMatrix();

      /**
       * Create merge matrix.
       *  
       * \note The merge likelihood between a detected object and two tracked objects.  
       */
      void createMergeMatrix();

      /**
       * Perform Combinatorial Multi-Hypothesis Tracking.
       *
       * \param[in] fNbDetectedObjects The number of detected objects.
       * \param[in]  fNbTrackedObjects The number of tracked objects.
       */ 
      void cmht(unsigned int fNbDetectedObjects, unsigned int fNbTrackedObjects);

      /**
       * Get the set of best hypothesis for every object.
       *
       * \param[in]            fPath The path for which we generate the set of hypotheses.
       * \param[out] fBestHypotheses The generated set of hypotheses.
       */
      void generateBestHypotheses(HypothesisPath &fPath, 
                                  std::vector<Hypothesis> &fBestHypotheses);
      
      /**
       * Get the k best hypotheses out of a set of hypotheses
       *
       * \param[in]                fK The number of hypotheses to get.
       * \param[in]   fBestHypotheses The set of hypotheses to consider.
       * \param[out] fKBestHypotheses The k best hypotheses.
       */ 
      void getKBestHypotheses(unsigned int fK, 
                              std::vector<Hypothesis> &fBestHypotheses, 
                              std::vector<Hypothesis> &fKBestHypotheses);

      /**
       * Get the k best path out of a set of candidate paths
       * 
       * \param[in]                fK The number of paths to get.
       * \param[in]   fCandidatePaths The set of paths to consider.
       * \param[out]      fKBestPaths The k best paths.
       */
      void getKBestPaths(unsigned int fK, 
                         std::vector<HypothesisPath> &fCandidatePaths,
                         std::vector<HypothesisPath> &fKBestPaths);

      /**
       * Get the best path out of a set of the best k paths. This function sets
       *  the internal public mBestPath variable.
       * 
       * \param[in] fKBestPath The k best paths.
       */
      void setBestPath(std::vector<HypothesisPath> &fKBestPath); 
      
      /**
       * Remove the paths that are the same.
       *
       * \param[in,out] fCandidatePaths The set of paths to be processed.
       */
      void removeSamePaths(std::vector<HypothesisPath> &fCandidatePaths);

      /**
       * Check that the hypothesis from the best path are consistent.
       *
       * \note  An hypothesis is considered as consistent if the other objects involved are also into another
       *        hypothesis that make sense (exemple: if track x is matched to detection y, then detection y
       *        has to be match to track x). 
       */
      void checkHypothesesConsistency(std::vector<Vi::TrackedObject*> &fTracks);

			/**
			 * Trying to correct the hypothesis that were not consistent. Did not work well ..
			 *
			 *	\note Currently not used.
			 */
			void enforceHypothesisConsistency(std::vector<Vi::TrackedObject> &fTracks);

      /**
       * This function apply basic and generic tracking rules.
       *
       * \note  Based on the hypothesis, it will act on the current tracked objects and the current detections.
       *        APPEAR: the new objects list will be updated with the corresponding detection.
       *        TRACK: the tracked objects is re-initialized with the current detection.
       *        MERGE: the update of the tracked objects is left to the mean-shift tracker.
       *        SPLIT: a new object will added to the list, the other one will be match to the "closest" one.
       *        DISAPPEAR: a track object will flagged to be removed if it has not been matched for some time. 
       *
       *  \param[in]      fDetectedObjects The list of detected objects.       
       *  \param[in]       fTrackedObjects The list of pointers to the tracked objects.       
       *  \param[in]                fFrame The current frame.
       *  \param[in,out]       fNewObjects The list of bounding boxes that should become new tracked objects.
       */       
      void applyTrackingRules(std::vector<Vi::GenericObject> &fDetectedObjects, 
                              std::vector<Vi::TrackedObject*> &fTrackedObjects,
                              const Vi::Image<> &fFrame,
															std::vector<Vi::Bbox<int> > &fNewObjects);

      /**
       * Tells if a tracked object should be removed.
       *
       * \note  The object will be flagged to be removed if it has not been matched to any 
			 *				detections for a pre-defined number of frames.
       *
       * \param[in] fTrackedObject The considered object.
       */
      void trackShouldBeRemoved(Vi::TrackedObject* &fTrackedObject);
     
      /**
       * Print an hypotheses path. Debugging function.
       *
       * \param[in] fPath The path to be printed.
       */      
      void printPath(HypothesisPath &fPath);
 
      /**
       * Print a matrix. Debugging function.
       *
       * \param[in] fMatrix The matrix to be printed.
       */      
      void printMatrix(std::vector<std::vector <float> > &fMatrix);

      /**
       * Print the set of best hypothesis. Debugging function.
       */
      void printInfo(const Vi::HypothesisPath &fPath, const std::vector<Vi::TrackedObject> &fTracks);
      
      /**
       * Print info on the objects. Debugging function.
       */
      void printObjectsInfos(std::vector<Vi::TrackedObject> &fTracks);

      /**
       * Return the hypothesis type in string. Debugging function.
       */
			std::string printHypothesisState(Vi::Hypothesis &fHypothesis);
      
			/**
       * Print info on the objects. Debugging function.
       */
			void printHypothesis(Vi::Hypothesis &fHypothesis, std::vector<Vi::TrackedObject> &fTracks);

		      
      unsigned int mNbDetectedObjects; ///< The number of detected objects
      unsigned int mNbTrackedObjects;  ///< The number of tracked objects
			
			bool mVerbose;									 ///< If you want to print some information.

      std::vector< std::vector<float> > mResemblanceMatrix;   ///< The resemblance matrix.
      std::vector< std::vector<float> > mAppearanceMatrix;    ///< The appearance matrix.
      std::vector< std::vector<float> > mDisappearanceMatrix; ///< The disappearance matrix.
      std::vector< std::vector<float> > mTrackMatrix;         ///< The track matrix.
      std::vector< std::vector<float> > mSplitMatrix;         ///< The split matrix.
      std::vector< std::vector<float> > mMergeMatrix;         ///< The merge matrix.

      std::vector< std::vector<int> > mSplitAdditionalMatrix; ///< The split additional matrix.
      std::vector< std::vector<int> > mMergeAdditionalMatrix; ///< The merge additional matrix.
  };
}

#endif // COMBI_MULTI_HYPO_TRACKING_HPP
