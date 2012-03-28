#ifndef _TRANSFORMATION_HPP
#define _TRANSFORMATION_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"
#include "constants.hpp"

LIB_NAMESPACE_START

#if !defined(M_PI)
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401
#endif

namespace math {;

template < class ValueType, class HashType >
struct Matrix4;
template < class ValueType, class HashType >
struct Vector4;
template < class ValueType, class HashType = hash::DefaultHashType >
struct Vector3;

template < class ValueType, class HashType >
struct Quaternion;

struct EulerOrder
{
	enum type
	{ 
		XYZ, ZXY, ZYX, YZX, XZY, YXZ 
	};

	type reverse_order(type order) 
	{
		switch(order) {
		case XYZ: return ZYX;
		case ZXY: return YXZ;
		case ZYX: return XYZ;
		case YZX: return XZY;
		case XZY: return YZX;
		case YXZ: default: return ZXY;
		};
	}
};

template < class V >
V deg_to_rad(V deg)
{
	return (deg / static_cast<V>(180)) * static_cast<V>(constants::pi);
}

template < class V >
V rad_to_deg(V rad)
{
	return (rad / static_cast<V>(constants::pi)) * static_cast<V>(180);
}

template < class V >
Matrix4<V> rotatex(V angleDegrees)
{
	V s = static_cast<V>(sin(deg_to_rad(angleDegrees)));
	V c = static_cast<V>(cos(deg_to_rad(angleDegrees)));

	return Matrix4<V> (
		static_cast<V>(1),	static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(0),
		static_cast<V>(0),	c,	-s, static_cast<V>(0),
		static_cast<V>(0),	s,	c,  static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1));
}

template < class V >
Matrix4<V> rotatey(V angleDegrees)
{
	V s = static_cast<V>(sin(deg_to_rad(angleDegrees)));
	V c = static_cast<V>(cos(deg_to_rad(angleDegrees)));

	return Matrix4<V>(
		c,	static_cast<V>(0),	s,	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(1),  static_cast<V>(0),	static_cast<V>(0),
		-s,	static_cast<V>(0),  c,	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1));
}

template < class V >
Matrix4<V> rotatez(V angleDegrees)
{
	V s = static_cast<V>(sin(deg_to_rad(angleDegrees)));
	V c = static_cast<V>(cos(deg_to_rad(angleDegrees)));

	return Matrix4<V>(
		c,	-s,	static_cast<V>(0),	static_cast<V>(0),
		s,	c,	static_cast<V>(0),	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1),	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1));
}

template < class V >
Matrix4<V> rotatez_rad(V angleRadians)
{
	V s = static_cast<V>(sin(angleRadians));
	V c = static_cast<V>(cos(angleRadians));

	return Matrix4<V>(
		c,	-s,	static_cast<V>(0),	static_cast<V>(0),
		s,	c,	static_cast<V>(0),	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1),	static_cast<V>(0),
		static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(0),	static_cast<V>(1));
}


template < class V >
Matrix4<V> rotate_euler(V x, V y, V z, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	switch(rotOrder) {
case EulerOrder::XYZ:
	return (rotatex(x) * rotatey(y)) * rotatez(z);
case EulerOrder::XZY:
	return (rotatex(x) * rotatez(z)) * rotatey(y);
case EulerOrder::YXZ:
	return (rotatey(y) * rotatex(x)) * rotatez(z);
case EulerOrder::YZX:
	return (rotatey(y) * rotatez(z)) * rotatex(x);
case EulerOrder::ZXY:
	return (rotatez(z) * rotatex(x)) * rotatey(y);
case EulerOrder::ZYX:
default:
	return (rotatez(z) * rotatey(y)) * rotatex(x);
	};
}

template < class V, class H >
Matrix4<V,H> rotate_euler(const Vector3<V,H>& rotVec, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	switch(rotOrder) {
	case EulerOrder::XYZ:
		return (rotatex(rotVec.x) * rotatey(rotVec.y)) * rotatez(rotVec.z);
	case EulerOrder::XZY:
		return (rotatex(rotVec.x) * rotatez(rotVec.z)) * rotatey(rotVec.y);
	case EulerOrder::YXZ:
		return (rotatey(rotVec.y) * rotatex(rotVec.x)) * rotatez(rotVec.z);
	case EulerOrder::YZX:
		return (rotatey(rotVec.y) * rotatez(rotVec.z)) * rotatex(rotVec.x);
	case EulerOrder::ZXY:
		return (rotatez(rotVec.z) * rotatex(rotVec.x)) * rotatey(rotVec.y);
	case EulerOrder::ZYX:
	default:
		return (rotatez(rotVec.z) * rotatey(rotVec.y)) * rotatex(rotVec.x);
	};
}

template < class V >
Matrix4<V> rotate_axis_angle(V ax, V ay, V az, V angle)
{
	V c = static_cast<V>(cos(deg_to_rad(angle)));
	V s = static_cast<V>(sin(deg_to_rad(angle)));
	V t = 1 - c;

	return Matrix4<V>(
		static_cast<V>(1 + t * (ax * ax - 1)), static_cast<V>(-az * s + t * ax * ay), 
		static_cast<V>(ay * s + t * ax * az), static_cast<V>(0),
		static_cast<V>(az * s + t * ax * ay), static_cast<V>(1 + t * (ay * ay - 1)), 
		static_cast<V>(-ax * s + t * ay * az), static_cast<V>(0),
		static_cast<V>(-ay * s + t * ax * az), static_cast<V>(ax * s + t * ay * az), 
		static_cast<V>(1 + t * (az * az - 1)), static_cast<V>(0),
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V, class H >
Matrix4<V,H> rotate_axis_angle(const Vector3<V,H>& axis, V angle)
{
	V c = static_cast<V>(cos(deg_to_rad(angle)));
	V s = static_cast<V>(sin(deg_to_rad(angle)));
	V t = static_cast<V>(1.0) - c;

	return Matrix4<V>(
		static_cast<V>(V(1.0) + t * (axis.x * axis.x - V(1.0))), static_cast<V>(-axis.z * s + t * axis.x * axis.y), 
		static_cast<V>(axis.y * s + t * axis.x * axis.z), static_cast<V>(0),
		static_cast<V>(axis.z * s + t * axis.x * axis.y), static_cast<V>(V(1.0) + t * (axis.y * axis.y - V(1.0))), 
		static_cast<V>(-axis.x * s + t * axis.y * axis.z), static_cast<V>(0),
		static_cast<V>(-axis.y * s + t * axis.x * axis.z), static_cast<V>(axis.x * s + t * axis.y * axis.z), 
		static_cast<V>(V(1.0) + t * (axis.z * axis.z - V(1.0))), static_cast<V>(0),
		static_cast<V>(V(0.0)), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V >
Matrix3<V> rotate_2D_rad(V angle)
{
	V c = static_cast<V>(::cos(angle));
	V s = static_cast<V>(::sin(angle));
	return Matrix3<V>(
		c,		s,		V(0), 
		-s,		c,		V(0),
		V(0),	V(0),	V(1));
}

template < class V >
Matrix4<V> translate(V x, V y, V z)
{
	return Matrix4<V>(
		static_cast<V>(1), static_cast<V>(0), static_cast<V>(0), x,
		static_cast<V>(0), static_cast<V>(1), static_cast<V>(0), y,
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(1), z,
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V, class H >
Matrix4<V,H> translate(const Vector3<V,H>& t)
{
	return Matrix4<V,H>(
		static_cast<V>(1), static_cast<V>(0), static_cast<V>(0), t.x,
		static_cast<V>(0), static_cast<V>(1), static_cast<V>(0), t.y,
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(1), t.z,
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V, class H >
Vector3<V,H> get_translate(const Matrix4<V,H>& t)
{
	return Vector3<V,H>(t.getColumnVector(3)); 
}

template < class V, class H >
Vector3<V,H> get_scale(const Matrix4<V,H>& t)
{
	return Vector3<V,H>(t.getColumnVector(0).length(), t.getColumnVector(1).length(), t.getColumnVector(2).length());
}

//template < class V, class H >
//Quaternion<V,H> get_quaternion(const Matrix4<V,H>& t)
//{
//	Quaternion<V,H> q;
//	
//	V m11, m22, m33;
//	// have to remove scaling from the matrix
//	V trace = m11 + m22 + m33; // I removed + 1.0f; see discussion with Ethan
//	if(trace > 0) 
//	{
//		// I changed M_EPSILON to 0
//		V s = 0.5f / sqrtf(trace + 1.0f);
//		q.w = 0.25f / s;
//		q.x = ( t.m32 - t.m23 ) * s;
//		q.y = ( t.m13 - t.m31 ) * s;
//		q.z = ( t.m21 - t.m12 ) * s;
//	}
//	else 
//	{
//		if(m11 > m22 && m11 > m33) 
//		{
//			V s = 2.0f * sqrtf( 1.0f + m11 - m22 - m33);
//			q.w = (t.m32 - t.m23 ) / s;
//			q.x = 0.25f * s;
//			q.y = (t.m12 + t.m21 ) / s;
//			q.z = (t.m13 + t.m31 ) / s;
//		} 
//		else if(m22 > m33) 
//		{
//			V s = 2.0f * sqrtf( 1.0f + m22 - m11 - m33);
//			q.w = (t.m13 - t.m31 ) / s;
//			q.x = (t.m12 + t.m21 ) / s;
//			q.y = 0.25f * s;
//			q.z = (t.m23 + t.m32 ) / s;
//		} 
//		else 
//		{
//			V s = 2.0f * sqrtf( 1.0f + m33 - m11 - m22 );
//			q.w = (t.m21 - t.m12 ) / s;
//			q.x = (t.m13 + t.m31 ) / s;
//			q.y = (t.m23 + t.m32 ) / s;
//			q.z = 0.25f * s;
//		}
//	}
//	return q;
//}

template < class V, class H >
void split_transform(const Matrix4<V,H>& matrix, Quaternion<V,H>& rot, Vector3<V,H>& trans, Vector3<V,H>& sc)
{
	sc = get_scale(matrix);
	Matrix4<V,H> unscaledMat(matrix * scale(1 / sc.x, 1 / sc.y, 1 / sc.z));
	rot.from_matrix(unscaledMat);
	trans = get_translate(matrix);
}

// template < class V, class H >
// Vector3<V,H> get_rotate(const Matrix4<V,H>& t)
// {
// 	return t.getColumnVector(3); 
// }

template < class V >
Matrix4<V> scale(V x, V y, V z)
{
	return Matrix4<V>(
		x, static_cast<V>(0), static_cast<V>(0), static_cast<V>(0),
		static_cast<V>(0), y, static_cast<V>(0), static_cast<V>(0),
		static_cast<V>(0), static_cast<V>(0), z, static_cast<V>(0),
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V, class H >
Matrix4<V,H> scale(const Vector3<V,H>& t)
{
	return Matrix4<V,H>(
		t.x, static_cast<V>(0), static_cast<V>(0), static_cast<V>(0),
		static_cast<V>(0), t.y, static_cast<V>(0), static_cast<V>(0),
		static_cast<V>(0), static_cast<V>(0), t.z, static_cast<V>(0),
		static_cast<V>(0), static_cast<V>(0), static_cast<V>(0), static_cast<V>(1));
}

template < class V, class H >
Matrix4<V,H> transform(const Matrix4<V,H>& rotMat, const Matrix4<V,H>& transMat, const Matrix4<V,H>& scaleMat)
{
	return (transMat * rotMat) * scaleMat;
}

template < class V, class H >
Matrix4<V,H> transform(const Matrix4<V,H>& rotMat, const Matrix4<V,H>& transMat)
{
	return transMat * rotMat;
}

template < class V, class H >
Matrix4<V,H> transform_reverse(const Matrix4<V,H>& rotMat, const Matrix4<V,H>& transMat, const Matrix4<V,H>& scaleMat)
{
	return transform(rotMat, transMat, scaleMat).inverse();
}

template < class V, class H >
Matrix4<V,H> transform_reverse(const Matrix4<V,H>& rotMat, const Matrix4<V,H>& transMat)
{
	return transform(rotMat, transMat).inverse();
}

template < class V, class H >
Matrix4<V,H> transform(const Vector3<V,H>& rotVec, const Vector3<V,H>& transVec, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	return transform(rotate_euler(rotVec, rotOrder), translate(transVec));
}

template < class V, class H >
Matrix4<V,H> transform(const Vector3<V,H>& rotVec, const Vector3<V,H>& transVec, const Vector3<V,H>& scaleVec, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	return transform(rotate_euler(rotVec, rotOrder), translate(transVec), scale(scaleVec));
}

template < class V, class H >
math::Matrix4<V,H> transform(const Quaternion<V,H>& rot, const Vector3<V,H>& trans, const Vector3<V,H>& sc)
{
	return transform(rot.to_matrix(), translate(trans), scale(sc));
}

template < class V, class H >
Matrix4<V,H> transform_reverse(const Vector3<V,H>& rotVec, const Vector3<V,H>& transVec, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	return rotate_euler(rotVec * -1, EulerOrder::reverse_order(rotOrder))*translation(transVec * -1);
}

template < class V, class H >
Matrix4<V,H> transform_reverse(const Vector3<V,H>& rotVec, const Vector3<V,H>& transVec, const Vector3<V,H>& scaleVec, EulerOrder::type rotOrder = EulerOrder::XYZ)
{
	return (scale(Vector3<V,H>(1 / scaleVec.x, 1 / scaleVec.y, 1 / scaleVec.z))*
		rotate_euler(rotVec * -1, EulerOrder::reverse_order(rotOrder)))*translation(transVec * -1);
}

template < class V, class H >
Matrix4<V,H> look_at(const Vector3<V,H>& eyePosition3D, const Vector3<V,H>& center3D, Vector3<V,H> upVector3D )
{
	//------------------
	Vector3<V,H> forward = (center3D - eyePosition3D).normal();
	if(forward == upVector3D)
	{
		upVector3D = Vector3<V,H>(forward.z, forward.y, forward.x).normal();
	}
	//------------------
	//Side = forward x up
	Vector3<V,H> side = upVector3D.crossp(forward).normal();
	//------------------
	//Recompute up as: up = side x forward
	Vector3<V,H> up = forward.crossp(side).normal();
	//------------------
	Matrix4<V,H> matrix2;
	matrix2.setColumnVector(0, Vector4<V,H>(side, 0));
	matrix2.setColumnVector(1, Vector4<V,H>(up, 0));
	matrix2.setColumnVector(2, Vector4<V,H>(forward, 0));
	matrix2.setColumnVector(3, Vector4<V,H>(eyePosition3D, 1));
	return matrix2;
	//matrix2[0] = side[0];
	//matrix2[4] = side[1];
	//matrix2[8] = side[2];
	//matrix2[12] = 0.0;
	////------------------
	//matrix2[1] = up[0];
	//matrix2[5] = up[1];
	//matrix2[9] = up[2];
	//matrix2[13] = 0.0;
	////------------------
	//matrix2[2] = -forward[0];
	//matrix2[6] = -forward[1];
	//matrix2[10] = -forward[2];
	//matrix2[14] = 0.0;
	////------------------
	//matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
	//matrix2[15] = 1.0;
	////------------------
	//return matrix2 * translate(-eyePosition3D);
}

template < class V >
Matrix4<V> frustum(V left, V right, V bottom, V top, V znear, V zfar)
{
	V temp = V(2.0) * znear, 
		temp2 = right - left, 
		temp3 = top - bottom, 
		temp4 = zfar - znear;
	return Matrix4<V>(
		temp/temp2,			V(0.0),				(right+left)/temp2,		V(0.0),
		V(0.0),				temp/temp3,			(top+bottom)/temp3,		V(0.0),
		V(0.0),				V(0.0),				(-zfar-znear)/temp4,	V(-temp*zfar)/temp4, 
		V(0.0),				V(0.0),				V(-1.0),					V(0.0));
	//MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);
	//memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

template < class V >
Matrix4<V> perspective(V fovyInDegrees, V aspectRatio, V znear, V zfar)
{
	V ymax, xmax;
	ymax = znear * ::tan(fovyInDegrees*V(M_PI/360.0));
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax = ymax * aspectRatio;
	return frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

template < class V >
Matrix4<V> ortho(V left, V right, V bottom, V top, V znear, V zfar)
{
	V temp2=right-left, temp3=top-bottom, temp4=zfar-znear;
	return Matrix4<V>(
		V(2.0)/temp2,	V(0.0),			V(0.0),			(-right-left)/temp2,
		V(0.0),			V(2.0)/temp3,	V(0.0),			(-top-bottom)/temp3,
		V(0.0),			V(0.0),			V(-2.0)/temp4,	(-zfar-znear)/temp4, 
		V(0.0),			V(0.0),			V(0.0),			V(1.0));
}

}

LIB_NAMESPACE_END

#endif // _TRANSFORMATION_HPP