/** \file
 * Morphology function prototypes.
 * 
 * $Id: Morphology.hpp 1975 2011-01-17 10:27:31Z kris $
 */

#ifndef MORPHOLOGY_HPP 
#define MORPHOLOGY_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <Settings/Settings.hpp>

/**
 * This data type is used to define a single row in the morphology operator.
 */
class rowElement
{
public:
  rowElement() : row(0), offset(0), bytes(0) {}

  int row;    ///< Row index of the row element (can be negative).
  int offset; ///< First white pixel index in this row.
  int bytes;  ///< The amount of white pixels in this row (following the offset).
};

namespace Vi
{
  class EXPORT_TYPE Morphology
  {
  public:

    /**
     * Constructor.
     *
     * \param[in] fRadiusX The X-radius of the operator.
     * \param[in] fRadiusY The Y-radius of the operator.
     */
    Morphology(unsigned int fRadiusX, unsigned int fRadiusY);

    /**
     * Destructor.
     */
    ~Morphology();

    /**
     * Dilate the given mask using an elipsoid defined by an X- and Y-radius.
     *
     * \param[in,out] fMask The input mask/the morphed output mask.
     */
    void dilateMask(Vi::Image<> &fMask);

    /**
     * Erode the given mask using an elipsoid defined by an X- and Y-radius.
     *
     * \param[in,out] fMask The input mask/the morphed output mask.
     */
    void erodeMask(Vi::Image<> &fMask);

    /**
     * Open the given mask using an elipsoid defined by an X- and Y-radius.
     *
     * \param[in,out] fMask The input mask/the morphed output mask.
     */
    void openMask(Vi::Image<> &fMask);

    /**
     * Close the given mask using an elipsoid defined by an X- and Y-radius.
     *
     * \param[in,out] fMask The input mask/the morphed output mask.
     */
    void closeMask(Vi::Image<> &fMask);

	
		/**
     * Run-time configuration function.
     */
		void configure(const Vi::Settings &fConfig);

  private:

		/**
		 * Build the morphology operator.
		 */
		void buildOperator();

    /**
     * The actual morphology processing.
     *
     * \param[in,out]  fMask The in-/output mask.
     * \param[in] fMaskValue The mask value used for filling in the operator.
     */
    void doMorphology(Vi::Image<> &fMask, unsigned char fMaskValue);

    uint32_t mRadiusX;        ///< The operator X-radius.
    uint32_t mRadiusY;        ///< The operator Y-radius.
    unsigned int mOperatorSizeX;  ///< The operator size in X-direction.
    unsigned int mOperatorSizeY;  ///< The operator size in Y-direction.
    rowElement *mMorphOperator;   ///< The morphology operator.
  };
}

#endif // MORPHOLOGY_HPP
