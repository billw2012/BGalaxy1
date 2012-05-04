#ifndef hermite_h__
#define hermite_h__

#include "vector3.hpp"

namespace math {;

// Hermite basis functions:
// h00(t) = 2t^3 - 3t^2 + 1
// h10(t) = t^3 - 2t^2 + t
// h01(t) = -2t^3 + 3t^2
// h11(t) = t^3 - t^2

template < class ValTy_ >
struct Hermite
{
	typedef ValTy_ value_type;
	typedef Vector3<ValTy_> vector_type;

	// p = point, m = tangent
	vector_type p0, m0;
	vector_type p1, m1;

	Hermite() {}
	Hermite(const vector_type& p0_, const vector_type& m0_,	
		const vector_type& p1_, const vector_type& m1_)
		: p0(p0_), m0(m0_), p1(p1_), m1(m1_) {}

	vector_type eval(value_type t) const
	{
		value_type h00t = 2 * t*t*t - 3 * t*t + 1;
		value_type h10t = t*t*t - 2 * t*t + t;
		value_type h01t = -2 * t*t*t + 3 * t*t;
		value_type h11t = t*t*t - t*t;
		return h00t * p0 + h10t * m0 + h01t * p1 + h11t * m1;
	}
};

}

#endif // hermite_h__
