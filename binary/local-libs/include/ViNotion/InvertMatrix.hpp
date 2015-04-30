/** \file
 * Template function for matrix inversion.
 *
 * $Id: InvertMatrix.hpp 1675 2010-09-10 14:25:07Z kris $
 */

#ifndef INVERT_MATRIX_HPP
#define INVERT_MATRIX_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <stdexcept>

namespace ub = boost::numeric::ublas;

namespace Vi
{
  /**
   * Invert matrix. Uses lu_factorize and lu_substitute in uBLAS to invert a matrix. 
   * From boost wiki: http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?LU_Matrix_Inversion .
   */
  template<class T>
  ub::matrix<T> invertMatrix(ub::matrix<T> input)
  {
    ub::matrix<T> inverse;

    // Create a permutation matrix for the LU-factorization.
    ub::permutation_matrix<std::size_t> pm(input.size1());

    // Perform LU-factorization.
    if (ub::lu_factorize(input, pm) != 0)
      throw std::logic_error("[invertMatrix]: Matrix is not invertible.");

    // Create identity matrix of "inverse".
    inverse = ub::identity_matrix<T>(input.size1());

    // Backsubstitute to get the inverse.
    ub::lu_substitute(input, pm, inverse);

    return inverse;
  }
}

#endif // INVERT_MATRIX_HPP
