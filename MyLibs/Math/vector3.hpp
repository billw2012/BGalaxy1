#ifndef _VECTOR3_HPP
#define _VECTOR3_HPP

#include <limits>
#include <cmath>
#include <iostream>

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

#undef min
#undef max

LIB_NAMESPACE_START

namespace math {;

template < class V, class H > struct Vector2;
template < class V, class H > struct Vector4;

template < class ValueType, class HashType = hash::DefaultHashType >
struct Vector3
{
	typedef Vector3<ValueType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;
	typedef unsigned int index_type;

	value_type x;
	value_type y;
	value_type z;
	value_type __alignment;

	static const this_type XAxis;
	static const this_type YAxis;
	static const this_type ZAxis;
	static const this_type Zero;
	static const this_type One;
	static const this_type Min;
	static const this_type Max;

	// Construct default vector (0, 0, 0)
	Vector3() : x(static_cast<value_type>(0)), y(static_cast<value_type>(0)), z(static_cast<value_type>(0)) {}
	// Construct vector using specified values
	Vector3(value_type xi, value_type yi, value_type zi) : x(xi), y(yi), z(zi) {}
	// Construct vector using specified values
	template < class OtherType >
	Vector3(OtherType xi, OtherType yi, OtherType zi) : x(xi), y(yi), z(zi) {}
	// Construct vector using specified values array
	template < class OtherType >
	Vector3(const OtherType* const coordinates) 
		: x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {}
	// Copy constructor
	Vector3(const this_type& vector) : x(vector.x), y(vector.y), z(vector.z) {}

	template < class V, class H >
	explicit Vector3(const Vector3<V,H>& vector) 
		: x(static_cast<value_type>(vector.x)), 
		y(static_cast<value_type>(vector.y)), 
		z(static_cast<value_type>(vector.z)) {}

	template < class V, class H >
	explicit Vector3(const Vector4<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(vector.z)) {}

	template < class V, class H, class V2 >
	explicit Vector3(const Vector2<V,H>& vector, V2 z_) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(z_)) {}

	template < class V, class H >
	explicit Vector3(const Vector2<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(0)) {}

	template < class V, class H >
	value_type dotp(const Vector3<V,H>& v) const
	{
		return static_cast<value_type>((x * v.x) + (y * v.y) + (z * v.z));
	}

	template < class V, class H >
	value_type angle(const Vector3<V,H>& v) const
	{
		return static_cast<value_type>(std::acos( MY_MAX(MY_MIN(dotp(v), 1.0), -1.0) ));
	}

	template <class V, class H >
	this_type crossp(const Vector3<V,H>& v) const
	{
		return this_type( 
			y*v.z - z*v.y, 
			z*v.x - x*v.z, 
			x*v.y - y*v.x );
	}

	void normalize()
	{
		value_type len = length();
		if (len != static_cast<value_type>(0))
		{
			value_type invLen = static_cast<value_type>(1) / len;
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
	}

	this_type normal() const
	{
		this_type norm(*this);
		norm.normalize();
		return norm;
	}

	value_type length() const
	{
		value_type lenSquared = lengthSquared();
		return (lenSquared == static_cast<value_type>(0)) ? static_cast<value_type>(0) : static_cast<value_type>(std::sqrt(static_cast<double>(lenSquared)));
	}

	value_type lengthSquared() const
	{
		return (x * x + y * y + z * z);
	}

	value_type operator[](index_type index) const
	{
		return *(&x + index);
	}

	value_type& operator()(index_type index) 
	{
		return *(&x + index);
	}

	hash_type hashcode() const
	{
		return *reinterpret_cast<const hash_type* >(&x) ^
			*reinterpret_cast<const hash_type* >(&y) ^
			*reinterpret_cast<const hash_type* >(&z);
	}
};

template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::XAxis = Vector3<V, H>(V(1), V(0), V(0));
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::YAxis = Vector3<V, H>(V(0), V(1), V(0));
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::ZAxis = Vector3<V, H>(V(0), V(0), V(1));
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::Zero = Vector3<V, H>(V(0), V(0), V(0));
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::One = Vector3<V, H>(V(1), V(1), V(1));
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::Min = Vector3<V, H>(-std::numeric_limits<V>::max(), -std::numeric_limits<V>::max(), -std::numeric_limits<V>::max());
template<class V, class H> const typename Vector3<V, H>::this_type Vector3<V, H>::Max = Vector3<V, H>(std::numeric_limits<V>::max(), std::numeric_limits<V>::max(), std::numeric_limits<V>::max());

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;
typedef Vector3<unsigned int> Vector3u;

#define DEFINE_V3_UNARY_OPERATOR(op) template <class U, class UH> \
	LIB_NAMESPACE::math::Vector3<U,UH> operator##op(const LIB_NAMESPACE::math::Vector3<U,UH>& u) { \
		return LIB_NAMESPACE::math::Vector3<U,UH>(op u.x, op u.y, op u.z); \
	}

#define DEFINE_V3V3_BINARY_OPERATOR(op) template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector3<U,UH> operator##op(const LIB_NAMESPACE::math::Vector3<U,UH>& u, const LIB_NAMESPACE::math::Vector3<V,VH>& v) { \
	return LIB_NAMESPACE::math::Vector3<U,UH>(u.x op v.x, u.y op v.y, u.z op v.z); \
}

#define DEFINE_V3S_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector3<U,UH> operator##op(const LIB_NAMESPACE::math::Vector3<U,UH>& u, V v) { \
	return LIB_NAMESPACE::math::Vector3<U,UH>(u.x op v, u.y op v, u.z op v); \
}

#define DEFINE_SV3_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector3<U,UH> operator##op(V v, const LIB_NAMESPACE::math::Vector3<U,UH>& u) { \
	return LIB_NAMESPACE::math::Vector3<U,UH>(v op u.x, v op u.y, v op u.z); \
}

#define DEFINE_V3V3_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector3<U,UH>& operator##op(LIB_NAMESPACE::math::Vector3<U,UH>& u, const LIB_NAMESPACE::math::Vector3<V,VH>& v) { \
	u.x op v.x; u.y op v.y; u.z op v.z; return u; \
}

#define DEFINE_V3S_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector3<U,UH>& operator##op(LIB_NAMESPACE::math::Vector3<U,UH>& u, V v) { \
	u.x op v; u.y op v; u.z op v; return u; \
}

DEFINE_V3_UNARY_OPERATOR(-)

DEFINE_V3V3_BINARY_OPERATOR(+)
DEFINE_V3V3_BINARY_OPERATOR(-)
DEFINE_V3V3_BINARY_OPERATOR(/)
DEFINE_V3V3_BINARY_OPERATOR(*)

DEFINE_V3S_BINARY_OPERATOR(+)
DEFINE_V3S_BINARY_OPERATOR(-)
DEFINE_V3S_BINARY_OPERATOR(/)
DEFINE_V3S_BINARY_OPERATOR(*)

DEFINE_SV3_BINARY_OPERATOR(+)
DEFINE_SV3_BINARY_OPERATOR(-)
DEFINE_SV3_BINARY_OPERATOR(/)
DEFINE_SV3_BINARY_OPERATOR(*)

DEFINE_V3V3_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V3V3_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V3V3_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V3V3_MODIFY_BINARY_OPERATOR(*=)

DEFINE_V3S_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V3S_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V3S_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V3S_MODIFY_BINARY_OPERATOR(*=)

template <class U, class UH, class V, class VH> 
bool operator==(const LIB_NAMESPACE::math::Vector3<U,UH>& u, const LIB_NAMESPACE::math::Vector3<V,VH>& v) { 
	return u.x == v.x && u.y == v.y && u.z == v.z;
}

template <class U, class UH, class V, class VH> 
bool operator!=(const LIB_NAMESPACE::math::Vector3<U,UH>& u, const LIB_NAMESPACE::math::Vector3<V,VH>& v) { 
	return u.x != v.x || u.y != v.y || u.z != v.z;
}

template < class V, class H >
std::ostream& operator<<(std::ostream& str, const LIB_NAMESPACE::math::Vector3<V,H>& u)
{
	str << u.x << " " << u.y << " " << u.z;
	return str;
}

template < class FTy_, class HTy_ >
math::Vector3<FTy_, HTy_> clamp(const math::Vector3<FTy_, HTy_>& val, 
								const math::Vector3<FTy_, HTy_>& minVal, 
								const math::Vector3<FTy_, HTy_>& maxVal)
{
	return math::Vector3<FTy_, HTy_>(
		clamp(val.x, minVal.x, maxVal.x),
		clamp(val.y, minVal.y, maxVal.y),
		clamp(val.z, minVal.z, maxVal.z)
		);
}

}

LIB_NAMESPACE_END

#endif // _VECTOR3_HPP