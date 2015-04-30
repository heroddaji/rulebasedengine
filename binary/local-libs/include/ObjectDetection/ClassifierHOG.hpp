/** \file
 * HOG classifier prototype.
 *
 * $Id$
 */

#ifndef CLASSIFIER_HOG_HPP
#define CLASSIFIER_HOG_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include <Features/FeatureSpaceHOG.hpp>

namespace Vi
{
  /**
   * HOG classifier prototype.
   *
   * \note For feature storing, an std::vector was chosen, because it was a
   *        hassle to implement the serialization functions load/store with
   *        dynamically allocated arrays.
   */
  class EXPORT_TYPE ClassifierHOG
  {
  public:

    /**
     * Constructor.
     *
     * \param[in] fFileName The file that holds the classifier data (optional).
     */
    ClassifierHOG(const std::string &fFileName = std::string());

    std::vector<float> mFeatures;  ///< The feature data.
    unsigned int mNumFeatures;     ///< The total amount of features.

    unsigned int mCellGridWidth;   ///< HOG cell grid width [cells].
    unsigned int mCellGridHeight;  ///< HOG cell grid height [cells].
    unsigned int mCellWidth;       ///< HOG cell width [px].
    unsigned int mCellHeight;      ///< HOG cell height [px].

    unsigned int mOrientationBins; ///< The number of orientation bins / cell.
    bool mUseOrientationSign;      ///< Use the orientation sign if true.

    unsigned int mNormBlockWidth;  ///< Width of the normalization block [cells].
    unsigned int mNormBlockHeight; ///< Height of the normalization block [cells].

    /// Method of block normalization.
    Vi::FeatureSpaceHOG::BlockNormalizationType mNormalizationType;

    float mWeightsBias; ///< Classifier weights bias.

    /**
     * Load classifier from file.
     *
     * \param[in] fFileName The file that holds the classifier data.
     */
    void loadFromFile(const std::string &fFileName);

    /**
     * Store classifier to file.
     *
     * \param[in] fFileName The file that will hold the classifier data.
     */
    void storeToFile(const std::string &fFileName);

    /**
     * Print information of the classifier (for debugging purposes).
     */
    void printInfo();

    /**
     * Write a visualization of the positive and negative features to an image.
     *
     * The images; 'ClassifierHOG_{Pos,Neg}.png' will be created.
     *
     * The features are visualized such that the normalization blocks per cell
     *  are summed and the maximum bin value corresponds to the maximal
     *  displayable value, the other values are normalized to this value.
     *
     * \note If a lot of orientation bins have to be shown per cell, the
     *        orientation bin lines will overwrite and the output image is
     *        unusable.
     *
     * \todo Make image names configurable.
     * \todo Although this function works, it might need some optimization.
     *
     * \param[in] fScaleFactor The scale factor of the visualization w.r.t. the
     *                          original pixel grid (cell width/height).
     * \param[in]   fUseJetMap If true, a jet map (color map) will be used to
     *                          indicate the bin magnitude, otherwise plain
     *                          pixel intensity will be used (default: true).
     */
    void visualizeFeatures(unsigned int fScaleFactor = 6,
                           bool fUseJetMap = true) const;

    /**
     * Same as visualizeFeatures(), but only shows blocks intensities instead
     *  of the actual histograms.
     *
     * The image files are saved as 'ClassifierHOG_Block{Pos,Neg}.png'.
     */
    void visualizeFeaturesBlock(unsigned int fScaleFactor = 4,
                                bool fUseJetMap = true) const;

  private:

    /**
     * Get memory index in the normalized feature vector (taken from
     *  Vi::FeatureSpaceHOG).
     *
     * \param[in]    fCellIdxHor Horizontal cell grid index.
     * \param[in]    fCellIdxVer Vertical cell grid index.
     * \param[in] fCellGridWidth The width of the HOG cell grid.
     * \param[in]         fBlock Normalization block cell index.
     *
     * \return The normalized feature data index.
     */
    inline unsigned int getMemoryIndexNorm(unsigned int fCellIdxHor,
                                           unsigned int fCellIdxVer,
                                           unsigned int fCellGridWidth,
                                           unsigned int fBlock) const
    {
      return (((((fCellIdxVer * fCellGridWidth) + fCellIdxHor)
                  * mNormBlockWidth * mNormBlockHeight)
                  + fBlock) * mOrientationBins);
    }

    /// Boost serialization.
    friend class boost::serialization::access;
	
    /// Serialization function for saving classifier objects.
    template<class Archive>
      void save(Archive &fAr, const unsigned int fVersion) const
    {
      fAr & mFeatures;
      fAr & mNumFeatures;
      fAr & mCellGridWidth;
      fAr & mCellGridHeight;
      fAr & mCellWidth;
      fAr & mCellHeight;
      fAr & mOrientationBins;
      fAr & mUseOrientationSign;
      fAr & mNormBlockWidth;
      fAr & mNormBlockHeight;
      fAr & mNormalizationType;
      fAr & mWeightsBias;
    }

    /// Serialization function for loading classifier objects.
    template<class Archive>
      void load(Archive &fAr, const unsigned int fVersion)
    {
      fAr & mFeatures;
      fAr & mNumFeatures;
      fAr & mCellGridWidth;
      fAr & mCellGridHeight;
      fAr & mCellWidth;
      fAr & mCellHeight;
      fAr & mOrientationBins;
      fAr & mUseOrientationSign;
      fAr & mNormBlockWidth;
      fAr & mNormBlockHeight;
      fAr & mNormalizationType;
      fAr & mWeightsBias;
    }

    // Tells Boost serialization to split in load/save functions
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };
}

#endif // CLASSIFIER_HOG_HPP
