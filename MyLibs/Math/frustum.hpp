#ifndef _MATH_FRUSTUM_HPP
#define _MATH_FRUSTUM_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"
#include "vector3.hpp"

LIB_NAMESPACE_START

namespace math
{

template < class ValueType, class HashType = hash::DefaultHashType >
struct Frustum 
{
	typedef Frustum<ValueType,HashType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;

	typedef Vector3<value_type, hash_type> VectorType;

private:
	VectorType _forward;
	VectorType _right;
	VectorType _up;
	VectorType _eyePos;
	value_type _rFactor;
	value_type _uFactor;
	value_type _nearZ;
	value_type _farZ;

public:

	Frustum() : _forward(), _right(), _up(), _eyePos(), _rFactor(0.0), _uFactor(0.0), _nearZ(0.0), _farZ(0.0) {}

// 	template < class V, class H, class FTy >
// 	Frustum(const Vector3<V,H>& forward, const Vector3<V,H>& right, const Vector3<V,H>& up, 
// 		const Vector3<V,H>& eyePos, FTy fov, FTy viewAspect, FTy nearZ, FTy farZ) 
// 		: _forward(forward), _right(right), _up(up), 
// 		_eyePos(eyePos), _uFactor(tanf(deg_to_rad(fov)*0.55f)), _nearZ(nearZ), _farZ(farZ)
// 	{
// 		_rFactor = _uFactor * viewAspect;
// 	}

	Frustum(const VectorType& forward, const VectorType& right, const VectorType& up, 
		const VectorType& eyePos, value_type fovv, value_type fovh, /*value_type viewAspect, */value_type nearZ, value_type farZ) 
		: _forward(forward), _right(right), _up(up), _eyePos(eyePos), 
		_uFactor(static_cast<value_type>(tan(deg_to_rad(fovv * value_type(0.5))))), 
		_rFactor(static_cast<value_type>(tan(deg_to_rad(fovh * value_type(0.5))))), 
		_nearZ(nearZ), _farZ(farZ)
	{
		
		//_rFactor = _uFactor * viewAspect;
	}

	const VectorType& forward() const { return _forward; }
	const VectorType& right() const { return _right; }
	const VectorType& up() const { return _up; }
	const VectorType& eyePos() const { return _eyePos; }
	value_type rFactor() const { return _rFactor; }
	value_type uFactor() const { return _uFactor; }
	value_type nearZ() const { return _nearZ; }
	value_type farZ() const { return _farZ; }
};

typedef Frustum<float> Frustumf;
}

LIB_NAMESPACE_END

#endif // _MATH_FRUSTUM_HPP
