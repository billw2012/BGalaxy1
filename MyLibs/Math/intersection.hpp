#ifndef _MATH_INTERSECTION_HPP
#define _MATH_INTERSECTION_HPP

#include "Misc/libdesc.hpp"
#include "Misc/hash.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "boundingsphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "frustum.hpp"
#include "rectangle.hpp"

#ifdef min
#undef min
#undef max
#endif

LIB_NAMESPACE_START

namespace math {;

template < class ValueType >
struct IntersectionPair
{
	typedef ValueType value_type;
	typedef IntersectionPair< value_type > this_type;

	typedef Vector3< value_type > VectorType;

	bool occured;
	VectorType point;
	double t;
	ValueType u, v;
	bool front;

	IntersectionPair(bool occured_ = false) : occured(occured_), t(0), u(0), v(0), front(false) {}

	template < class OtherValueType >
	IntersectionPair(bool _occured, 
		const Vector3<OtherValueType>& _point = Vector3<value_type>::Zero, 
		double _t = 0, OtherValueType _u = 0, OtherValueType _v = 0, bool _front = false)
		: occured(_occured), point(_point), t(_t), u(_u), v(_v), front(_front) {}
};

typedef IntersectionPair<float> IntersectionPairf;
typedef IntersectionPair<double> IntersectionPaird;

template < class V, class H > struct Ray;
template < class V, class H > struct Plane;

template < 
	class RayValueType, 
	class PlaneValueType 
>
IntersectionPair< RayValueType > intersects(const Ray< RayValueType >& ray, 
											const Plane< PlaneValueType >& plane)
{
	typedef Vector3< RayValueType > VectorType;

	bool intersect = false;
	RayValueType t = 0;
	VectorType hitPoint = VectorType::Zero;
	RayValueType denominator = plane.normal.dotp(ray.direction);

	// Check if the ray is parallel to the plane
	if (denominator == static_cast<RayValueType>(0))
	{
		// If the line is parallel to the plane it only intersects the plane if it is on the plane.
		intersect = (Sign::sign(plane.distance(ray.origin)) == Sign::ZERO);
		if (intersect)
			hitPoint = ray.origin;
	}
	else
	{
		t = (plane.distanceConstant - plane.normal.dotp(ray.origin)) / denominator;
		hitPoint = ray.origin + ray.direction * t;
		intersect = true;
	}

	return IntersectionPair< RayValueType >(intersect, hitPoint, t);
}

template < 
	class RayValueType, 
	class BSphValueType 
>
IntersectionPair< RayValueType > intersects(const Ray< RayValueType >& ray, const BoundingSphere< BSphValueType >& sphere)
{
	typedef Vector3< RayValueType > VectorType;
	typedef Ray< RayValueType > RayType;
	typedef BoundingSphere< BSphValueType > BoundingSphereType;

	VectorType oc = sphere.center() - ray.origin;

	float l2oc = oc.dotp(oc);
	float sphereR2 = sphere.radius() * sphere.radius();
	if( l2oc < sphereR2 ) // starts inside of the sphere
	{ 
		float rdd = ray.direction.dotp(ray.direction);
		// omit division if ray.d is normalized
		float tca = oc.dotp(ray.direction) / rdd;		
		// division
		float l2hc = (sphereR2 - l2oc) / rdd + tca * tca; 

		float t = tca + sqrt(l2hc);
		return IntersectionPair< RayValueType >(true, ray.origin + ray.direction * t, t); 
	} 
	else 
	{
		float tca = oc.dotp(ray.direction);
		if( tca < 0 ) // points away from the sphere
			return IntersectionPair< RayValueType >(false);//std::numeric_limits<float>::infinity();

		float l2hc = (sphereR2 - l2oc) / ray.direction.dotp(ray.direction) + (tca * tca);	// division
		if( l2hc > 0 )
		{
			float t = tca - sqrt(l2hc);
			return IntersectionPair< RayValueType >(true, ray.origin + ray.direction * t, t); 
		}
		else
			return IntersectionPair< RayValueType >(false);
	}
}

template < 
	class RayValueType, 
	class AABBValueType 
>
IntersectionPair< RayValueType > intersects(const Ray< RayValueType >& ray, const AABB< AABBValueType >& aabb)
{
	typedef Vector3< RayValueType > VectorType;

	RayValueType tmin = std::numeric_limits< RayValueType >::min();
	RayValueType tmax = std::numeric_limits< RayValueType >::max();

	VectorType p = aabb.center() - ray.origin;

	static const VectorType dir[] = 
	{
		VectorType::XAxis, VectorType::YAxis, VectorType::ZAxis
	};

	VectorType hl = aabb.extents()*0.5f;

	for(unsigned int i=0; i<3; i++)
	{
		RayValueType e = p[i];//Vector3F::dotProduct(dir[i], p);
		RayValueType f = ray.direction[i];//Vector3F::dotProduct(dir[i], ray.direction);

		if(std::fabs(f) > static_cast<RayValueType>(0.00001))
		{
			RayValueType oneOverF = static_cast<RayValueType>(1) / f;
			RayValueType t1 = (e + hl[i]) * oneOverF;
			RayValueType t2 = (e - hl[i]) * oneOverF;
			if(t1 > t2) std::swap(t1, t2);
			if(t1 > tmin) tmin = t1;
			if(t2 < tmax) tmax = t2;
			if(tmin > tmax) return IntersectionPair(false);
			if(tmax < static_cast<RayValueType>(0)) return IntersectionPair(false);
		}
		else if( (-e - hl[i]) > static_cast<RayValueType>(0) || 
			(-e + hl[i]) < static_cast<RayValueType>(0)) return IntersectionPair(false);
	}
	if(tmin > static_cast<RayValueType>(0)) 
		return IntersectionPair(true, ray.origin + ray.direction * tmin, tmin);
	return IntersectionPair(true, ray.origin + ray.direction * tmax, tmax);
}

// template < class VectorValueType, class AABBValueType >
// bool intersects(const Vector3< VectorValueType >& a, const Vector3< VectorValueType >& b, const Vector3< VectorValueType >& c, const AABB< AABBValueType >& aabb)

template < 
	class RayValueType, 
	class VectorValueType 
>
IntersectionPair< RayValueType > intersects(const Ray< RayValueType >& ray, 
											const Vector3< VectorValueType >& ta, 
											const Vector3< VectorValueType >& tb, 
											const Vector3< VectorValueType >& tc, 
											bool ignoreBackface = true, bool infiniteRay = false)
{
	typedef Vector3< RayValueType > VectorType;

	VectorType u, v, n;
	VectorType w0, w;
	RayValueType r, a, b;

	u = tb - ta;
	v = tc - ta;
	n = u.cross(v);


	if (n.x == static_cast<RayValueType>(0) 
		&& n.y == static_cast<RayValueType>(0) 
		&& n.z == static_cast<RayValueType>(0))
		return IntersectionPair();   // triangle is degenerate

	b = n.dot(ray.direction);

	bool front = (b <= static_cast<RayValueType>(0));
	if (ignoreBackface && !front)
		return IntersectionPair();   // triangle is degenerate

	w0 = ray.origin - ta;
	a = -n.dot(w0);

	if (std::fabs(b) < static_cast<RayValueType>(0))
		return IntersectionPair();   // line lies in plane or disjoint from it

	r = a / b;
	//float len = ray.direction.getLength();
	if (r < static_cast<RayValueType>(0) || (!infiniteRay && r > 1))
		return IntersectionPair();  // no intersect


	VectorType isect = ray.origin + (ray.direction * r);

	//if(tAtIntersect )
	// is I inside T?
	RayValueType uu, uv, vv, wu, wv, D;
	uu = Vector3F::dotProduct(u, u);
	uv = Vector3F::dotProduct(u, v);
	vv = Vector3F::dotProduct(v, v);
	w = isect - ta;
	wu = Vector3F::dotProduct(w, u);
	wv = Vector3F::dotProduct(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	RayValueType s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < static_cast<RayValueType>(0) || s > static_cast<RayValueType>(1))
		return IntersectionPair();
	t = (uv * wu - uu * wv) / D;
	if (t < static_cast<RayValueType>(0) || (s + t) > static_cast<RayValueType>(1))
		return IntersectionPair();

	//result = isect;
	return IntersectionPair(true, isect, r, s, t, front);
}	

template < 
class VectorValueType, 
class PlaneValueType 
>
IntersectionPair< VectorValueType >
intersects(const Vector3< VectorValueType >& point, const Plane< PlaneValueType >& plane)
{
	typedef Vector3< VectorValueType > VectorType;

	VectorValueType t = (plane.normal.dotp((VectorType::One * plane.distanceConstant) - point));

	return IntersectionPair< VectorValueType >(true, point + plane.normal * t, t);
}


template < 
	class VectorValueType,
	class RectType
>
IntersectionPair< VectorValueType >
intersects(const Vector2< VectorValueType >& point, const Rectangle< RectType >& rect)
{
	if(point.x >= rect.left && point.x <= rect.right && 
		((point.y >= rect.bottom && point.y <= rect.top && rect.bottom <= rect.top) || 
		(point.y >= rect.top && point.y <= rect.bottom && rect.top <= rect.bottom)))
		return IntersectionPair< VectorValueType >(true, Vector3<VectorValueType>());
	return IntersectionPair< VectorValueType >(false, Vector3<VectorValueType>());
}

template < 
	class RectTypeA,
	class RectTypeB
>
IntersectionPair< RectTypeA >
intersects(const Rectangle< RectTypeA >& recta, const Rectangle< RectTypeB >& rectb)
{
	typedef Vector2< RectTypeA > VectorType;
	
	if(math::intersects(VectorType(recta.left, recta.bottom), rectb).occured || math::intersects(VectorType(recta.left, recta.top), rectb).occured || math::intersects(VectorType(recta.right, recta.bottom), rectb).occured || math::intersects(VectorType(recta.right, recta.top), rectb).occured)
		return IntersectionPair< RectTypeA >(true, Vector3<RectTypeA>());
	return IntersectionPair< RectTypeA >(false, Vector3<RectTypeA>());
}

template < class V, class H, class VP, class HP > 
IntersectionPair< V >
	intersects(const BoundingSphere<V,H>& bsph, const Plane<VP,HP>& plane)
{
	if(plane.distance(bsph.center + plane.normal * bsph.radius) < 0)
		return IntersectionPair< V >(true, Vector3<V>());
	return IntersectionPair< V >();
}

// this isn't 100% accurate, it is conservative (some false positives)
template < class V, class H, class V2, class H2 > 
IntersectionPair< V >
	intersects(const BoundingSphere<V,H>& bsph, const AABB<V2,H2>& aabb)
{
	if(bsph.center().x + bsph.radius() < aabb.min().x || 
		bsph.center().y + bsph.radius() < aabb.min().y ||
		bsph.center().z + bsph.radius() < aabb.min().z ||
		bsph.center().x - bsph.radius() > aabb.max().x || 
		bsph.center().y - bsph.radius() > aabb.max().y ||
		bsph.center().z - bsph.radius() > aabb.max().z)
		return IntersectionPair< V >();
	return IntersectionPair< V >(true, Vector3<V>());
}

template < class V, class H, class VP, class HP > 
IntersectionPair< V >
intersects( const BoundingSphere<V,H>& sphere, const Frustum<VP,HP>& frustum )
{
	Vector3<V,H> op = sphere.center() - frustum.eyePos();   
	V f = op.dotp(frustum.forward());
	if (f < V(frustum.nearZ())-V(sphere.radius()) || V(frustum.farZ())+V(sphere.radius()) < f) 
		return IntersectionPair<V>();

	V r = op.dotp(frustum.right());
	V rLimit = static_cast<V>(frustum.rFactor()) * f;
	V rTop = rLimit + static_cast<V>(sphere.radius());
	if (r < -rTop || rTop < r) 
		return IntersectionPair<V>();

	V u = static_cast<V>(op.dotp(frustum.up()));
	V uLimit = static_cast<V>(frustum.uFactor()) * f;
	V uTop = uLimit + static_cast<V>(sphere.radius());
	if (u < -uTop || uTop < u) 
		return IntersectionPair<V>();
	return IntersectionPair<V>(true);
}

}

LIB_NAMESPACE_END

#endif // _MATH_INTERSECTION_HPP