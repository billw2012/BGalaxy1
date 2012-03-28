#ifndef _AABB_HPP
#define _AABB_HPP

#include <vector>
#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"
#include "vector3.hpp"
#include "misc.hpp"

#undef max
#undef min

LIB_NAMESPACE_START

namespace math
{

template < class ValueType, class HashType = hash::DefaultHashType >
struct AABB 
{
	typedef AABB<ValueType,HashType> this_type;
	typedef HashType hash_type;
	typedef ValueType value_type;

	typedef Vector3<value_type, hash_type> VectorType;

private:
	VectorType _min, _max;

public:

	AABB() : _min(VectorType::Max), _max(VectorType::Min) {}

	template < class V, class H >
	AABB(const Vector3<V,H>& min, const Vector3<V,H>& max) : _min(min), _max(max) { }

	// Create box from min and max vectors
	template < class V, class H >
	void create(const Vector3<V,H>& min, const Vector3<V,H>& max)
	{
		_min = min;
		_max = max;
	}

	void reset()
	{
		_min = VectorType::Max;
		_max = VectorType::Min;
	}

	// Expand the box to contain vec if required
	template < class V, class H >
	void expand(const Vector3<V,H>& vec)
	{
		if(vec.x < _min.x)	_min.x = vec.x;
		if(vec.x >  _max.x)	_max.x = vec.x;
		if(vec.y < _min.y)	_min.y = vec.y;
		if(vec.y >  _max.y)	_max.y = vec.y;
		if(vec.z < _min.z)	_min.z = vec.z;
		if(vec.z >  _max.z)	_max.z = vec.z;
	}

	// Expand the box to contain vec if required
	template < class V, class H >
	void expand(const AABB<V,H>& aabb)
	{
		expand(aabb.min());
		expand(aabb.max());
	}

	const VectorType& min() const
	{
		return _min;
	}

	const VectorType& max() const
	{
		return _max;
	}

	VectorType center() const
	{
		return (_min + _max) * static_cast<value_type>(0.5);
	}

	VectorType extents() const
	{
		return _max - _min;
	}

	template < class V, class H > 
	bool contains(const Vector3<V,H>& vec) const
	{
		return vec.x >= _min.x && vec.x <= _max.x &&
			vec.y >= _min.y && vec.y <= _max.y &&
			vec.z >= _min.z && vec.z <= _max.z;
	}

	template < class V > 
	bool contains(V x, V y, V z) const
	{
		return x >= _min.x && x <= _max.x &&
			y >= _min.y && y <= _max.y &&
			z >= _min.z && z <= _max.z;
	}

	template < class V > 
	bool contains_conservative(V x, V y, V z) const
	{
		return x >= _min.x && x < _max.x &&
			y >= _min.y && y < _max.y &&
			z >= _min.z && z < _max.z;
	}

	template < class V, class H > 
	bool contains(const AABB<V,H>& aabb) const
	{
		return contains(aabb._min) && contains(aabb._max);
	}

	template < class V, class H >
	void getVertices(std::vector< Vector3<V,H> > &vertices) const
	{
		vertices.resize(8);
		vertices[0] = _min;
		vertices[1] = Vector3<V,H>(_min.x, _min.y, _max.z);
		vertices[2] = Vector3<V,H>(_min.x, _max.y, _max.z);
		vertices[3] = Vector3<V,H>(_min.x, _max.y, _min.z);
		vertices[4] = Vector3<V,H>(_max.x, _min.y, _min.z);
		vertices[5] = Vector3<V,H>(_max.x, _min.y, _max.z);
		vertices[6] = _max;
		vertices[7] = Vector3<V,H>(_max.x, _max.y, _min.z);
	}

	template < class V, class H >
	Vector3<V,H> closest_point(const Vector3<V,H>& vert) const
	{
		Vector3<V,H> closePt;

		for(size_t idx = 0; idx < 3; ++idx)
			closePt(idx) = clamp(vert[idx], _min[idx], _max[idx]);

		return closePt;
	}

};

typedef AABB<float> AABBf;
typedef AABB<double> AABBd;
}

LIB_NAMESPACE_END

#endif // _AABB_HPP
