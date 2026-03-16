#ifndef _MATH_MISC_HPP
#define _MATH_MISC_HPP

#undef min
#undef max

LIB_NAMESPACE_START

namespace math
{

namespace Sign
{
	enum SignType
	{
		NEGATIVE = -1,
		ZERO = 0,
		POSITIVE = 1			
	};
	template < class ValueType >
	SignType sign(ValueType f, ValueType tolerance = 0)
	{
		if( f > tolerance )	
		{					
			return POSITIVE;
		}
		if( f < -tolerance )	
		{
			return NEGATIVE;
		}
		return ZERO;
	}
}

template < class Ty_ >
Ty_ clamp(const Ty_& val, const Ty_& minVal, const Ty_& maxVal)
{
	return std::min(maxVal, std::max(minVal, val));
}

// fast floor function (x87 asm replaced for x64 compatibility)
inline int lrfloorf(float flt)
{
	return static_cast<int>(std::floor(flt));
}

// not fast floor function!
inline int lrfloorf(double flt)
{
	return (int)floor(flt);
}

template < class Ty_ >
Ty_ my_rand(Ty_ maxVal)
{
	return (rand() / (Ty_)RAND_MAX) * maxVal;
}

template < class Ty_ >
Ty_ my_rand(Ty_ minVal, Ty_ maxVal)
{
	return minVal + (rand() / (Ty_)RAND_MAX) * (maxVal - minVal);
}


}

LIB_NAMESPACE_END

#endif // _MATH_MISC_HPP
