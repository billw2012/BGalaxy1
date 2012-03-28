#ifndef _SSE_UTILS_HPP
#define _SSE_UTILS_HPP
#include <xmmintrin.h>
#include <emmintrin.h>

#define force_inline __forceinline

#if defined(_MSC_VER) && !defined(DISABLE_SSE)
#	define MICROSOFT_SSE
#	define SSE_ALIGNED						_MM_ALIGN16
#	define SSE_DOUBLE_LITERAL(name, val)	static const _MM_ALIGN16 double name[] = {val, val};
#	define SSE_FLOAT_LITERAL(name, val)		static const _MM_ALIGN16 float name[] = {val, val, val, val};
#	define SSE_UINT_LITERAL(name, val)		static const _MM_ALIGN16 unsigned int name[] = {val, val, val, val};
#	define SSE_INT_LITERAL(name, val)		static const _MM_ALIGN16 int name[] = {val, val, val, val};
#	define SSE_UINT64_LITERAL(name, val)	static const _MM_ALIGN16 unsigned __int64 name[] = {val, val};
#	define SSE_INT64_LITERAL(name, val)		static const _MM_ALIGN16 __int64 name[] = {val, val};
#	define SSE_LOAD_LITERALF(name)			_mm_load_ps(reinterpret_cast<const float*>(&name))
#	define SSE_LOAD_LITERALD(name)			_mm_load_pd(reinterpret_cast<const double*>(&name))
#	define SSE_128f							__m128
#	define SSE_128d							__m128d
#	if !defined(ENABLE_SSE)
#		define ENABLE_SSE
#	endif
#endif

#undef min
#undef max

LIB_NAMESPACE_START
namespace math {;

template < class Ty > 
struct SSETraits
{
};

#ifdef MICROSOFT_SSE

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
template < class _Ty, class _Other > force_inline
bool operator==(const AlignedAllocator<_Ty>&, const AlignedAllocator<_Other>&) _THROW0()
{	// test for allocator equality (always true)
	return (true);
}

template < class _Ty, class _Other > force_inline
bool operator!=(const AlignedAllocator<_Ty>&, const AlignedAllocator<_Other>&) _THROW0()
{	// test for allocator inequality (always false)
	return (false);
}

template <> 
struct SSETraits<float>
{
	typedef float		value_type;
	typedef SSE_128f	sse_type;
	typedef std::vector< value_type, AlignedAllocator< value_type > > Vector;

	static const size_t VEC_SIZE = 4;

	static force_inline sse_type pack_vals(value_type* flts)						{ return _mm_loadu_ps(flts); }
	static force_inline sse_type pack_vals(const value_type* flts)				{ return _mm_loadu_ps(flts); }
	static force_inline sse_type pack_vals_aligned(value_type* flts)				{ return _mm_load_ps(flts); }
	static force_inline sse_type pack_vals_aligned(const value_type* flts)		{ return _mm_load_ps(flts); }
	static force_inline sse_type pack_vals(const value_type* a, const value_type* b, const value_type* c, const value_type* d)
	{
		sse_type mac = _mm_unpacklo_ps(_mm_load_ss(a), _mm_load_ss(c));
		sse_type mbd = _mm_unpacklo_ps(_mm_load_ss(b), _mm_load_ss(d));
		return _mm_unpacklo_ps(mac, mbd);
	}
	static force_inline void unpack_vals_aligned(sse_type val, value_type* flts)	{ _mm_store_ps(flts, val); }

	static force_inline sse_type load1(const value_type* val)						{ return _mm_load1_ps(val); }
	static force_inline sse_type set1(value_type val)								{ return _mm_set1_ps(val); }
	static force_inline void stream(value_type* tgt, sse_type val)				{ _mm_stream_ps(tgt, val); }

	static force_inline sse_type sub(sse_type a, sse_type b)	{ return _mm_sub_ps(a, b); }
	static force_inline sse_type mul(sse_type a, sse_type b)	{ return _mm_mul_ps(a, b); }
	static force_inline sse_type add(sse_type a, sse_type b)	{ return _mm_add_ps(a, b); }

	static force_inline sse_type min(sse_type a, sse_type b)	{ return _mm_min_ps(a, b); }
	static force_inline sse_type max(sse_type a, sse_type b)	{ return _mm_max_ps(a, b); }

	// Thanks go to Moomin for this >>>>>>>>
	static force_inline sse_type round(sse_type x)
	{
		SSE_INT_LITERAL(sign_mask, 0x80000000);
		SSE_FLOAT_LITERAL(_2_pow_23, 8388608.0f);
		sse_type t=_mm_or_ps( _mm_and_ps(SSE_LOAD_LITERALF(sign_mask),x), SSE_LOAD_LITERALF(_2_pow_23) );
		return _mm_sub_ps(_mm_add_ps(x,t),t);
	}

	static force_inline sse_type floor(sse_type x)
	{
		SSE_FLOAT_LITERAL(one, 1.0f);
		sse_type t=round(x);
		return _mm_sub_ps(t,_mm_and_ps(_mm_cmplt_ps(x,t),SSE_LOAD_LITERALF(one)));
	}
	// <<<<<<<<<<<<<<<<

	static force_inline void extract_int32s(sse_type v, int& a, int& b, int& c, int& d)
	{
		a = _mm_cvtss_si32(v);
		v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
		b = _mm_cvtss_si32(v);
		v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
		c = _mm_cvtss_si32(v);
		v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
		d = _mm_cvtss_si32(v);
	}

	static force_inline sse_type abs(sse_type val)
	{
		SSE_UINT_LITERAL(maskval, 0x7fffffff);
		SSE_128f mask = SSE_LOAD_LITERALF(maskval);
		return _mm_and_ps(val, mask);
	}
};

//const size_t SSETraits<float>::VEC_SIZE = 4;

template <> 
struct SSETraits<double>
{
	typedef double		value_type;
	typedef SSE_128d	sse_type;
	static const size_t VEC_SIZE = 2;
	typedef std::vector< value_type, AlignedAllocator< value_type > > Vector;

	static force_inline sse_type pack_vals(value_type* flts)						{ return _mm_loadu_pd(flts); }
	static force_inline sse_type pack_vals(const value_type* flts)				{ return _mm_loadu_pd(flts); }
	static force_inline sse_type pack_vals_aligned(value_type* flts)				{ return _mm_load_pd(flts); }
	static force_inline sse_type pack_vals_aligned(const value_type* flts)		{ return _mm_load_pd(flts); }
	static force_inline sse_type pack_vals(const value_type* a, const value_type* b)
	{
		return _mm_unpacklo_pd(_mm_load_sd(a), _mm_load_sd(b));
	}
	static force_inline void unpack_vals_aligned(sse_type val, value_type* flts)	{ _mm_store_pd(flts, val); }

	static force_inline sse_type load1(const value_type* val)						{ return _mm_load1_pd(val); }
	static force_inline sse_type set1(value_type val)								{ return _mm_set1_pd(val); }
	static force_inline void stream(value_type* tgt, sse_type val)				{ _mm_stream_pd(tgt, val); }

	static force_inline sse_type sub(sse_type a, sse_type b)	{ return _mm_sub_pd(a, b); }
	static force_inline sse_type mul(sse_type a, sse_type b)	{ return _mm_mul_pd(a, b); }
	static force_inline sse_type add(sse_type a, sse_type b)	{ return _mm_add_pd(a, b); }

	static force_inline sse_type min(sse_type a, sse_type b)	{ return _mm_min_pd(a, b); }
	static force_inline sse_type max(sse_type a, sse_type b)	{ return _mm_max_pd(a, b); }

#pragma warning(disable : 4799)
	static force_inline sse_type round(sse_type x)
	{
		// convert to int and then back to double
		return _mm_cvtpi32_pd(_mm_cvtpd_pi32(x));
	}
#pragma warning(default : 4799)

	static force_inline sse_type floor(sse_type x)
	{
		SSE_DOUBLE_LITERAL(one, 1.0);
		sse_type t=round(x);
		return _mm_sub_pd(t,_mm_and_pd(_mm_cmplt_pd(x,t),SSE_LOAD_LITERALD(one)));
	}

	static force_inline void extract_int32s(sse_type v, int& a, int& b)
	{
		a = _mm_cvtsd_si32(v);
		v = _mm_shuffle_pd(v, v, _MM_SHUFFLE2(0, 1));
		b = _mm_cvtsd_si32(v);
	}

	static force_inline sse_type abs(sse_type val)
	{
		SSE_UINT64_LITERAL(maskval, 0x7fffffffFFFFFFFF);
		sse_type mask = SSE_LOAD_LITERALD(maskval);
		return _mm_and_pd(val, mask);
	}
};
//const size_t SSETraits<double>::VEC_SIZE = 2;

//typedef float value_type;
//typedef float simd_typed;
//typedef std::vector< value_type, AlignedAllocator< value_type > > SIMDArray;
//typedef std::vector< simd_typed, AlignedAllocator< simd_typed > > SIMDArrayd;
//
template < class FTy_ >
struct SIMDSoAVector3
{
private:
	typedef FTy_ value_type;
	typedef typename SSETraits<FTy_>::sse_type sse_type;
	typedef typename SSETraits<FTy_>::Vector VectorType;
	VectorType	_x,	_y, _z;

public:
	SIMDSoAVector3	(size_t esize, value_type val)	: _x(esize, val), _y(esize, val), _z(esize, val) {}
	SIMDSoAVector3	(size_t esize)					: _x(esize), _y(esize), _z(esize) {}
	SIMDSoAVector3	()								: _x(), _y(), _z() {}

	void resize		(size_t size)
	{
		_x.resize(size);
		_y.resize(size);
		_z.resize(size);
	}

	void resize		(size_t size, value_type val)
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
		VectorType tx, ty, tz;
		_x.swap(tx);
		_y.swap(ty);
		_z.swap(tz);
	}

	value_type& x		(size_t index)					{ return _x[index]; }
	const value_type& x	(size_t index) const			{ return _x[index]; }
	value_type& y		(size_t index)					{ return _y[index]; }
	const value_type& y	(size_t index) const			{ return _y[index]; }
	value_type& z		(size_t index)					{ return _z[index]; }
	const value_type& z	(size_t index) const			{ return _z[index]; }

	void push_back  (value_type x, value_type y, value_type z)
	{
		_x.push_back(x); _y.push_back(y); _z.push_back(z);
	}

	void push_back  (const math::Vector3<value_type>& val)
	{
		_x.push_back(val.x); _y.push_back(val.y); _z.push_back(val.z);
	}

	void set		(size_t index, const math::Vector3<value_type>& val)
	{
		_x[index] = val.x;
		_y[index] = val.y;
		_z[index] = val.z;
	}

	const math::Vector3<value_type> get(size_t index) const
	{
		return math::Vector3<value_type>(_x[index], _y[index], _z[index]);
	}

	const math::Vector3<value_type> operator[](size_t index) const
	{
		return math::Vector3<value_type>(_x[index], _y[index], _z[index]);
	}

	size_t size		() const						{ return _x.size(); }
};

//template < class Ty_ >
//static force_inline SSETraits<Ty_>::sse_type pack_vals(Ty_* flts)
//{
//	return _mm_loadu_ps(flts);
//}
//
//static force_inline SSE_128f pack_vals(const float* flts)
//{
//	return _mm_loadu_ps(flts);
//}

//static force_inline SSE_128f packFloatsAligned(float* flts)
//{
//	return _mm_load_ps(flts);
//}
//
//static force_inline SSE_128f packFloatsAligned(const float* flts)
//{
//	return _mm_load_ps(flts);
//}

//static force_inline SSE_128f pack_vals(const float* a, const float* b, const float* c, const float* d)
//{
//	SSE_128f mac = _mm_unpacklo_ps(_mm_load_ss(a), _mm_load_ss(c));
//	SSE_128f mbd = _mm_unpacklo_ps(_mm_load_ss(b), _mm_load_ss(d));
//	//return _mm_movehl_ps(_mm_load_ss(b), _mm_load_ss(d));
//	return _mm_unpacklo_ps(mac, mbd);
//
//
//	////return _mm_set_ps(*a, *b, *c, *d);
//	//// load a
//	//SSE_128f v1 = _mm_load_ss(a);
//	//// load b and merge the two registers
//	//v1 = _mm_shuffle_ps(v1, _mm_load_ss(b), _MM_SHUFFLE(0, 1, 0, 1));
//	//// shuffle the values to be next to each other
//	//v1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 3, 1, 3));
//	//// load c
//	//SSE_128f v2 = _mm_load_ss(c);
//	//// load d and merge the two registers
//	//v2 = _mm_shuffle_ps(v2, _mm_load_ss(d), _MM_SHUFFLE(0, 1, 0, 1));
//	//// shuffle the values to be next to each other
//	//v2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 3, 1, 3));
//	//// shuffle the two sets of values together into a single register
//	//SSE_128f r = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(0, 1, 2, 3));
//	//return r;
//}
//

//static force_inline void unpack_vals(SSE_128f val, float* flts)
//{
//	_mm_storeu_ps(flts, val);
//}
//
//static force_inline void unpack_vals_aligned(SSE_128f val, float* flts)
//{
//	_mm_store_ps(flts, val);
//}
//
//static force_inline void unpack_vals(SSE_128f val, float& a, float& b, float& c, float& d)
//{
//	float v[4];
//	_mm_storeu_ps(v, val);
//	a = v[0]; b = v[1]; c = v[2]; d = v[3];
//}
//
//static force_inline SSE_128f floorfSSE(SSE_128f flt)
//{
//	// convert lower two floats to 32 bit int
//	__m64 low = _mm_cvtps_pi32(flt);
//	// move high floats to low floats and covert then to 32 bit int
//	__m64 high = _mm_cvtps_pi32(_mm_movehl_ps(flt, flt));
//	// convert ints back to floats and load into low, pass through top values from original
//	SSE_128f ret = _mm_cvtpi32_ps(flt, high);
//	// convert ints back to floats and load into low, after swapping low to high
//	return _mm_cvtpi32_ps(_mm_movelh_ps(ret, ret), low);
//}
//
//static force_inline void floorfSSE(SSE_128f flt, __m64* low, __m64* high)
//{
//	// convert lower two floats to 32 bit int
//	*low = _mm_cvtps_pi32(flt);
//	// move high floats to low floats and covert then to 32 bit int
//	*high = _mm_cvtps_pi32(_mm_movehl_ps(flt, flt));
//}

//// Thanks go to Moomin for this >>>>>>>>
//static force_inline SSE_128f _mm_round_ps(SSE_128f x)
//{
//	SSE_INT_LITERAL(sign_mask, 0x80000000);
//	SSE_FLOAT_LITERAL(_2_pow_23, 8388608.0f);
//	SSE_128f t=_mm_or_ps( _mm_and_ps(SSE_LOAD_LITERALF(sign_mask),x), SSE_LOAD_LITERALF(_2_pow_23) );
//	return _mm_sub_ps(_mm_add_ps(x,t),t);
//}
//
//static force_inline SSE_128f _mm_floor_ps(SSE_128f x)
//{
//	SSE_FLOAT_LITERAL(one, 1.0f);
//	SSE_128f t=_mm_round_ps(x);
//	return _mm_sub_ps(t,_mm_and_ps(_mm_cmplt_ps(x,t),SSE_LOAD_LITERALF(one)));
//}
//// <<<<<<<<<<<<<<<<

//static force_inline SSE_128f packInts(__m64 low, __m64 high)
//{
//	// convert ints back to floats and load into low, pass through top values from original
//	SSE_128f ret;
//	ret = _mm_cvtpi32_ps(ret, high);
//	// convert ints back to floats and load into low, after swapping low to high
//	return _mm_cvtpi32_ps(_mm_movelh_ps(ret, ret), low);
//}
//
//static force_inline void extract_int32s(__m64 v, int* low, int* high)
//{
//	// extract low 16bits of low word
//	int ll = _mm_extract_pi16(v, 0);
//	// extract high 16bits of low word
//	int lh = _mm_extract_pi16(v, 1);
//	// extract low 16bits of high word
//	int hl = _mm_extract_pi16(v, 2);
//	// extract high 16bits of high word
//	int hh = _mm_extract_pi16(v, 3);
//	*low = (lh << 16) + ll;
//	*high = (hh << 16) + hl;
//}
//
//static force_inline void extract_int32s(SSE_128f v, int& a, int& b, int& c, int& d)
//{
//	a = _mm_cvtss_si32(v);
//	v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
//	b = _mm_cvtss_si32(v);
//	v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
//	c = _mm_cvtss_si32(v);
//	v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
//	d = _mm_cvtss_si32(v);
//}

//// this function returns the absolute value of its input but zeroing the sign bit
//static force_inline SSE_128f abs(SSE_128f val)
//{
//	SSE_UINT_LITERAL(maskval, 0x7fffffff);
//	SSE_128f mask = SSE_LOAD_LITERALF(maskval);
//	return _mm_and_ps(val, mask);
//}

//static force_inline SSE_128f minSSE(SSE_128f a, SSE_128f b)
//{
//	return _mm_min_ps(a, b);
//	//SSE_128f cmp = _mm_cmplt_ps(a, b);
//	//SSE_128f res = _mm_and_ps(cmp, a);
//	//res = _mm_add_ps(res, _mm_andnot_ps(cmp, b));
//	//return res;
//}
//
//static force_inline SSE_128f maxSSE(SSE_128f a, SSE_128f b)
//{
//	return _mm_max_ps(a, b);
//	//SSE_128f cmp = _mm_cmpgt_ps(a, b);
//	//SSE_128f res = _mm_and_ps(cmp, a);
//	//res = _mm_add_ps(res, _mm_andnot_ps(cmp, b));
//	//return res;
//}

#endif

}

LIB_NAMESPACE_END

#endif // _SSE_UTILS_HPP
