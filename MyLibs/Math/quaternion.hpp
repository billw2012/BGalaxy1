#if !defined(__MATH_QUATERNION_HPP__)
#define __MATH_QUATERNION_HPP__

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"

#include "matrix4.hpp"

namespace math
{

template < class ValueType, class HashType = hash::DefaultHashType >
struct Quaternion
{
	typedef ValueType value_type;
	typedef HashType hash_type;

	typedef Quaternion < value_type, hash_type > this_type;

	float x, y, z, w;

	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(value_type xs, value_type ys, value_type zs, value_type ws) : x(xs), y(ys), z(zs), w(ws) {}

	Quaternion(const Matrix4< value_type >& from) 
	{
		from_matrix(from);
	}

	const this_type& operator*=(float scale)			
	{ 
		x *= scale; 
		y *= scale; 
		z *= scale; 
		w *= scale; 
		return *this; 
	}

	const this_type operator*(float scale) const
	{
		this_type cpy(*this);
		cpy *= scale;
		return cpy;
	}

	const this_type& operator/=(float scale)			
	{ 
		x /= scale; 
		y /= scale; 
		z /= scale; 
		w /= scale; 
		return *this; 
	}

	const this_type operator/(float scale) const	
	{
		this_type cpy(*this);
		cpy /= scale;
		return cpy;
	}

	const this_type& operator+=(float val)			
	{ 
		x += val; 
		y += val; 
		z += val; 
		w += val; 
		return *this; 
	}

	const this_type operator+(float val) const
	{
		this_type cpy(*this);
		cpy += val;
		return cpy;
	}

	const this_type& operator+=(const this_type& val)			
	{ 
		x += val.x; 
		y += val.y; 
		z += val.z; 
		w += val.w; 
		return *this; 
	}

	const this_type operator+(const this_type& val) const
	{
		this_type cpy(*this);
		cpy += val;
		return cpy;
	}

	const this_type operator -() const
	{ 
		return this_type(-x, -y, -z, -w); 
	}

	//! gets the length of this quaternion
	value_type length() const
	{ 
		return static_cast<value_type>(::sqrt(x*x + y*y + z*z + w*w)); 
	}

	//! gets the squared length of this quaternion
	double length_squared() const
	{ 
		return x*x + y*y + z*z + w*w; 
	}

	//! normalizes this quaternion
	void normalize()
	{ 
		*this /= length(); 
	}

	//! returns the normalized version of this quaternion
	this_type normal() const
	{ 
		return *this / length(); 
	}

	value_type dotp(const this_type &q2) const
	{ 
		return x*q2.x + y*q2.y + z*q2.z + w*q2.w; 
	}

	//! linear quaternion interpolation
	this_type lerp(const this_type& q2, float t) const 
	{ 
		return (*this * (1 - t) + q2 * t).normal(); 
	}

	//! spherical linear interpolation
	this_type slerp(const this_type& q2, float t) const 
	{
		this_type q3;
		value_type dot = this->dotp(q2);

		/*	dot = cos(theta)
			if (dot < 0), q1 and q2 are more than 90 degrees apart,
			so we can invert one to reduce spinning	*/
		if(dot < 0)
		{
			dot = -dot;
			q3 = -q2;
		} 
		else q3 = q2;
		
		if(dot < 0.95)
		{
			float angle = std::acos(dot);
			return (*this * std::sin(angle * (1 - t)) + q3 * std::sin(angle * t)) / std::sin(angle);
		} 
		// if the angle is small, use linear interpolation								
		return this->lerp(q3,t);		
	}

	Matrix4< value_type > to_matrix() const
	{
		value_type sqw = w * w;
		value_type sqx = x * x;
		value_type sqy = y * y;
		value_type sqz = z * z;

		Matrix4<value_type> m;
		// invs (inverse square length) is only required if quaternion is not already normalised
		double invs = 1 / (sqx + sqy + sqz + sqw);
		m.m11 = static_cast<float>(( sqx - sqy - sqz + sqw) * invs); // since sqw + sqx + sqy + sqz =1/invs*invs
		m.m22 = static_cast<float>((-sqx + sqy - sqz + sqw) * invs);
		m.m33 = static_cast<float>((-sqx - sqy + sqz + sqw) * invs);

		double tmp1 = x * y;
		double tmp2 = z * w;
		m.m21 = static_cast<float>(2 * (tmp1 + tmp2) * invs);
		m.m12 = static_cast<float>(2 * (tmp1 - tmp2) * invs);

		tmp1 = x * z;
		tmp2 = y * w;
		m.m31 = static_cast<float>(2 * (tmp1 - tmp2) * invs);
		m.m13 = static_cast<float>(2 * (tmp1 + tmp2) * invs);
		tmp1 = y * z;
		tmp2 = x * w;
		m.m32 = static_cast<float>(2 * (tmp1 + tmp2) * invs);
		m.m23 = static_cast<float>(2 * (tmp1 - tmp2) * invs);   

		return m;
	}

	void from_matrix(const Matrix4< value_type >& t)
	{
		//V m11, m22, m33;
		// have to remove scaling from the matrix
		value_type trace = t.m11 + t.m22 + t.m33; // I removed + 1.0f; see discussion with Ethan
		if(trace > 0) 
		{
			// I changed M_EPSILON to 0
			value_type s = 0.5 / std::sqrt(trace + 1.0);
			w = 0.25 / s;
			x = ( t.m32 - t.m23 ) * s;
			y = ( t.m13 - t.m31 ) * s;
			z = ( t.m21 - t.m12 ) * s;
		}
		else 
		{
			if(t.m11 > t.m22 && t.m11 > t.m33) 
			{
				value_type s = 2.0 * std::sqrt(1.0 + t.m11 - t.m22 - t.m33);
				w = (t.m32 - t.m23 ) / s;
				x = 0.25 * s;
				y = (t.m12 + t.m21 ) / s;
				z = (t.m13 + t.m31 ) / s;
			} 
			else if(t.m22 > t.m33) 
			{
				value_type s = 2.0 * std::sqrt(1.0 + t.m22 - t.m11 - t.m33);
				w = (t.m13 - t.m31 ) / s;
				x = (t.m12 + t.m21 ) / s;
				y = 0.25 * s;
				z = (t.m23 + t.m32 ) / s;
			} 
			else 
			{
				value_type s = 2.0 * std::sqrt(1.0 + t.m33 - t.m11 - t.m22);
				w = (t.m21 - t.m12 ) / s;
				x = (t.m13 + t.m31 ) / s;
				y = (t.m23 + t.m32 ) / s;
				z = 0.25 * s;
			}
		}
	}

private:
};

typedef Quaternion< float > Quaternionf;
typedef Quaternion< double > Quaterniond;

}
#endif // __MATH_QUATERNION_HPP__