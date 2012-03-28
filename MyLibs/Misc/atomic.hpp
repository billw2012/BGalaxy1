#if !defined(__MISC_ATOMIC_HPP__)
#define __MISC_ATOMIC_HPP__

#include "platform.h"

#if defined(PLATFORM_WIN32)
#include <Windows.h>
#endif

namespace misc {;

#if defined(PLATFORM_WIN32)

struct AtomicInt32
{
	AtomicInt32()
	{
		_val = static_cast<volatile LONG *>(_aligned_malloc(sizeof(LONG), 4));
	}

	explicit AtomicInt32(LONG val)
	{
		_val = static_cast<volatile LONG *>(_aligned_malloc(sizeof(LONG), 4));
		InterlockedExchange(_val, val);
	}

	explicit AtomicInt32(bool val)
	{
		_val = static_cast<volatile LONG *>(_aligned_malloc(sizeof(LONG), 4));
		InterlockedExchange(_val, val?1:0);
	}

	~AtomicInt32()
	{
		_aligned_free((void*)(_val));
	}

	LONG dec()
	{
		return InterlockedDecrement(_val);
	}

	LONG inc()
	{
		return InterlockedIncrement(_val);
	}

	LONG add(LONG amount) // can use for negative values aswell..
	{
		LONG prev = InterlockedExchangeAdd(_val, amount);
		return prev + amount;
	}

	operator LONG() const
	{
		// best way of doing this? 
		// (can't see a specialised read func for interlocked vars)
		return InterlockedCompareExchange(_val, 0, 0);
	}

	operator bool() const
	{
		return (LONG)(*this) != 0;
	}

	bool operator!() const
	{
		return (LONG)(*this) == 0;
	}

	LONG set(LONG val)
	{
		return InterlockedExchange(_val, val);
	}

	bool set(bool val)
	{
		return InterlockedExchange(_val, val?1:0) != 0;
	}

	LONG operator=(LONG val)
	{
		set(val);
		return val;
	}

	bool operator=(bool val)
	{
		set(val);
		return val;
	}

	bool operator==(const AtomicInt32& other) const
	{
		return (LONG)(*this) == (LONG)other;
	}

private:
	AtomicInt32(const AtomicInt32& other);

private:
	mutable LONG volatile* _val;
};

#endif

}
#endif // __MISC_ATOMIC_HPP__