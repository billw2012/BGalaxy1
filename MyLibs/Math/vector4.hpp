#ifndef _VECTOR4_HPP
#define _VECTOR4_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

LIB_NAMESPACE_START

namespace math
{
	
template < class V, class H > struct Vector2;
template < class V, class H > struct Vector3;

template < class ValueType, class HashType = hash::DefaultHashType >
struct Vector4
{
	typedef Vector4<ValueType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;
	typedef unsigned int index_type;

	value_type x;
	value_type y;
	value_type z;
	value_type w;

	static const this_type XAxis;
	static const this_type YAxis;
	static const this_type ZAxis;
	static const this_type WAxis;
	static const this_type Zero;
	static const this_type One;
	static const this_type Min;
	static const this_type Max;

	// Construct default vector (0, 0, 0, 0)
	Vector4() : x(static_cast<value_type>(0)), y(static_cast<value_type>(0)), z(static_cast<value_type>(0)), w(static_cast<value_type>(0)) {}
	// Construct vector using specified values
	template < class OtherType >
	Vector4(OtherType xi, OtherType yi, OtherType zi, OtherType wi = 0) : x(static_cast<value_type>(xi)), y(static_cast<value_type>(yi)), z(static_cast<value_type>(zi)), w(static_cast<value_type>(wi)) {}
	// Construct vector using specified values array
	template < class OtherType >
	Vector4(const OtherType* const coordinates) 
		: x(coordinates[0]), y(coordinates[1]), z(coordinates[2]), w(coordinates[3]) {}
	// Copy constructor
	Vector4(const this_type& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

	template < class V, class H >
	explicit Vector4(const Vector4<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(vector.z)), w(static_cast<value_type>(vector.w)) {}

	template < class V, class H >
	explicit Vector4(const Vector3<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(vector.z)), w(static_cast<value_type>(0)) {}

	template < class V, class H, class V2 >
	explicit Vector4(const Vector3<V,H>& vector, V2 w_) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(vector.z)), w(static_cast<value_type>(w_)) {}

	template < class V, class H >
	explicit Vector4(const Vector2<V,H>& vector) : x(static_cast<value_type>(vector.x)), y(static_cast<value_type>(vector.y)), z(static_cast<value_type>(0)), w(static_cast<value_type>(0)) {}

	template < class V, class H >
	value_type dotp(const Vector4<V,H>& v) const
	{
		return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
	}

	template < class V, class H >
	this_type crossp(const Vector4<V,H>& v) const
	{
		return this_type( 
			y*v.z - z*v.y, 
			z*v.x - x*v.z, 
			x*v.y - y*v.x, static_cast<V>(0) );
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
			w *= invLen;
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
			*reinterpret_cast<const hash_type* >(&z) ^
			*reinterpret_cast<const hash_type* >(&w);
	}

};

template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::XAxis = Vector4<V, H>(V(1), V(0), V(0), V(0));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::YAxis = Vector4<V, H>(V(0), V(1), V(0), V(0));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::ZAxis = Vector4<V, H>(V(0), V(0), V(1), V(0));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::WAxis = Vector4<V, H>(V(0), V(0), V(0), V(1));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::Zero = Vector4<V, H>(V(0), V(0), V(0), V(0));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::One = Vector4<V, H>(V(1), V(1), V(1), V(1));
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::Min = Vector4<V, H>(-std::numeric_limits<V>::max(), -std::numeric_limits<V>::max(), -std::numeric_limits<V>::max(), -std::numeric_limits<V>::max());
template< class V, class H > const typename Vector4<V, H>::this_type Vector4<V, H>::Max = Vector4<V, H>(std::numeric_limits<V>::max(), std::numeric_limits<V>::max(), std::numeric_limits<V>::max(), std::numeric_limits<V>::max());

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int> Vector4i;
typedef Vector4<unsigned int> Vector4u;

#define DEFINE_V4_UNARY_OPERATOR(op) template <class U> \
	LIB_NAMESPACE::math::Vector4<U> operator##op(const LIB_NAMESPACE::math::Vector4<U>& u) { \
		return LIB_NAMESPACE::math::Vector4<U>(op u.x, op u.y, op u.z, op u.w); \
	}

#define DEFINE_V4V4_BINARY_OPERATOR(op) template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector4<U,UH> operator##op(const LIB_NAMESPACE::math::Vector4<U,UH>& u, const LIB_NAMESPACE::math::Vector4<V,VH>& v) { \
	return LIB_NAMESPACE::math::Vector4<U,UH>(u.x op v.x, u.y op v.y, u.z op v.z, u.w op v.w); \
}

#define DEFINE_V4S_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector4<U,UH> operator##op(const LIB_NAMESPACE::math::Vector4<U,UH>& u, V v) { \
	return LIB_NAMESPACE::math::Vector4<U,UH>(u.x op v, u.y op v, u.z op v, u.w op v); \
}

#define DEFINE_SV4_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector4<U,UH> operator##op(V v, const LIB_NAMESPACE::math::Vector4<U,UH>& u) { \
	return LIB_NAMESPACE::math::Vector4<U,UH>(v op u.x, v op u.y, v op u.z, v op u.w); \
}

#define DEFINE_V4V4_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V, class VH> \
	LIB_NAMESPACE::math::Vector4<U,UH>& operator##op(LIB_NAMESPACE::math::Vector4<U,UH>& u, const LIB_NAMESPACE::math::Vector4<V,VH>& v) { \
	u.x op v.x; u.y op v.y; u.z op v.z; u.w op v.w; return u; \
}

#define DEFINE_V4S_MODIFY_BINARY_OPERATOR(op) template <class U, class UH, class V> \
	LIB_NAMESPACE::math::Vector4<U,UH>& operator##op(LIB_NAMESPACE::math::Vector4<U,UH>& u, V v) { \
	u.x op v; u.y op v; u.z op v; u.w op v; return u; \
}

DEFINE_V4_UNARY_OPERATOR(-)

DEFINE_V4V4_BINARY_OPERATOR(+)
DEFINE_V4V4_BINARY_OPERATOR(-)
DEFINE_V4V4_BINARY_OPERATOR(/)
DEFINE_V4V4_BINARY_OPERATOR(*)

DEFINE_V4S_BINARY_OPERATOR(+)
DEFINE_V4S_BINARY_OPERATOR(-)
DEFINE_V4S_BINARY_OPERATOR(/)
DEFINE_V4S_BINARY_OPERATOR(*)

DEFINE_SV4_BINARY_OPERATOR(+)
DEFINE_SV4_BINARY_OPERATOR(-)
DEFINE_SV4_BINARY_OPERATOR(/)
DEFINE_SV4_BINARY_OPERATOR(*)

DEFINE_V4V4_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V4V4_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V4V4_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V4V4_MODIFY_BINARY_OPERATOR(*=)

DEFINE_V4S_MODIFY_BINARY_OPERATOR(+=)
DEFINE_V4S_MODIFY_BINARY_OPERATOR(-=)
DEFINE_V4S_MODIFY_BINARY_OPERATOR(/=)
DEFINE_V4S_MODIFY_BINARY_OPERATOR(*=)

template <class U, class UH, class V, class VH> 
bool operator==(const LIB_NAMESPACE::math::Vector4<U,UH>& u, const LIB_NAMESPACE::math::Vector4<V,VH>& v) { 
	return u.x == v.x && u.y == v.y && u.z == v.z && u.w == v.w;
}

template <class U, class UH, class V, class VH> 
bool operator!=(const LIB_NAMESPACE::math::Vector4<U,UH>& u, const LIB_NAMESPACE::math::Vector4<V,VH>& v) { 
	return u.x != v.x || u.y != v.y || u.z != v.z || u.w != v.w;
}

template < class V, class H >
std::ostream& operator<<(std::ostream& str, const LIB_NAMESPACE::math::Vector4<V,H>& u)
{
	str << u.x << " " << u.y << " " << u.z << " " << u.w;
	return str;
}

template < class FTy_, class HTy_ >
math::Vector4<FTy_, HTy_> clamp(const math::Vector4<FTy_, HTy_>& val, 
								const math::Vector4<FTy_, HTy_>& minVal, 
								const math::Vector4<FTy_, HTy_>& maxVal)
{
	return math::Vector4<FTy_, HTy_>(
		clamp(val.x, minVal.x, maxVal.x),
		clamp(val.y, minVal.y, maxVal.y),
		clamp(val.z, minVal.z, maxVal.z),
		clamp(val.w, minVal.w, maxVal.w)
		);
}

}

LIB_NAMESPACE_END

#endif // _VECTOR4_HPP