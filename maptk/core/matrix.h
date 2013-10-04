/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef MAPTK_MATRIX_H_
#define MAPTK_MATRIX_H_

#include <iostream>
#include <cassert>
#include <boost/integer/static_min_max.hpp>

#include "vector.h"
#include "vector_cmath.h"

namespace maptk
{

/// A representation of a matrix
template <unsigned M, unsigned N, typename T>
class matrix_
{
public:
  /// a compile time constant defined to be min(M,N)
  static unsigned const min_dim = boost::static_unsigned_min<M,N>::value;
  /// a compile time constant defined to be max(M,N)
  static unsigned const max_dim = boost::static_unsigned_max<M,N>::value;
  /// a compile time constant defined to be M*N
  static unsigned const num_elems = M*N;
  /// typedef for vector cmath (i.e. treat the matrix as a vector)
  typedef vector_cmath_<num_elems,T> cmath;


  /// Constructor - does not initialize
  matrix_<M,N,T>() {}

  /// Copy Constructor
  matrix_<M,N,T>(const matrix_<M,N,T>& other)
  {
    memcpy( data_[0], other.data_[0], M*N*sizeof(T) );
  }

  /// Constructor - from block of data (row-wise)
  explicit matrix_<M,N,T>(const T* data)
  {
    memcpy( data_[0], data, M*N*sizeof(T) );
  }

  /// Constructor - fill with a constant value
  explicit matrix_<M,N,T>(const T& value) { this->fill(value); }

  /// Assignment operator
  matrix_<M,N,T>& operator=(const matrix_<M,N,T>& other)
  {
    memcpy( data_[0], other.data_[0], M*N*sizeof(T) );
    return *this;
  }

  /// Return the i-th row
  T* operator[](unsigned int i) { return data_[i]; }

  /// Return the i-th row (const)
  T const* operator[](unsigned int i) const { return data_[i]; }

  /// Return the i-th row, j-th column
  T& operator()(unsigned int i, unsigned int j)
  {
    assert(i<M);
    assert(j<N);
    return data_[i][j];
  }

  /// Return the i-th row (const)
  const T& operator()(unsigned int i, unsigned int j) const
  {
    assert(i<M);
    assert(j<N);
    return data_[i][j];
  }

  /// Return a pointer to the contiguous block of memory
  T* data() { return data_[0]; }

  /// Return a pointer to the contiguous block of memory
  T const* data() const { return data_[0]; }

  /// Fill the matrix with this value
  matrix_<M,N,T>& fill(const T& value);

  /// Fill the diagonal with this value
  matrix_<M,N,T>& fill_diagonal(const T& value);

  /// Set the diagonal to this vector
  matrix_<M,N,T>& set_diagonal(const vector_<min_dim,T>& diag);

  /// Set the matrix to the identity matrix
  /// Extra rows or columns of a non-square matrix are set to zero
  matrix_<M,N,T>& set_identity();

  /// Add a scalar in place
  matrix_<M,N,T>& operator+=( T s ) { cmath::add( data_[0], s, data_[0] ); return *this; }

  /// Subract a scalr in place
  matrix_<M,N,T>& operator-=( T s ) { cmath::sub( data_[0], s, data_[0] ); return *this; }

  /// Multiply a scalar in place
  matrix_<M,N,T>& operator*=( T s ) { cmath::mul( data_[0], s, data_[0] ); return *this; }

  /// Divide by a scalar in place
  matrix_<M,N,T>& operator/=( T s ) { cmath::div( data_[0], s, data_[0] ); return *this; }

  /// Add a matrix in place
  matrix_<M,N,T>& operator+=( const matrix_<M,N,T>& m )
  {
    cmath::add( data_[0], m.data_[0], data_[0] );
    return *this;
  }

  /// Subract a matrix in place
  matrix_<M,N,T>& operator-=( const matrix_<M,N,T>& m )
  {
    cmath::sub( data_[0], m.data_[0], data_[0] );
    return *this;
  }

  /// Negate operator
  matrix_<M,N,T> operator-() const
  {
    matrix_<M,N,T> result;
    cmath::sub( T(0), data_[0], result.data_[0] );
    return result;
  }


protected:
  T data_[M][N];
};


typedef matrix_<2,2,double> matrix_2x2d;
typedef matrix_<2,2,float> matrix_2x2f;
typedef matrix_<3,3,double> matrix_3x3d;
typedef matrix_<3,3,float> matrix_3x3f;
typedef matrix_<3,4,double> matrix_3x4d;
typedef matrix_<3,4,float> matrix_3x4f;

// --- Matrix-scalar operators ----------------------------------------

/// Matrix-scalar addtion operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator+( const matrix_<M,N,T>& m, const T& s )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::add( m.data(), s, r.data() );
  return r;
}

/// Scalar-matrix addition operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator+( const T& s, const matrix_<M,N,T>& m )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::add( m.data(), s, r.data() );
  return r;
}

/// Matrix-scalar subraction operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator-( const matrix_<M,N,T>& m, const T& s )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::sub( m.data(), s, r.data() );
  return r;
}

/// Scalar-matrix subraction operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator-( const T& s, const matrix_<M,N,T>& m )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::sub( s, m.data(), r.data() );
  return r;
}

/// Scalar post-multiplcation operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator*( const matrix_<M,N,T>& m, const T& s )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::mul( m.data(), s, r.data() );
  return r;
}

/// Scalar pre-multiplication operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator*( const T& s, const matrix_<M,N,T>& m )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::mul( m.data(), s, r.data() );
  return r;
}

/// Scalar division operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator/( const matrix_<M,N,T>& m, const T& s )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::div( m.data(), s, r.data() );
  return r;
}


// --- Matrix-matrix operators ----------------------------------------

/// Addition operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator+( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::add( a.data(), b.data(), r.data() );
  return r;
}

/// Subraction operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> operator-( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::sub( a.data(), b.data(), r.data() );
  return r;
}

/// Element-wise product
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> element_product( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::mul( a.data(), b.data(), r.data() );
  return r;
}

/// Element-wise quotient
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline matrix_<M,N,T> element_quotient( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  matrix_<M,N,T> r;
  vector_cmath_<M*N,T>::div( a.data(), b.data(), r.data() );
  return r;
}

/// Equality operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline bool operator==( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  return vector_cmath_<M*N,T>::eq(a.data(), b.data());
}

/// Inequality operator
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline bool operator!=( const matrix_<M,N,T>& a, const matrix_<M,N,T>& b )
{
  return ! vector_cmath_<M*N,T>::eq(a.data(), b.data());
}

// --- Matrix and vector multiplication -----------------------------------

/// Multiply matrix_ (M x N) and vector_ (N)
/// \relatesalso vector_
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline
vector_<M,T> operator*(const matrix_<M,N,T>& a, const vector_<N,T>& b)
{
  vector_<M,T> out;
  for (unsigned i = 0; i < M; ++i)
  {
    T accum = a(i,0) * b(0);
    for (unsigned k = 1; k < N; ++k)
      accum += a(i,k) * b(k);
    out(i) = accum;
  }
  return out;
}

/// Multiply vector_ (M) and matrix_ (M x N)
/// \relatesalso vector_
/// \relatesalso matrix_
template <unsigned M, unsigned N, typename T>
inline
vector_<N,T> operator*(const vector_<M,T>& a, const matrix_<M,N,T>& b)
{
  vector_<N,T> out;
  for (unsigned i = 0; i < N; ++i)
  {
    T accum = a(0) * b(0,i);
    for (unsigned k = 1; k < M; ++k)
      accum += a(k) * b(k,i);
    out(i) = accum;
  }
  return out;
}

/// Multiply two matrix_ (M x N) times (N x O)
/// \relatesalso matrix_
template <unsigned M, unsigned N, unsigned O, typename T>
inline
matrix_<M,O,T> operator*(const matrix_<M,N,T>& a, const matrix_<N,O,T>& b)
{
  matrix_<M,O,T> out;
  for (unsigned i = 0; i < M; ++i)
    for (unsigned j = 0; j < O; ++j)
    {
      T accum = a(i,0) * b(0,j);
      for (unsigned k = 1; k < N; ++k)
        accum += a(i,k) * b(k,j);
      out(i,j) = accum;
    }
  return out;
}


/// output stream operator for a matrix
template <unsigned M, unsigned N, typename T>
std::ostream&  operator<<(std::ostream& s, const matrix_<M,N,T>& m);

/// input stream operator for a matrix
template <unsigned M, unsigned N, typename T>
std::istream&  operator>>(std::istream& s, matrix_<M,N,T>& m);


} // end namespace maptk


#endif // MAPTK_MATRIX_H_