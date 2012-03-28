#ifndef stl_aligned_allocator_h__
#define stl_aligned_allocator_h__

template <class T , int Alignment=16>
class aligned_allocator 
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	template <class U>
	struct rebind
	{
		typedef aligned_allocator<U> other;
	};

	pointer address (reference value) const 
	{
		return &value;
	}

	const_pointer address (const_reference value) const 
	{
		return &value;
	}

	aligned_allocator() throw() 
	{
	}

	aligned_allocator(const aligned_allocator&) throw() 
	{
	}

	template <class U, int A>
	aligned_allocator (const aligned_allocator<U, A>&) throw() 
	{
	}

	~aligned_allocator() throw() 
	{
	}

	//max capacity
	size_type max_size () const throw() 
	{
		return 268435455;
	}

	pointer allocate (size_type num, const_pointer *hint = 0) 
	{
		return (pointer) _aligned_malloc( num*sizeof(T), Alignment);
	}

	void construct (pointer p, const T& value) 
	{
		// memcpy( p, &value, sizeof T );
		*p=value;
		//  new ( (void *) p ) T ( value );
	}

	void destroy (pointer p) 
	{
		p->~T();
	}

	void deallocate (pointer p, size_type num) 
	{
		_aligned_free( p );
	}
};

#endif // stl_aligned_allocator_h__
