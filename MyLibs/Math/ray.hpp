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
	typedef Ray< ValueType, HashType > this_type;

	VectorType origin;
	VectorType direction;

	template < class V, class H >
	Ray() {}

	template < class V, class H >
	Ray(const Vector3< V,H >& _origin, Vector3< V,H > _direction) : origin(_origin), direction(_direction) {}

	std::pair<float, float> distance(const this_type& ray2) const
	{
		VectorType p2  = origin			+ direction;
		VectorType p4  = ray2.origin	+ ray2.direction;
		VectorType d21 = origin			- p2; // 3 sub x 3
		VectorType d34 = p4				- ray2.origin;
		VectorType d13 = ray2.origin	- origin;

		// m * u = x
		float a =  d21.dotp(d21); // (3 mul + 3 add ) x 5
		float b =  d21.dotp(d34);
		float c =  d34.dotp(d34);
		float d = -d13.dotp(d21);
		float e = -d13.dotp(d34);

		// Solve for u1 &amp;amp;amp;amp;amp;amp;amp;amp; u2
		float u = (d*c-e*b)/(c*a-b*b);
		return	/*u[0] =*/ std::pair<float, float>(u, // 4 mul, 2 sub, 1 div
				/*u[1] =*/ (e - b * u) / c); // 1 mul, 1 sub, 1 div
		// Total # of FP operations
		// 22 multiplications and division
		// 27 additions and subtractions
	}
};

typedef Ray<float> Rayf;
typedef Ray<double> Rayd;

}

LIB_NAMESPACE_END

#endif // _MATH_RAY_HPP
