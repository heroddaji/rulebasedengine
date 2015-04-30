/** \file
 * HOG feature space prototype.
 *
 * $Id: FeatureSpaceHOG.hpp 2024 2011-02-01 12:41:23Z kris $
 */

#ifndef FEATURE_SPACE_HOG_HPP
#define FEATURE_SPACE_HOG_HPP

#include <vector>

#include <ViNotion/Compat.hpp>
#include <ViNotion/Misc.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>

#include <Settings/Settings.hpp>

/**
 * Enable this macro to let the features be calculated over the color
 *  information in the image, instead of grayscale information only.
 */
//#define FEATURE_SPACE_HOG_PROCESS_COLOR

/**
 * Enable this macro to get extensive debugging output:
 *
 *  - 'FSHOG_imgGradHor.png': Horizontal gradient image.
 *  - 'FSHOG_imgGradVer.png': Vertical gradient image.
 *  - 'FSHOG_imgGradMag.png': Gradient magnitude image.
 */
//#define FEATURE_SPACE_HOG_DEBUG

namespace Vi
{
  /**
   * This class describes a single-scale feature vector for a HOG descriptor.
   */
  class FeatureVectorHOG
  {
  public:

    /**
     * Constructor.
     *
     * \param[in]         fLength The length of the feature vector.
     * \param[in]     fLengthNorm The length of the normalized feature vector.
     * \param[in]          fScale The scale of the feature vector.
     * \param[in]  fCellGridWidth The width of the HOG cell grid.
     * \param[in] fCellGridHeight The height of the HOG cell grid.
     */
    FeatureVectorHOG(unsigned int fLength,
                     unsigned int fLengthNorm,
                     const Vi::Frac<int> &fScale,
                     unsigned int fCellGridWidth,
                     unsigned int fCellGridHeight);

    /**
     * Destructor.
     */
    ~FeatureVectorHOG();

    /**
     * Clear the feature values.
     */
    void clear();

    Vi::Frac<int> mScale; ///< The scale of the features.
    float *mFeatures;     ///< The feature data.
    float *mFeaturesNorm; ///< The normalized feature data.

    unsigned int mNumFeatures;     ///< The total amount of features.
    unsigned int mNumFeaturesNorm; ///< The total amount of normalized.
    unsigned int mCellGridWidth;   ///< HOG cell grid width [cells].
    unsigned int mCellGridHeight;  ///< HOG cell grid height [cells].
  };

  /**
   * This class describes a multi-scale feature space for HOG descriptors. It
   *  is used as a more-or-less global pool of HOG object features, to be used
   *  by any HOG feature class.
   *
   * This class includes all the functionality to create features for given
   *  input image data.
   *
   * \todo On a reinitialization, the LUTs are recalculated. If, for some
   *        future reason this is bad, you'll find it in initialize().
   * \todo Think about policies regarding the creation of the HOG cell grid
   *        (see calculateClassSettings(). For now; dump the edge pixels.
   * \todo Maybe try to represent the feature grid in some more intellegent
   *        manner (e.g. a template class-like entity instead of plain width
   *        and height).
   * \todo Create a smarter way to describe the feature vectors. For now, it
   *        seems a combination between a bounding box (at cell-grid level) and
   *        a corresponding scale is sufficient.
   */
  class EXPORT_TYPE FeatureSpaceHOG
  {
    friend class ObjectDetectorHOG;

  public:

    /**
     * Constructor. The default call (empty parameters) will create an empty
     *  FeatureSpaceHOG that will need to be configured afterwards by the
     *  configure() call.
     *
     * Because of the fact that the image dimensions are only known at either
     *  construction time or update() time, there's three sensible ways of
     *  initializing the class:
     *
     *  -# Call the constructor with image dimensions and configuration, which
     *      creates a fully configured and initialized class (recommended
     *      approach; all is done at first update() call).
     *  -# First call the default constructor (which will create an empty and
     *      uninitialized class), maybe run configure() and then let the class
     *      initialize at the first call of update().
     *  -# Call the constructor with only image dimensions (which creates an
     *      initialized class), maybe run configure() to reinitialize.
     *
     * \note The HOG cell grid is calculated such that the maximum amount of
     *        cells is fitted into the input image size, the remaining edge
     *        pixels will be discarded.
     *
     * \param[in]  fImgWidth The input image width [px] (0 by default).
     * \param[in] fImgHeight The input image height [px] (0 by default).
     * \param[in]    fConfig Optional configuration file.
     */
    FeatureSpaceHOG(unsigned int fImgWidth = 0,
                    unsigned int fImgHeight = 0,
                    const Vi::Settings &fConfig = Vi::Settings());

    /**
     * Destructor.
     */
    ~FeatureSpaceHOG();

    /**
     * Set run-time configuration.
     *
     * \param[in] fConfig The configuration settings.
     */
    void configure(const Vi::Settings &fConfig);

    /**
     * Update/recalculate the feature space state given the image data.
     *
     * \param[in] fImage The image data for which to calculate the features.
     * \param[in] fScale If given, only this scale will be processed, otherwise
     *                    all scales.
     */
    void update(const Vi::Image<> &fImage,
                const Vi::Frac<int> &fScale = Vi::Frac<int>(0, 1));
    
    /**
     * Write a visualization of the feature vector to an image. Two images will
     *  be created; 'FSHOG_features.png' and 'FSHOG_featuresNorm.png' where the
     *  former visualizes the non-normalized features, and the latter
     *  visualizes the normalized features respectively.
     *
     * The non-normalized features are visualized such that the maximum bin
     *  value corresponds to the maximal displayable value, the other values
     *  are normalized to this value.
     *
     * The normalized features are visualized such that the normalization
     *  blocks per cell are summed and the maximum bin value corresponds to the
     *  maximal displayable value, the other values are normalized to this
     *  value.
     *
     * \note If a lot of orientation bins have to be shown per cell, the
     *        orientation bin lines will overwrite and the output image is
     *        unusable.
     *
     * \todo For now, this function will generate two image files;
     *        'FSHOG_features.png' and 'FSHOG_featuresNorm.png'. Maybe make
     *        this configurable in the future.
     *
     * \todo Although this function works, it might need some optimization.
     *
     * \param[in]       fScale The scale for which to show the features.
     * \param[in] fScaleFactor The scale factor of the visualization w.r.t. the
     *                          original input image.
     * \param[in]   fUseJetMap If true, a jet map (color map) will be used to
     *                          indicate the bin magnitude, otherwise plain
     *                          pixel intensity will be used (default: true).
     */
    void visualizeFeatureVector(const Vi::Frac<int> fScale = Vi::Frac<int>(1),
                                unsigned int fScaleFactor = 4,
                                bool fUseJetMap = true) const;

    /**
     * Map a given bounding box (from pixel space) to a bounding box on the
     *  HOG cell grid edges in either one of the scales of the feature space.
     *
     * To find the mapping, the dice metric between the original bounding box
     *  and the mapped bounding box over all scales is maximized.
     *
     * \note This function will find a mapping such that a cell block will be
     *        selected that completely encompasses the given (pixel grid-level)
     *        bounding box.
     *
     * \param[in]        fBboxIn The input bounding box (on pixel grid).
     * \param[out] fBboxPixelOut The output bounding box (in [px] on HOG
     *                            feature grid).
     * \param[out] fBboxCellOut The output bounding box (in [cells] on HOG
     *                            feature grid).
     * \param[out]    fScaleOut The scale of the HOG features on which the
     *                            output bounding box was mapped.
     */
    void mapToFeatureSpaceGrid(const Vi::Bbox<int> &fBboxIn,
                               Vi::Bbox<int> &fBboxPixelOut,
                               Vi::Bbox<int> &fBboxCellOut,
                               Vi::Frac<int> &fScaleOut) const;
    
    /**
     * Map a given bounding box (cell units) to a bounding box (pixel units).
     *
     * This function uses the relative scale of the feature vector to calculate
     *  the `real' bounding box position in scale 1/1.
     *
     * \param[in]  fFeatVec The feature vector onto which the map must be made.
     * \param[in] fBboxCell The bounding box (cell units) to be mapped.
     *
     * \return The equivalent bounding box at scale 1/1.
     */
    Vi::Bbox<int> mapCellToRealPix(const Vi::FeatureVectorHOG &fFeatVec,
                                   const Vi::Bbox<int> &fBboxCell);
    
    /**
     * Compare two feature vectors (of identical length).
     *
     * \todo Now, the feature vector dot product loop is unrolled four times,
     *        if we assume 64-systems, we may want to unroll eight times.
     *
     * \param[in] fBboxCell1 A cell-grid bounding box describing the left hand
     *                        side feature vector at the given scale.
     * \param[in]    fScale1 Scale of left hand feature vector.
     * \param[in] fBboxCell2 A cell-grid bounding box describing the right hand
     *                        side feature vector at the given scale.
     * \param[in]    fScale2 Scale of right hand feature vector.
     *
     * \return A value between 0.0f and 1.0f, indicating the similarity between
     *          the given feature vectors, where 0.0f means no similarity, and
     *          1.0f means high similarity/identical, respectively.
     */
    float compareFeatures(const Vi::Bbox<int> &fBboxCell1,
                          const Vi::Frac<int> &fScale1,
                          const Vi::Bbox<int> &fBboxCell2,
                          const Vi::Frac<int> &fScale2);

    /// HOG cell block normalization type enumerator.
    enum BlockNormalizationType
    {
      NORM_L1,     ///< f = v / (||v|| + e)
      NORM_L1SQRT, ///< f = sqrt(v / (||v|| + e))
      NORM_L2,     ///< f = v / sqrt(||v||^2 + e^2)
      NORM_L2HYS   ///< NORM_L2 + clipping + renormalization (default)
    };

    /// Similarity measure type enumerator.
    enum SimilarityMeasureType
    {
      SIMILARITY_BHATTACHARYYA  ///< Bhattacharyya coefficient.
    };

  private:

    // ------------------------------------------------------------------------
    // \{ \name Main class parameters

    /// The feature vectors for each scale.
    std::vector<Vi::FeatureVectorHOG *> mScaleFeatures;

    unsigned int mImgWidth;    ///< The input frame width [px].
    unsigned int mImgHeight;   ///< The input frame height [px].

    uint32_t mCellWidth;       ///< HOG cell width [px].
    uint32_t mCellHeight;      ///< HOG cell height [px].
    uint32_t mOrientationBins; ///< The number of orientation bins / cell.
    bool mUseOrientationSign;  ///< Use the orientation sign if true.
    uint32_t mNormBlockWidth;  ///< Width of the normalization block [cells].
    uint32_t mNormBlockHeight; ///< Height of the normalization block [cells].
    uint32_t mNormBlockNum;    ///< The total amount of normalization blocks.

    Vi::Frac<int> mScaleMin;    ///< Minimal scale to be processed.
    Vi::Frac<int> mScaleMax;    ///< Maximum scale to be processed.
    Vi::Frac<int> mScaleFactor; ///< Scale calculation factor.

    BlockNormalizationType mNormalizationType; ///< Method of block normalization.

    /**
     * Epsilon constant for HOG normalization (small constant value, added
     *  during normalization). Normally something like: 0.001.
     */
    float mEpsNormalization; 

    SimilarityMeasureType mSimilarityMeasure; ///< Method of similarity calculation.

    bool mGradientClipEnable; ///< Gradient magnitude clipping enabled flag.
    float mGradientClipMax;   ///< Maximum gradient clipping value.
    float mGradientClipMin;   ///< Minimum gradient clipping value.
    
    /// Pointer to the normalization function.
    void (FeatureSpaceHOG::*mNormalizationFunc)(Vi::FeatureVectorHOG &,
                                                unsigned int,
                                                unsigned int,
                                                unsigned int,
                                                float);
    
    /// Pointer to the histogram sum function.
    float (FeatureSpaceHOG::*mHistSumFunc)(const float **, unsigned int) const;
   
    /// Pointer to the normalization factor function.
    float (FeatureSpaceHOG::*mNormalizationFactorFunc)(float fHistSum) const;

    /// Pointer to the feature comparison function.
    float (FeatureSpaceHOG::*mFeatureComparisonFunc)(const Vi::FeatureVectorHOG &,
                                                     const Vi::FeatureVectorHOG &,
                                                     const Vi::Bbox<int> &,
                                                     const Vi::Bbox<int> &) const;

    bool mIsInitialized; ///< Flag to denote proper class initialization.

    // ------------------------------------------------------------------------
    // \{ \name LUTs and LUT parameters

    /**
     * Spatial cell index relative to current cell for all mCellWidth.
     * Values contain -1 for x values [0, mCellWidth / 2] and +1 otherwise.
     */
    char *mLUTSpatialCellIndexRelative;

    /**
     * Interpolation weights LUT.
     *
     * mLUTSpatialInterpolation[y][x]   = intpolFactHorSelf  * intpolFactVerSelf
     * mLUTSpatialInterpolation[y][x+1] = intpolFactHorSelf  * intpolFactVerOther
     * mLUTSpatialInterpolation[y][x+2] = intpolFactHorOther * intpolFactVerSelf
     * mLUTSpatialInterpolation[y][x+3] = intpolFactHorOther * intpolFactVerOther
     */
    float ***mLUTSpatialInterpolation;

    /// Orientation bin index for each gradient angle, for the other bin.
    unsigned char *mLUTOriBinIndexOther;

    /// Orientation interpolation weights LUT.
    float *mLUTOriInterpolation;

    // ------------------------------------------------------------------------
    // \{ \name Calculation functions

    /**
     * Create HOG features on a single scale.
     *
     * \todo Maybe remove the static_cast'ing stuff in the cell interpolation
     *        direction part.
     */
    void calculateFeaturesSingleScale(const Vi::Image<> &fImage,
                                      const Vi::Frac<int> &fScale);

    /**
     * Normalize all feature cells w.r.t. the neighboring cells.
     *
     * \param[in,out] fFeatureVec The feature vector to normalize.
     */
    void normalizeCells(Vi::FeatureVectorHOG &fFeatureVec);

    /**
     * Get memory index in the feature vector.
     *
     * \param[in] fCellIdxHor Horizontal cell grid index.
     * \param[in] fCellIdxVer Vertical cell grid index.
     * \param[in] fCellGridWidth The width of the HOG cell grid.
     *
     * \return The feature data index.
     */
    inline unsigned int getMemoryIndex(unsigned int fCellIdxHor,
                                       unsigned int fCellIdxVer,
                                       unsigned int fCellGridWidth) const
    {
      return (((fCellIdxVer * fCellGridWidth) + fCellIdxHor) * mOrientationBins);
    }

    /**
     * Get memory index in the normalized feature vector.
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
      return (((((fCellIdxVer * fCellGridWidth) + fCellIdxHor) * mNormBlockNum)
              + fBlock) * mOrientationBins);
    }

    // ------------------------------------------------------------------------
    // \{ \name Histogram sum functions

    /**
     * Get sum over a HOG cell (L1).
     *
     * \param[in] fFeaturePtr A pointer to the features array.
     * \param[in]      fIndex The index of the HOG cell.
     * 
     * \return The sum over the HOG cell.
     */
    inline float getHistSumL1(const float **fFeaturePtr,
                              unsigned int fIndex) const;

    /**
     * Get sum over a HOG cell (L2).
     *
     * \param[in] fFeaturePtr A pointer to the features array.
     * \param[in]      fIndex The index of the HOG cell.
     * 
     * \return The sum over the HOG cell.
     */
    inline float getHistSumL2(const float **fFeaturePtr,
                              unsigned int fIndex) const;

    // ------------------------------------------------------------------------
    // \{ \name Normalization factor functions

    /**
     * Get the normalization factor for a given histogram sum (L1).
     *
     * \param[in] fHistSum The histogram sum.
     *
     * \return The normalization factor.
     */
    inline float getNormalizationFactorL1(float fHistSum) const;

    /**
     * Get the normalization factor for a given histogram sum (L2).
     *
     * \param[in] fHistSum The histogram sum.
     *
     * \return The normalization factor.
     */
    inline float getNormalizationFactorL2(float fHistSum) const;

    // ------------------------------------------------------------------------
    // \{ \name Normalization functions

    /**
     * Normalize a block of HOG cells using NORM_L1 normalization.
     *
     * \param[in,out] fFeatureVec The feature vector object.
     * \param[in]              fX The X-coordinate of the cell in the cell grid.
     * \param[in]              fY The Y-coordinate of the cell in the cell grid.
     * \param[in]     fBlockIndex The index of the cell in the normalization block.
     * \param[in]     fNormFactor The normalization factor.
     */
    inline void normalizeCellBlockL1(Vi::FeatureVectorHOG &fFeatureVec,
                                     unsigned int fX,
                                     unsigned int fY,
                                     unsigned int fBlockIndex,
                                     float fNormFactor);

    /**
     * Normalize a block of HOG cells using NORM_L1SQRT normalization.
     *
     * \param[in,out] fFeatureVec The feature vector object.
     * \param[in]              fX The X-coordinate of the cell in the cell grid.
     * \param[in]              fY The Y-coordinate of the cell in the cell grid.
     * \param[in]     fBlockIndex The index of the cell in the normalization block.
     * \param[in]     fNormFactor The normalization factor.
     */
    inline void normalizeCellBlockL1SQRT(Vi::FeatureVectorHOG &fFeatureVec,
                                         unsigned int fX,
                                         unsigned int fY,
                                         unsigned int fBlockIndex,
                                         float fNormFactor);

    /**
     * Normalize a block of HOG cells using NORM_L2 normalization.
     *
     * \param[in,out] fFeatureVec The feature vector object.
     * \param[in]              fX The X-coordinate of the cell in the cell grid.
     * \param[in]              fY The Y-coordinate of the cell in the cell grid.
     * \param[in]     fBlockIndex The index of the cell in the normalization block.
     * \param[in]     fNormFactor The normalization factor.
     */
    inline void normalizeCellBlockL2(Vi::FeatureVectorHOG &fFeatureVec,
                                     unsigned int fX,
                                     unsigned int fY,
                                     unsigned int fBlockIndex,
                                     float fNormFactor);

    /**
     * Normalize a block of HOG cells using NORM_L2HYS normalization (pass 1).
     *
     * \param[in,out] fFeatureVec The feature vector object.
     * \param[in]              fX The X-coordinate of the cell in the cell grid.
     * \param[in]              fY The Y-coordinate of the cell in the cell grid.
     * \param[in]     fBlockIndex The index of the cell in the normalization block.
     * \param[in]     fNormFactor The normalization factor.
     */
    inline void normalizeCellBlockL2HYSPass1(Vi::FeatureVectorHOG &fFeatureVec,
                                             unsigned int fX,
                                             unsigned int fY,
                                             unsigned int fBlockIndex,
                                             float fNormFactor);

    /**
     * Normalize a block of HOG cells using NORM_L2HYS normalization (pass 2).
     *
     * \param[in,out] fFeatureVec The feature vector object.
     * \param[in]              fX The X-coordinate of the cell in the cell grid.
     * \param[in]              fY The Y-coordinate of the cell in the cell grid.
     * \param[in]     fBlockIndex The index of the cell in the normalization block.
     * \param[in]     fNormFactor The normalization factor.
     */
    inline void normalizeCellBlockL2HYSPass2(Vi::FeatureVectorHOG &fFeatureVec,
                                             unsigned int fX,
                                             unsigned int fY,
                                             unsigned int fBlockIndex,
                                             float fNormFactor);

    // ------------------------------------------------------------------------
    // \{ \name Similarity measure calculation functions

    /**
     * Calculate Bhattacharyya coefficient.
     *
     * \param[in]   fFeatVec1 Pointer to the feature vector for bbox 1.
     * \param[in]   fFeatVec2 Pointer to the feature vector for bbox 2.
     * \param[in]  fBboxCell1 Bounding box 1.
     * \param[in]  fBboxCell2 Bounding box 2.
     *
     * \return A similarity value between 0.0f and 1.0f, where 0.0f means no/
     *          low similarity, and 1.0f means identical/high similarity.
     */
    float similarityBhattacharyya(const Vi::FeatureVectorHOG &fFeatVec1,
                                  const Vi::FeatureVectorHOG &fFeatVec2,
                                  const Vi::Bbox<int> &fBboxCell1,
                                  const Vi::Bbox<int> &fBboxCell2) const;

    // ------------------------------------------------------------------------
    // \{ \name Miscellanea

    /**
     * (Re-)Initialize the class. This function recalculates class settings and
     *  calls the proper (de-)allocation functions.
     */
    void initialize();

    /**
     * (Re-)Calculate class settings such as cell grid dimensions.
     */
    void calculateClassSettings();

    /**
     * Allocate the feature vector data (at class construction).
     */
    void allocateFeatureData();

    /**
     * Deallocate the feature vector data (at class destruction).
     */
    void deallocateFeatureData();

    /**
     * Create LUT for spatial interpolation weights.
     */
    void createLUTSpatialInterpolation();

    /**
     * Cleanup LUT for spatial interpolation weights.
     */
    void cleanupLUTSpatialInterpolation();

    /**
     * Create LUT for orientation interpolation weights.
     */
    void createLUTOriInterpolation();

    /**
     * Cleanup LUT for orientation interpolation weights.
     */
    void cleanupLUTOriInterpolation();
  };
}

#endif // FEATURE_SPACE_HOG_HPP
