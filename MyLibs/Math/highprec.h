
#if !defined(__MATHS_HIGHPREC_H__)
#define __MATHS_HIGHPREC_H__

#include "hpalib/src/xpre.h"

namespace math {;

typedef xpr hp_float;

inline hp_float operator+(const hp_float& op1, const hp_float& op2)
{	
	return xadd(op1, op2, 0);
}

inline hp_float operator-(const hp_float& op1, const hp_float& op2)
{	
	return xadd(op1, op2, 1);
}

inline hp_float operator*(const hp_float& op1, const hp_float& op2)
{	
	return xmul(op1, op2);
}

inline hp_float operator/(const hp_float& op1, const hp_float& op2)
{	
	return xdiv(op1, op2);
}

inline hp_float& operator+=(hp_float& op1, const hp_float& op2)
{	
	return (op1 = xadd(op1, op2, 0));
}

inline hp_float& operator-=(hp_float& op1, const hp_float& op2)
{	
	return (op1 = xadd(op1, op2, 1));
}

inline hp_float& operator*=(hp_float& op1, const hp_float& op2)
{	
	return (op1 = xmul(op1, op2));
}

inline hp_float& operator/=(hp_float& op1, const hp_float& op2)
{	
	return (op1 = xdiv(op1, op2));
}

inline hp_float operator-(const hp_float& op1)
{	
	return xneg(op1);
}

struct HighPrecFloat
{
	HighPrecFloat(hp_float value_) : value(value_) {}
	explicit HighPrecFloat(double dvalue_) : value(dbltox(dvalue_)) {}
	explicit HighPrecFloat(float dvalue_) : value(flttox(dvalue_)) {}
	explicit HighPrecFloat(long dvalue_) : value(inttox(dvalue_)) {}
	explicit HighPrecFloat(unsigned long dvalue_) : value(uinttox(dvalue_)) {}
	explicit HighPrecFloat(int dvalue_) : value(inttox((long)dvalue_)) {}
	explicit HighPrecFloat(unsigned int dvalue_) : value(uinttox((unsigned long)dvalue_)) {}
	
	HighPrecFloat() {}

	operator hp_float() const 
	{ 
		return value;
	}

	operator double() const 
	{
		return xtodbl(value);
	}

	operator float() const 
	{
		return xtoflt(value);
	}

	inline HighPrecFloat operator+(const HighPrecFloat& op) const 
	{	
		return value + op.value;
	}

	inline HighPrecFloat operator-(const HighPrecFloat& op) const 
	{	
		return value - op.value;
	}

	inline HighPrecFloat operator*(const HighPrecFloat& op) const 
	{	
		return value * op.value;
	}

	inline HighPrecFloat operator/(const HighPrecFloat& op) const 
	{	
		return value / op.value;
	}

	inline HighPrecFloat& operator+=(const HighPrecFloat& op)
	{	
		value += op.value;
		return *this;
	}

	inline HighPrecFloat& operator-=(const HighPrecFloat& op)
	{	
		value -= op.value;
		return *this;
	}

	inline HighPrecFloat& operator*=(const HighPrecFloat& op)
	{	
		value *= op.value;
		return *this;
	}

	inline HighPrecFloat& operator/=(const HighPrecFloat& op)
	{	
		value /= op.value;
		return *this;
	}

	inline HighPrecFloat operator-() const 
	{	
		return -value;
	}

	inline bool operator==(const HighPrecFloat& other) const 
	{	
		return xeq(value, other.value) != 0;
	}

	inline bool operator!=(const HighPrecFloat& other) const 
	{	
		return xneq(value, other.value) != 0;
	}

	inline bool operator<(const HighPrecFloat& other) const 
	{	
		return xlt(value, other.value) != 0;
	}

	inline bool operator>(const HighPrecFloat& other) const 
	{	
		return xgt(value, other.value) != 0;
	}

	inline bool operator<=(const HighPrecFloat& other) const 
	{	
		return xle(value, other.value) != 0;
	}

	inline bool operator>=(const HighPrecFloat& other) const 
	{	
		return xge(value, other.value) != 0;
	}

	xpr value;
};

//template < class Ty_ >
//struct MathTypeTraits
//{
//	typedef Ty_ value_type;
//	typedef MathTypeTraits<Ty_> this_type;
//
//	static inline value_type add(const value_type& op1, const value_type& op2)
//	{
//		return op1 + op2;
//	}
//	static inline value_type sub(const value_type& op1, const value_type& op2)
//	{
//		return op1 - op2;
//	}
//	static inline value_type mul(const value_type& op1, const value_type& op2)
//	{
//		return op1 * op2;
//	}
//	static inline value_type div(const value_type& op1, const value_type& op2)
//	{
//		return op1 / op2;
//	}
//
//	static inline value_type& add_self(value_type& op1, const value_type& op2)
//	{
//		return op1 += op2;
//	}
//	static inline value_type& sub_self(value_type& op1, const value_type& op2)
//	{
//		return op1 -= op2;
//	}
//	static inline value_type& mul_self(value_type& op1, const value_type& op2)
//	{
//		return op1 *= op2;
//	}
//	static inline value_type& div_self(value_type& op1, const value_type& op2)
//	{
//		return op1 /= op2;
//	}
//
//	static inline value_type neg(const value_type& op1)
//	{
//		return -op1;
//	}
//
//	static inline value_type abs(const value_type& op1)
//	{
//		return ::abs(op1);
//	}
//
//	static inline value_type pow(const value_type& op1, int i)
//	{
//		return ::pow(op1, i);
//	}
//
//	static inline value_type pow(const value_type& op1, const value_type& op2)
//	{
//		return ::pow(op1, op2);
//	}
//
//	static inline bool equal(const value_type& op1, const value_type& op2)
//	{
//		return op1 == op2;
//	}
//
//	static inline bool less_than(const value_type& op1, const value_type& op2)
//	{
//		return op1 < op2;
//	}
//
//	static inline bool greater_than(const value_type& op1, const value_type& op2)
//	{
//		return op1 > op2;
//	}
//
//
//};


}

inline math::HighPrecFloat abs(const math::HighPrecFloat& op)
{
	return xabs(op.value);
}

inline math::HighPrecFloat pow(const math::HighPrecFloat& s, int n)
{
	return xpwr(s.value, n);
}

inline math::HighPrecFloat pow(const math::HighPrecFloat& x, const math::HighPrecFloat& y)
{
	return xpow(x.value, y);
}

inline math::HighPrecFloat ceil(const math::HighPrecFloat& op)
{
	return xceil(op.value);
}

inline math::HighPrecFloat floor(const math::HighPrecFloat& op)
{
	return xfloor(op.value);
}

inline math::HighPrecFloat sqrt(const math::HighPrecFloat& op)
{
	return xsqrt(op.value);
}

inline math::HighPrecFloat exp(const math::HighPrecFloat& op)
{
	return xexp(op.value);
}

inline math::HighPrecFloat log(const math::HighPrecFloat& op)
{
	return xlog(op.value);
}

inline math::HighPrecFloat tan(const math::HighPrecFloat& op)
{
	return xtan(op.value);
}

inline math::HighPrecFloat cos(const math::HighPrecFloat& op)
{
	return xcos(op.value);
}

inline math::HighPrecFloat sin(const math::HighPrecFloat& op)
{
	return xsin(op.value);
}

inline math::HighPrecFloat atan(const math::HighPrecFloat& op)
{
	return xatan(op.value);
}

inline math::HighPrecFloat acos(const math::HighPrecFloat& op)
{
	return xacos(op.value);
}

inline math::HighPrecFloat asin(const math::HighPrecFloat& op)
{
	return xasin(op.value);
}

inline math::HighPrecFloat atan2(const math::HighPrecFloat& x, const math::HighPrecFloat& y)
{
	return xatan2(x.value, y.value);
}

//namespace std {;
//
//template<> class _CRTIMP2_PURE numeric_limits<math::HighPrecFloat>
//: public _Num_float_base
//{	// limits for type math::HighPrecFloat
//public:
//	typedef math::HighPrecFloat _Ty;
//
//	static _Ty (__CRTDECL min)() _THROW0()
//	{	// return minimum value
//		return (xEmin);
//	}
//
//	static _Ty (__CRTDECL max)() _THROW0()
//	{	// return maximum value
//		return (xEmax);
//	}
//
//	static _Ty __CRTDECL epsilon() _THROW0()
//	{	// return smallest effective increment from 1.0
//		return (DBL_EPSILON);
//	}
//
//	static _Ty __CRTDECL round_error() _THROW0()
//	{	// return largest rounding error
//		return (0.5);
//	}
//
//	static _Ty __CRTDECL denorm_min() _THROW0()
//	{	// return minimum denormalized value
//		return (::_Denorm._Double);
//	}
//
//	static _Ty __CRTDECL infinity() _THROW0()
//	{	// return positive infinity
//		return (::_Inf._Double);
//	}
//
//	static _Ty __CRTDECL quiet_NaN() _THROW0()
//	{	// return non-signaling NaN
//		return (::_Nan._Double);
//	}
//
//	static _Ty __CRTDECL signaling_NaN() _THROW0()
//	{	// return signaling NaN
//		return (::_Snan._Double);
//	}
//
//	_STCONS(int, digits, DBL_MANT_DIG);
//	_STCONS(int, digits10, DBL_DIG);
//	_STCONS(int, max_exponent, (int)DBL_MAX_EXP);
//	_STCONS(int, max_exponent10, (int)DBL_MAX_10_EXP);
//	_STCONS(int, min_exponent, (int)DBL_MIN_EXP);
//	_STCONS(int, min_exponent10, (int)DBL_MIN_10_EXP);
//};
//
//}

#endif // __MATHS_HIGHPREC_H__