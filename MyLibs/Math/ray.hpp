#ifndef _MATH_RAY_HPP
#define _MATH_RAY_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

LIB_NAMESPACE_START

namespace math
{

template < class ValueType, class HashType = hash::DefaultHashType >
struct Ray
{
	typedef Vector3< ValueType, HashType > VectorType;
	VectorType origin;
	VectorType direction;

	template < class V, class H >
	Ray(const Vector3< V,H >& _origin, Vector3< V,H > _direction) : origin(_origin), direction(_direction) {}

};

typedef Ray<float> Rayf;
typedef Ray<double> Rayd;

}

LIB_NAMESPACE_END

#endif // _MATH_RAY_HPP
