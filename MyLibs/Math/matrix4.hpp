#ifndef _MATRIX4_HPP
#define _MATRIX4_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

#include "matrix3.hpp"

LIB_NAMESPACE_START

namespace math {;

template < class V, class H > struct Matrix3;
template < class V, class H > struct Vector4;

template < class ValueType, class HashType = hash::DefaultHashType >
struct Matrix4
{
	typedef Matrix4<ValueType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;
	typedef unsigned int index_type;

	value_type m11, m12, m13, m14;
	value_type m21, m22, m23, m24;
	value_type m31, m32, m33, m34;
	value_type m41, m42, m43, m44;
 
	static const this_type Identity;
	static const index_type Size;

	Matrix4()
		: m11(1), m12(0), m13(0), m14(0),
		m21(0), m22(1), m23(0), m24(0),
		m31(0), m32(0), m33(1), m34(0),
		m41(0), m42(0), m43(0), m44(1)
	{

	}

	template < class OtherType >
	Matrix4(OtherType e11, OtherType e12, OtherType e13, OtherType e14,
		OtherType e21, OtherType e22, OtherType e23, OtherType e24,
		OtherType e31, OtherType e32, OtherType e33, OtherType e34,
		OtherType e41, OtherType e42, OtherType e43, OtherType e44)
		: m11(static_cast<value_type>(e11)), m12(static_cast<value_type>(e12)), m13(static_cast<value_type>(e13)), m14(static_cast<value_type>(e14)),
		m21(static_cast<value_type>(e21)), m22(static_cast<value_type>(e22)), m23(static_cast<value_type>(e23)), m24(static_cast<value_type>(e24)),
		m31(static_cast<value_type>(e31)), m32(static_cast<value_type>(e32)), m33(static_cast<value_type>(e33)), m34(static_cast<value_type>(e34)),
		m41(static_cast<value_type>(e41)), m42(static_cast<value_type>(e42)), m43(static_cast<value_type>(e43)), m44(static_cast<value_type>(e44))
	{
	}

	template < class OtherType >
	Matrix4(const OtherType *elements)
		: m11(elements[0]), m12(elements[1]), m13(elements[2]), m14(elements[3]), 
		m21(elements[4]), m22(elements[5]), m23(elements[6]), m24(elements[7]), 
		m31(elements[8]), m32(elements[9]), m33(elements[10]), m34(elements[11]), 
		m41(elements[12]), m42(elements[13]), m43(elements[14]), m44(elements[15])
	{
	}

	template < class V, class H >
	Matrix4(const Vector4<V,H>& column1, const Vector4<V,H>& column2, 
		const Vector4<V,H>& column3, Vector4<V,H>& column4)
		: m11(column1.x), m12(column2.x), m13(column3.x), m14(column4.x),
		m21(column1.y), m22(column2.y), m23(column3.y), m24(column4.y),
		m31(column1.z), m32(column2.z), m33(column3.z), m34(column4.z),
		m41(column1.w), m42(column2.w), m43(column3.w), m44(column4.w)
	{
	}

	Matrix4(const this_type& m)
		: m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
		m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
		m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
		m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44)
	{
	}

	template < class V, class H >
	explicit Matrix4(const Matrix4<V,H>& m)
		: m11(static_cast<value_type>(m.m11)), m12(static_cast<value_type>(m.m12)), m13(static_cast<value_type>(m.m13)), m14(static_cast<value_type>(m.m14)),
		m21(static_cast<value_type>(m.m21)), m22(static_cast<value_type>(m.m22)), m23(static_cast<value_type>(m.m23)), m24(static_cast<value_type>(m.m24)),
		m31(static_cast<value_type>(m.m31)), m32(static_cast<value_type>(m.m32)), m33(static_cast<value_type>(m.m33)), m34(static_cast<value_type>(m.m34)),
		m41(static_cast<value_type>(m.m41)), m42(static_cast<value_type>(m.m42)), m43(static_cast<value_type>(m.m43)), m44(static_cast<value_type>(m.m44))
	{
	}

	template < class V, class H >
	explicit Matrix4(const Matrix3<V,H>& m)
		: m11(m.m11), m12(m.m12), m13(m.m13), m14(0),
		m21(m.m21), m22(m.m22), m23(m.m23), m24(0),
		m31(m.m31), m32(m.m32), m33(m.m33), m34(0),
		m41(0), m42(0), m43(0), m44(1)
	{
	}

	void makeIdentity()
	{
		*this = Identity;
	}

	this_type coFactor() const
	{
		this_type coMatrix;
		Matrix3<value_type> adjoindex_typeMat;

		for (index_type j = 0; j < Size; j++)
		{
			for (index_type i = 0; i < Size; i++)
			{
				// Form the adjoindex_type a_ij
				index_type i1 = 0;
				for (index_type ii = 0; ii < Size; ii++)
				{
					if (ii == i)
						continue;
					index_type j1 = 0;
					for (index_type jj = 0; jj < Size; jj++)
					{
						if (jj == j)
							continue;
						adjoindex_typeMat(i1, j1) = (*this)(ii, jj);
						j1++;
					}
					i1++;
				}

				// Calculate the determinate and fill in the elements of the cofactor
				coMatrix(i, j) = (value_type)std::pow(-1.0, i + j + 2.0) * adjoindex_typeMat.determinant();
			}
		}

		return coMatrix;
	}

	value_type determinant() const
	{
		value_type det = static_cast<value_type>(0.0);
		for (index_type col = 0; col < Size; col++)
		{
			if ((col % 2) == 0)
				det += (*this)(0, col) * minor(0, col).determinant();
			else
				det -= (*this)(0, col) * minor(0, col).determinant();
		}
		return det;
	}

	this_type adjoindex_type() const 
	{
		this_type result;
		for (index_type row = 0; row < Size; row++)
		{
			for (index_type col = 0; col < Size; col++) 
			{
				if (((col+row) % 2) == 0)
					result(row, col) = minor(col, row).determinant();
				else
					result(row, col) = -minor(col, row).determinant();
			}
		}

		return result;
	}

	Matrix3<value_type, hash_type> minor(index_type row, index_type column) const 
	{
		index_type r = 0;
		Matrix3<value_type, hash_type> result;

		for (index_type iRow = 0; iRow < Size; iRow++) 
		{
			index_type c = 0;
			if (iRow != row) 
			{
				for (index_type iCol = 0; iCol < Size; iCol++) 
				{
					if (iCol != column) 
					{
						result(r, c) = (*this)(iRow, iCol);
						c++;
					}
				}
				r++;
			}
		}
		return result;
	}

	value_type trace() const
	{
		return m11 + m22 + m33 + m44;
	}

	this_type transpose() const
	{
		this_type newMat(*this);
		std::swap(newMat.m12, newMat.m21);
		std::swap(newMat.m13, newMat.m31);
		std::swap(newMat.m14, newMat.m41);
		std::swap(newMat.m23, newMat.m32);
		std::swap(newMat.m24, newMat.m42);
		std::swap(newMat.m34, newMat.m43);
		return newMat;
	}

	this_type inverse() const
	{
		this_type cofact = coFactor();
		this_type adjoindex_type(cofact);

		adjoindex_type = adjoindex_type.transpose();

		value_type det = determinant();

		for (index_type i = 0; i < Size*Size; i++)
		{
			//for (index_type j = 0; j < Size; j++)
			//{
			adjoindex_type(i) /= det;
			//}
		}

		return adjoindex_type;
	}

	Vector4<value_type> getColumnVector(index_type col) const
	{
		return (Vector4<value_type>((*this)(0, col), (*this)(1, col), (*this)(2, col), (*this)(3, col)));
	}

	template < class V, class H >
	void setColumnVector(index_type col, const Vector4<V,H>& vec)
	{
		for (index_type i = 0; i < Size; i++)
			(*this)(i, col) = vec[i];
	}

	void orthoNormalize()
	{
		Vector4<value_type> xVec = getColumnVector(0).normal();
		Vector4<value_type> zVec = getColumnVector(2).normal();
		Vector4<value_type> yVec = zVec.crossp(xVec);
		zVec = xVec.crossp(yVec).normal();
		setColumnVector(0, xVec);
		setColumnVector(1, yVec);
		setColumnVector(2, zVec);

		//for(index_type col = 0; col<3; col++)
		//{
		//	Vector4F colVec = getColumnVector(col); 
		//	colVec.normalize();
		//	setColumnVector(col, colVec);
		//}
	}

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
	Vector4<value_type> operator*(const Vector4<V,H>& u) const
	{
		return Vector4<V,H>((m11 * u.x) + (m12 * u.y) + (m13 * u.z) + (m14 * u.w),
			(m21 * u.x) + (m22 * u.y) + (m23 * u.z) + (m24 * u.w),
			(m31 * u.x) + (m32 * u.y) + (m33 * u.z) + (m34 * u.w),
			(m41 * u.x) + (m42 * u.y) + (m43 * u.z) + (m44 * u.w));

	}

	template < class V, class H >
	this_type operator*(const Matrix4<V,H>& u) const
	{
		return this_type(
			m11 * u.m11 + m12 * u.m21 + m13 * u.m31 + m14 * u.m41,
			m11 * u.m12 + m12 * u.m22 + m13 * u.m32 + m14 * u.m42,
			m11 * u.m13 + m12 * u.m23 + m13 * u.m33 + m14 * u.m43,
			m11 * u.m14 + m12 * u.m24 + m13 * u.m34 + m14 * u.m44,

			m21 * u.m11 + m22 * u.m21 + m23 * u.m31 + m24 * u.m41,
			m21 * u.m12 + m22 * u.m22 + m23 * u.m32 + m24 * u.m42,
			m21 * u.m13 + m22 * u.m23 + m23 * u.m33 + m24 * u.m43,
			m21 * u.m14 + m22 * u.m24 + m23 * u.m34 + m24 * u.m44,

			m31 * u.m11 + m32 * u.m21 + m33 * u.m31 + m34 * u.m41,
			m31 * u.m12 + m32 * u.m22 + m33 * u.m32 + m34 * u.m42, 
			m31 * u.m13 + m32 * u.m23 + m33 * u.m33 + m34 * u.m43,
			m31 * u.m14 + m32 * u.m24 + m33 * u.m34 + m34 * u.m44,

			m41 * u.m11 + m42 * u.m21 + m43 * u.m31 + m44 * u.m41,
			m41 * u.m12 + m42 * u.m22 + m43 * u.m32 + m44 * u.m42, 
			m41 * u.m13 + m42 * u.m23 + m43 * u.m33 + m44 * u.m43,
			m41 * u.m14 + m42 * u.m24 + m43 * u.m34 + m44 * u.m44
		);
	}
};

template < class V, class H > const typename Matrix4<V,H>::this_type Matrix4<V,H>::Identity = Matrix4<V,H>(1, 0, 0, 0, 
																							0, 1, 0, 0, 
																							0, 0, 1, 0, 
																							0, 0, 0, 1);
template < class V, class H > const typename Matrix4<V,H>::index_type Matrix4<V,H>::Size = 4;

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

}

LIB_NAMESPACE_END

template <class U, class UH, class V, class VH> 
bool operator==(const LIB_NAMESPACE::math::Matrix4<U,UH>& u, const LIB_NAMESPACE::math::Matrix4<V,VH>& v) { 
	return u.m11 == v.m11 && u.m12 == v.m12 && u.m13 == v.m13 && u.m14 == v.m14 && 
		u.m21 == v.m21 && u.m22 == v.m22 && u.m23 == v.m23 && u.m24 == v.m24 && 
		u.m31 == v.m31 && u.m32 == v.m32 && u.m33 == v.m33 && u.m34 == v.m34 && 
		u.m41 == v.m41 && u.m42 == v.m42 && u.m43 == v.m43 && u.m44 == v.m44;
}

template <class U, class UH, class V, class VH> 
bool operator!=(const LIB_NAMESPACE::math::Matrix4<U,UH>& u, const LIB_NAMESPACE::math::Matrix4<V,VH>& v) { 
	return u.m11 != v.m11 || u.m12 != v.m12 || u.m13 != v.m13 || u.m14 != v.m14 || 
		u.m21 != v.m21 || u.m22 != v.m22 || u.m23 != v.m23 || u.m24 != v.m24 || 
		u.m31 != v.m31 || u.m32 != v.m32 || u.m33 != v.m33 || u.m34 != v.m34 || 
		u.m41 != v.m41 || u.m42 != v.m42 || u.m43 != v.m43 || u.m44 != v.m44;
}

template < class V, class H >
std::ostream& operator<<(std::ostream& str, const LIB_NAMESPACE::math::Matrix4<V,H>& u)
{
	str << u.m11 << " " << u.m12 << " " << u.m13 << " " << u.m14 << " "
		<< u.m21 << " " << u.m22 << " " << u.m23 << " " << u.m24 << " "
		<< u.m31 << " " << u.m32 << " " << u.m33 << " " << u.m34 << " "
		<< u.m41 << " " << u.m42 << " " << u.m43 << " " << u.m44;
	return str;
}

#endif // _MATRIX4_HPP