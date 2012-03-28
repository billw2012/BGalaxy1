#ifndef _VECTOR2_HPP
#define _VECTOR2_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

LIB_NAMESPACE_START

namespace math
{

template < class V, class H > struct Vector3;
template < class V, class H > struct Vector4;

template < class ValueType, class HashType = hash::DefaultHashType >
struct Vector2
{
	typedef Vector2<ValueType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;
	typedef unsigned int index_type;

	value_type x;
	value_type y;

	static const this_type XAxis;
	static const this_type YAxis;
	static const this_type Zero;
	static const this_type One;
	static const this_type Min;
	static const this_type Max;

	// Construct default vector (0, 0)
	Vector2() : x(static_cast<value_type>(0)), y(static_cast<value_type>(0)) {}
	// Construct vector using specified values
	template < class OtherType >
	Vector2(OtherType xi, OtherType yi) : x(static_cast<value_type>(xi)), y(static_cast<value_type>(yi)) {}
	// Construct vector using specified values array
	template < class OtherType >
	Vector2(const OtherType* const coordinates) 
		: x(coordinates[0]), y(coordinates[1]) {}
	// Copy constructor
	Vector2(const this_type& vector) : x(vector.x), y(vector.y) {}
	template < class V, class H >
	explicit Vector2(const Vector2<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)) {}
	template < class V, class H >
	explicit Vector2(const Vector3<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)) {}
	template < class V, class H >
	explicit Vector2(const Vector4<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)) {}

	template < class V, class H >
	value_type dotp(const Vector2<V,H>& v) const
	{
		return (x * v.x) + (y * v.y);
	}

	void normalize()
	{
		value_type len = length();
		if (len != 0)
		{
			value_type invLen = static_cast<value_type>(1) / len;
			x *= invLen;
			y *= invLen;
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
		return (x * x + y * y);
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
			*reinterpret_cast<const hash_type* >(&y);
	}

	bool operator<(const this_type& other) const
	{
		if(x < other.x)
			return true;
		if(x > other.x)
			return false;
		return y < other.y;
	}

};

template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::XAxis = Vector2<V, H>(V(1), V(0));
template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::YAxis = Vector2<V, H>(V(0), V(1));
template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::Zero = Vector2<V, H>(V(0), V(0));
template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::One = Vector2<V, H>(V(1), V(1));
template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::Min = Vector2<V, H>(-std::numeric_limits<V>::max(), -std::numeric_limits<V>::max());
template< class V, class H > const typename Vector2<V, H>::this_type Vector2<V, H>::Max = Vector2<V, H>(std::numeric_limits<V>::max(), std::numeric_limits<V>::max());

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;

#define DEFINE_V2_UNARY_OPERATOR(op) template <class U, class UH> \
	LIB_NAMESPACE::math::Vector2<U,UH> operator##op(const LIB_NAMESPACE::math::Vector2<U,UH>& u) { \
		return LIB_NAMESPACE::math::Vector2<U,UH>(op u.x, op u.y); \
	}

#define DEFINE_V2V2_BINARY_OPERATOR(op)	template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector2<U,UH> operator##op(const LIB_NAMESPACE::math::Vector2<U,UH>& u, const LIB_NAMESPACE::math::Vector2<V,VH>& v) { \
	return LIB_NAMESPACE::math::Vector2<U,UH>(u.x op v.x, u.y op v.y); \
}

#define DEFINE_V2S_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector2<U,UH> operator##op(const LIB_NAMESPACE::math::Vector2<U,UH>& u, V v) { \
	return LIB_NAMESPACE::math::Vector2<U,UH>(u.x op v, u.y op v); \
}

#define DEFINE_SV2_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector2<U,UH> operator##op(V v, const LIB_NAMESPACE::math::Vector2<U,UH>& u) { \
	return LIB_NAMESPACE::math::Vector2<U,UH>(v op u.x, v op u.y); \
}

#define DEFINE_V2V2_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector2<U,UH>& operator##op(LIB_NAMESPACE::math::Vector2<U,UH>& u, const LIB_NAMESPACE::math::Vector2<V,VH>& v) { \
	u.x op v.x; u.y op v.y; return u; \
}

#define DEFINE_V2S_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector2<U,UH>& operator##op(LIB_NAMESPACE::math::Vector2<U,UH>& u, V v) { \
	u.x op v; u.y op v; return u; \
}

DEFINE_V2_UNARY_OPERATOR(-)

DEFINE_V2V2_BINARY_OPERATOR(+)
DEFINE_V2V2_BINARY_OPERATOR(-)
DEFINE_V2V2_BINARY_OPERATOR(/)
DEFINE_V2V2_BINARY_OPERATOR(*)

DEFINE_V2S_BINARY_OPERATOR(+)
DEFINE_V2S_BINARY_OPERATOR(-)
DEFINE_V2S_BINARY_OPERATOR(/)
DEFINE_V2S_BINARY_OPERATOR(*)

DEFINE_SV2_BINARY_OPERATOR(+)
DEFINE_SV2_BINARY_OPERATOR(-)
DEFINE_SV2_BINARY_OPERATOR(/)
DEFINE_SV2_BINARY_OPERATOR(*)

DEFINE_V2V2_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V2V2_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V2V2_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V2V2_MODIFY_BINARY_OPERATOR(*=)

DEFINE_V2S_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V2S_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V2S_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V2S_MODIFY_BINARY_OPERATOR(*=)

template <class U, class UH, class V, class VH> 
bool operator==(const LIB_NAMESPACE::math::Vector2<U,UH>& u, const LIB_NAMESPACE::math::Vector2<V,VH>& v) { 
	return u.x == v.x && u.y == v.y;
}

template <class U, class UH, class V, class VH> 
bool operator!=(const LIB_NAMESPACE::math::Vector2<U,UH>& u, const LIB_NAMESPACE::math::Vector2<V,VH>& v) { 
	return u.x != v.x || u.y != v.y;
}

template < class V, class H >
std::ostream& operator<<(std::ostream& str, const LIB_NAMESPACE::math::Vector2<V,H>& u)
{
	str << u.x << " " << u.y;
	return str;
}


template < class FTy_, class HTy_ >
math::Vector2<FTy_, HTy_> clamp(const math::Vector2<FTy_, HTy_>& val, const math::Vector2<FTy_, HTy_>& minVal, const math::Vector2<FTy_, HTy_>& maxVal)
{
	return math::Vector2<FTy_, HTy_>(
		clamp(val.x, minVal.x, maxVal.x),
		clamp(val.y, minVal.y, maxVal.y)
		);
}

}

LIB_NAMESPACE_END

#endif // _VECTOR2_HPP