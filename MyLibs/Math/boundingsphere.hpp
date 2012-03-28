#ifndef _MATH_BOUNDING_SPHERE_HPP
#define _MATH_BOUNDING_SPHERE_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"
#include "Math/vector3.hpp"
#include "Math/aabb.hpp"

LIB_NAMESPACE_START

namespace math
{

template < class ValueType, class HashType = hash::DefaultHashType >
struct BoundingSphere 
{
	typedef BoundingSphere<ValueType,HashType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;

	typedef Vector3<value_type, hash_type> VectorType;

private:
	VectorType _center;
	value_type _radius;

public:

	BoundingSphere() : _center(), _radius(0) {}

	BoundingSphere(const this_type& other) : _center(other.center()), _radius(other.radius()) {}
	
	template < class V, class H >
	explicit BoundingSphere(const BoundingSphere<V,H>& other) : _center(other.center()), _radius(other.radius()) {}

	BoundingSphere(const VectorType& center, value_type radius) : _center(center), _radius(radius) {}
	template < class V, class H >
	BoundingSphere(const Vector3<V,H>& center, Vector3<V,H> radius) : _center(center), _radius(radius) {}

	//template < class VTy_, class HTy_, class RTy_ >
	//BoundingSphere(const VectorType& center_, value_type radius_) : _center(center_), _radius(radius_) {}

	BoundingSphere(const AABB<value_type,hash_type>& aabb) : _center(aabb.center()), _radius(aabb.extents().length() * 0.5) {}

	// Create box from min and max vectors
	template < class V, class H, class FTy >
	void create(const Vector3<V,H>& center, FTy radius)
	{
		_center = center;
		_radius = radius;
	}

	// Create box from min and max vectors
	template < class V, class H >
	void create(const AABB<V,H>& aabb)
	{
		_center = aabb.center();
		_radius = aabb.extents().length()/* * 0.5*/;
	}


	const VectorType& center() const
	{
		return _center;
	}

	value_type radius() const
	{
		return _radius;
	}

	template < class V, class H > 
	bool contains(const Vector3<V,H>& vec) const
	{
		value_type dist = (vec - _center).lengthSquared();
		return dist <= (_radius * _radius);
	}
};

typedef BoundingSphere<float> BoundingSpheref;
typedef BoundingSphere<double> BoundingSphered;

}

LIB_NAMESPACE_END

#endif // _MATH_BOUNDING_SPHERE_HPP
