/** \file
 * Connected components prototypes.
 *
 * $Id: ConnectedComponents.hpp 1989 2011-01-18 15:41:31Z kris $
 */

#ifndef CONNECTED_COMPONENTS_HPP
#define CONNECTED_COMPONENTS_HPP

#include <vector>

#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>

#include "Blob.hpp"

namespace Vi
{
  /**
   * The connected components class. This class uses 4-connectivity connected
   *  components analysis to find blobs in a given input image. As an output, a
   *  vector of Blob items can be obtained.
   */
  class EXPORT_TYPE ConnectedComponents
  {
  public:

    /**
     * Constructor.
     */
    ConnectedComponents();

    /**
     * Get the blobs from the connected components class. The connected com-
     *  ponents are found using 4-connectivity connected components analysis.
     *
     * \param outputBlobs Output vector of Blob objects.
     * \param inputImage Input ViNotion image.
     */
    void process(std::vector<Vi::Blob> &outputBlobs, const Vi::Image<> &inputImage);

  private:

    unsigned int bgVal;       ///< Background value.
    unsigned int fgThreshold; ///< Foreground value threshold.

    /// Labels to classes mapping. Pixels belonging to the same CC share classes.   
    std::vector<unsigned int> classMap;

    /// Contiguous labels to class mapping.
    std::vector<unsigned int> classMapCont;

    /// The label array image.
    Vi::Array2D<unsigned int> labelArray;

    /**
     * Merge class. Merge the classes of l1 and l2. Change all occurences of
     *  class of l2 to class of l1.
     *
     * \param cMap Labels to classes mapping.
     * \param l1 First label (label to which the class is merged to).
     * \param l2 Second label.
     */
    void mergeClass(std::vector<unsigned int> &cMap, unsigned int l1, unsigned int l2);

    /**
     * Make class numbers contiguous. E.g. 1 3 5 5 -> 1 2 3 3.
     *
     * \param cMapCont Labels to classes mapping with contiguous class numbers.
     * \param cMap Input labels to classes mapping.
     * \return The amount of classes in the contiguous class map.
     */
    unsigned int compressClasses(std::vector<unsigned int> &cMapCont,
                                 const std::vector<unsigned int> &cMap);
  };
}

#endif  // CONNECTED_COMPONENTS_HPP
