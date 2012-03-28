#ifndef _SIMD_TYPES_HPP
#define _SIMD_TYPES_HPP

#include <vector>

namespace misc
{

template < class Type, int _Alignment = 16 >
struct AlignedAllocator
{
	template < class Other >
	struct rebind 
	{
		typedef AlignedAllocator<Other, _Alignment> other;
	};

	typedef std::size_t size_type;
	typedef Type value_type;

	typedef Type* pointer;
	typedef const Type* const_pointer;
	typedef Type& reference;
	typedef const Type& const_reference;

	typedef std::ptrdiff_t difference_type;

	pointer address(reference obj) const					{ return &obj; }
	const_pointer address(const_reference obj) const		{ return &obj; }

   /* constructors and destructor
    * - nothing to do because the allocator has no state
    */
   AlignedAllocator() throw() {}
   AlignedAllocator(const AlignedAllocator&) throw() {}
   template <class U, int A> 
   AlignedAllocator (const AlignedAllocator<U, A>&) throw() {}

   ~AlignedAllocator() throw() {}

	pointer allocate(size_type _Count, const void* = 0)
	{
		return static_cast<pointer>(_aligned_malloc(sizeof(value_type) * _Count, _Alignment));
	}

	//pointer allocate(size_type _Count)
	//{
	//	return static_cast<pointer>(_aligned_malloc(sizeof(value_type) * _Count, _Alignment));
	//}

	void deallocate(pointer _Ptr, size_type)
	{
		_aligned_free(static_cast<void *>(_Ptr));
	}

	void construct(pointer _Ptr, const Type& _Val)
	{
		::new (static_cast<void *>(_Ptr)) Type( _Val );
	}

	void destroy(pointer _Ptr)
	{
		(_Ptr)->~Type();
	}
	
	size_type max_size() const
	{
		size_type _Count = (size_type)(-1) / sizeof(Type);
		return (0 < _Count ? _Count : 1);
	}
};

// allocator TEMPLATE OPERATORS
template < class _Ty, class _Other > inline
bool operator==(const AlignedAllocator<_Ty>&, const AlignedAllocator<_Other>&) _THROW0()
{	// test for allocator equality (always true)
	return (true);
}

template < class _Ty, class _Other > inline
bool operator!=(const AlignedAllocator<_Ty>&, const AlignedAllocator<_Other>&) _THROW0()
{	// test for allocator inequality (always false)
	return (false);
}

typedef float simd_type;
typedef float simd_typed;
typedef std::vector< simd_type, AlignedAllocator< simd_type > > SIMDArray;
typedef std::vector< simd_typed, AlignedAllocator< simd_typed > > SIMDArrayd;

template < class FTy_ >
struct SIMDSoAVector3
{
private:
	SIMDArray		_x;
	SIMDArray		_y;
	SIMDArray		_z;

public:
	SIMDSoAVector3	(size_t esize, simd_type val)	: _x(esize, val), _y(esize, val), _z(esize, val) {}
	SIMDSoAVector3	(size_t esize)					: _x(esize), _y(esize), _z(esize) {}
	SIMDSoAVector3	()								: _x(), _y(), _z() {}

	void resize		(size_t size)
	{
		_x.resize(size);
		_y.resize(size);
		_z.resize(size);
	}

	void resize		(size_t size, simd_type val)
	{
		_x.resize(size, val);
		_y.resize(size, val);
		_z.resize(size, val);
	}

	void reserve	(size_t size)
	{
		_x.reserve(size);
		_y.reserve(size);
		_z.reserve(size);
	}

	void clear		()								{ _x.clear(); _y.clear(); _z.clear(); }

	void clear_mem	()
	{
		clear();
		SIMDArray tx, ty, tz;
		_x.swap(tx);
		_y.swap(ty);
		_z.swap(tz);
	}

	float& x		(size_t index)					{ return _x[index]; }
	const float& x	(size_t index) const			{ return _x[index]; }
	float& y		(size_t index)					{ return _y[index]; }
	const float& y	(size_t index) const			{ return _y[index]; }
	float& z		(size_t index)					{ return _z[index]; }
	const float& z	(size_t index) const			{ return _z[index]; }

	void push_back  (simd_type x, simd_type y, simd_type z)
	{
		_x.push_back(x); _y.push_back(y); _z.push_back(z);
	}

	void push_back  (const math::Vector3<simd_type>& val)
	{
		_x.push_back(val.x); _y.push_back(val.y); _z.push_back(val.z);
	}

	void set		(size_t index, const math::Vector3<simd_type>& val)
	{
		_x[index] = val.x;
		_y[index] = val.y;
		_z[index] = val.z;
	}

	const math::Vector3<simd_type> get(size_t index) const
	{
		return math::Vector3<simd_type>(_x[index], _y[index], _z[index]);
	}

	const math::Vector3<simd_type> operator[](size_t index) const
	{
		return math::Vector3<simd_type>(_x[index], _y[index], _z[index]);
	}

	size_t size		() const						{ return _x.size(); }
};

};

#endif // _SIMD_TYPES_HPP