#ifndef _MATH_MATRIX3_HPP
#define _MATH_MATRIX3_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

LIB_NAMESPACE_START

namespace math {;

template < class V, class H > struct Vector3;
template < class V, class H > struct Matrix4;

template < class ValueType, class HashType = hash::DefaultHashType >
struct Matrix3
{
	typedef Matrix3<ValueType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;
	typedef unsigned int index_type;

	value_type m11, m12, m13;
	value_type m21, m22, m23;
	value_type m31, m32, m33;
 
	static const this_type Identity;
	static const index_type Size;

	Matrix3()
		: m11(1), m12(0), m13(0),
		m21(0), m22(1), m23(0),
		m31(0), m32(0), m33(1)
	{

	}

	template <class OtherType>
	Matrix3(OtherType e11, OtherType e12, OtherType e13,
		OtherType e21, OtherType e22, OtherType e23,
		OtherType e31, OtherType e32, OtherType e33)
		: m11(e11), m12(e12), m13(e13),
		m21(e21), m22(e22), m23(e23),
		m31(e31), m32(e32), m33(e33)
	{
	}

	template <class OtherType>
	Matrix3(const OtherType *elements)
		: m11(elements[0]), m12(elements[1]), m13(elements[2]), 
		m21(elements[3]), m22(elements[4]), m23(elements[5]), 
		m31(elements[6]), m32(elements[7]), m33(elements[8])
	{
	}

	template < class V, class H >
	Matrix3(const Vector3<V,H>& column1, const Vector3<V,H>& column2, const Vector3<V,H>& column3)
		: m11(column1.x), m12(column2.x), m13(column3.x),
		m21(column1.y), m22(column2.y), m23(column3.y),
		m31(column1.z), m32(column2.z), m33(column3.z)
	{
	}

	Matrix3(const this_type& m)
		: m11(m.m11), m12(m.m12), m13(m.m13),
		m21(m.m21), m22(m.m22), m23(m.m23),
		m31(m.m31), m32(m.m32), m33(m.m33)
	{
	}

	template < class V, class H >
	explicit Matrix3(const Matrix3<V,H>& m)
		: m11(m.m11), m12(m.m12), m13(m.m13),
		m21(m.m21), m22(m.m22), m23(m.m23),
		m31(m.m31), m32(m.m32), m33(m.m33)
	{
	}

	template < class V, class H >
	explicit Matrix3(const Matrix4<V,H>& m)
		: m11(m.m11), m12(m.m12), m13(m.m13),
		m21(m.m21), m22(m.m22), m23(m.m23),
		m31(m.m31), m32(m.m32), m33(m.m33)
	{
	}

	void makeIdentity()
	{
		*this = Identity;
	}

// 	this_type coFactor() const
// 	{
// 		this_type coMatrix;
// 		#WARNING#<value_type> adjoindex_typeMat;
// 
// 		for (index_type j = 0; j < Size; j++)
// 		{
// 			for (index_type i = 0; i < Size; i++)
// 			{
// 				// Form the adjoint a_ij
// 				index_type i1 = 0;
// 				for (index_type ii = 0; ii < Size; ii++)
// 				{
// 					if (ii == i)
// 						continue;
// 					index_type j1 = 0;
// 					for (index_type jj = 0; jj < Size; jj++)
// 					{
// 						if (jj == j)
// 							continue;
// 						adjoindex_typeMat(i1, j1) = (*this)(ii, jj);
// 						j1++;
// 					}
// 					i1++;
// 				}
// 
// 				// Calculate the determinate and fill in the elements of the cofactor
// 				coMatrix(i, j) = (value_type)std::pow(-1.0, i + j + 2.0) * adjoindex_typeMat.determinant();
// 			}
// 		}
// 
// 		return coMatrix;
// 	}

	value_type determinant() const
	{
		// rule of Sarrus
		return 
			m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 -
			m13 * m22 * m31 - m11 * m23 * m32 - m12 * m21 * m33;
	}

	value_type determinant_of_minor(int theRowHeightY, int theColumnWidthX ) const
	{
		int x1 = theColumnWidthX == 0 ? 1 : 0;  /* always either 0 or 1 */
		int x2 = theColumnWidthX == 2 ? 1 : 2;  /* always either 1 or 2 */
		int y1 = theRowHeightY   == 0 ? 1 : 0;  /* always either 0 or 1 */
		int y2 = theRowHeightY   == 2 ? 1 : 2;  /* always either 1 or 2 */

		return ( (*this) (y1,x1)  *  (*this) (y2,x2) )
			-  ( (*this) (y1,x2)  *  (*this) (y2,x1) );
	}

	this_type inverse() const
	{
		this_type theOutput;
		value_type det = determinant();

		/* Arbitrary for now.  This should be something nicer... */
		if(::fabs(det) < 1e-2)
			return theOutput;

		value_type oneOverDeterminant = 1.0 / det;

		for(int y = 0; y < 3; y++)
		{
			for(int x = 0; x < 3; x++)
			{
				/* Rule is inverse = 1/det * minor of the TRANSPOSE matrix.  *
				 * Note (y, x) becomes (x, y) INTENTIONALLY here!            */
				theOutput(y, x) = determinant_of_minor(x, y) * oneOverDeterminant;

				/* (y0,x1)  (y1,x0)  (y1,x2)  and (y2,x1)  all need to be negated. */
				if(1 == ((x + y) % 2))
					theOutput(y, x) = -theOutput(y, x);
			}
		}

		return theOutput;
	}

// 	this_type adjoint() const 
// 	{
// 		this_type result;
// 		for (index_type row = 0; row < Size; row++)
// 		{
// 			for (index_type col = 0; col < Size; col++) 
// 			{
// 				if (((col+row) % 2) == 0)
// 					result(row, col) = minor(col, row).determinant();
// 				else
// 					result(row, col) = -minor(col, row).determinant();
// 			}
// 		}
// 
// 		return result;
// 	}

// 	#WARNING#<value_type> minor(index_type row, index_type column) const 
// 	{
// 		index_type r = 0;
// 		#WARNING#<value_type> result;
// 
// 		for (index_type iRow = 0; iRow < Size; iRow++) 
// 		{
// 			index_type c = 0;
// 			if (iRow != row) 
// 			{
// 				for (index_type iCol = 0; iCol < Size; iCol++) 
// 				{
// 					if (iCol != column) 
// 					{
// 						result(r, c) = (*this)(iRow, iCol);
// 						c++;
// 					}
// 				}
// 				r++;
// 			}
// 		}
// 		return result;
// 	}

	value_type trace() const
	{
		return m11 + m22 + m33;
	}

	this_type transpose() const
	{
		this_type newMat(*this);
		std::swap(newMat.m12, newMat.m21);
		std::swap(newMat.m13, newMat.m31);
		std::swap(newMat.m23, newMat.m32);

		return newMat;
	}

//  	this_type inverse() const
//  	{
//  		this_type cofact = coFactor();
//  		this_type adjoindex_type(cofact);
//  
//  		adjoindex_type.transpose();
//  
//  		value_type det = determinant();
//  
//  		for (index_type i = 0; i < Size * Size; i++)
//  		{
//  			//for (index_type j = 0; j < Size; j++)
//  			//{
//  			adjoindex_type(i) /= det;
//  			//}
//  		}
//  
//  		return adjoindex_type;
//  	}

	Vector3<value_type> getColumnVector(index_type col) const
	{
		return (Vector3<value_type>((*this)(0, col), (*this)(1, col), (*this)(2, col)));
	}

	template < class V, class H >
	void setColumnVector(index_type col, const Vector3<V,H>& vec)
	{
		for (index_type i = 0; i < Size; i++)
			(*this)(i, col) = vec[i];
	}

// 	void orthoNormalize()
// 	{
// 		Vector3<value_type> xVec = getColumnVector(0).normal();
// 		Vector3<value_type> zVec = getColumnVector(2).normal();
// 		Vector3<value_type> yVec = zVec.crossp(xVec);
// 		zVec = xVec.crossp(yVec).normal();
// 		setColumnVector(0, xVec);
// 		setColumnVector(1, yVec);
// 		setColumnVector(2, zVec);
// 
// 		//for(index_type col = 0; col<Size; col++)
// 		//{
// 		//	Vector3F colVec = getColumnVector(col); 
// 		//	colVec.normalize();
// 		//	setColumnVector(col, colVec);
// 		//}
// 	}

	// Indexer allowing to access the matrix elements by an index
	// where index = Size*row + column.
	const value_type operator[](index_type index) const
	{			
		return *(&m11 + index);
	}

	const value_type operator()(index_type row, index_type column) const
	{			
		return *(&m11 + row * Size + column);
	}

	value_type& operator()(index_type row, index_type column)
	{			
		return *(&m11 + row * Size + column);
	}

	value_type& operator()(index_type index)
	{			
		return *(&m11 + index);
	}

	template < class V, class H >
	Vector3<V> operator*(const Vector3<V,H>& u) 
	{
		return Vector3<V>
			((m11 * u.x) + (m12 * u.y) + (m13 * u.z),
			(m21 * u.x) + (m22 * u.y) + (m23 * u.z),
			(m31 * u.x) + (m32 * u.y) + (m33 * u.z));
	}

	template < class V, class H >
	this_type operator*(const Matrix3<V,H>& u)
	{
		return Matrix3F(
			m11 * u.m11 + m12 * u.m21 + m13 * u.m31,
			m11 * u.m12 + m12 * u.m22 + m13 * u.m32,
			m11 * u.m13 + m12 * u.m23 + m13 * u.m33,

			m21 * u.m11 + m22 * u.m21 + m23 * u.m31,
			m21 * u.m12 + m22 * u.m22 + m23 * u.m32,
			m21 * u.m13 + m22 * u.m23 + m23 * u.m33,

			m31 * u.m11 + m32 * u.m21 + m33 * u.m31,
			m31 * u.m12 + m32 * u.m22 + m33 * u.m32, 
			m31 * u.m13 + m32 * u.m23 + m33 * u.m33
			);
	}
};

template < class V, class H > 
const typename Matrix3<V,H>::this_type Matrix3<V,H>::Identity = 
	Matrix3<V,H>(1, 0, 0, 0, 1, 0, 0, 0, 1);

template < class V, class H > 
const typename Matrix3<V,H>::index_type Matrix3<V,H>::Size = 3;

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;
typedef Matrix3<int> Matrix3i;

}

LIB_NAMESPACE_END

template <class U, class UH, class V, class VH> 
bool operator==(const LIB_NAMESPACE::math::Matrix3<U,UH>& u, const LIB_NAMESPACE::math::Matrix3<V,VH>& v) { 
	return u.m11 == v.m11 && u.m12 == v.m12 && u.m13 == v.m13 && 
		u.m21 == v.m21 && u.m22 == v.m22 && u.m23 == v.m23 && 
		u.m31 == v.m31 && u.m32 == v.m32 && u.m33 == v.m33;
}

template <class U, class UH, class V, class VH> 
bool operator!=(const LIB_NAMESPACE::math::Matrix3<U,UH>& u, const LIB_NAMESPACE::math::Matrix3<V,VH>& v) { 
	return u.m11 != v.m11 || u.m12 != v.m12 || u.m13 != v.m13 || 
		u.m21 != v.m21 || u.m22 != v.m22 || u.m23 != v.m23 || 
		u.m31 != v.m31 || u.m32 != v.m32 || u.m33 != v.m33;
}


template < class V, class H >
std::ostream& operator<<(std::ostream& str, const LIB_NAMESPACE::math::Matrix3<V,H>& u)
{
	str << u.m11 << " " << u.m12 << " " << u.m13 << " "
		<< u.m21 << " " << u.m22 << " " << u.m23 << " "
		<< u.m31 << " " << u.m32 << " " << u.m33;
	return str;
}

#endif // _MATH_MATRIX3_HPP