/** \file
 * MRF function prototype.
 *
 * Version: $Id: MarkovRandomField.hpp 1717 2010-09-17 10:15:55Z kris $
 */

#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>

namespace Vi
{
  void EXPORT_TYPE ApplyMRF(Vi::Image<> &segmentation_mask,
                            Vi::Image<> &mrf_segmentation_mask,
                            int aif = 2,
                            int garma = 2);
}
