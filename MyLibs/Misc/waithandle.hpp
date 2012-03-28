#if !defined(__MISC_WAIT_HANDLE_HPP__)
#define __MISC_WAIT_HANDLE_HPP__

#include "platform.h"

#if defined(PLATFORM_WIN32)

#include <Windows.h>

#endif

namespace misc{;

#if defined(PLATFORM_WIN32)

struct WaitResult { enum type {
	Abandoned = WAIT_ABANDONED,
	Triggered = WAIT_OBJECT_0,
	Timeout	  = WAIT_TIMEOUT,
	Failed    = WAIT_FAILED
};};

struct WaitHandle
{
	WaitHandle(bool startSet = false, bool manualReset = true); 
	~WaitHandle();

	void set();
	void reset();

	WaitResult::type wait_for_set(DWORD timeoutMS = INFINITE);

	bool is_set() const;

	HANDLE get_handle() const;

private:
	WaitHandle(const WaitHandle&);
	void operator=(const WaitHandle&);

private:
	HANDLE _handle;
};

inline WaitHandle::WaitHandle(bool startSet, bool manualReset)
{
	_handle = ::CreateEvent(NULL, manualReset, startSet, NULL);
	assert(_handle != NULL);
}

inline WaitHandle::~WaitHandle()
{
	::CloseHandle(_handle);
}

inline void WaitHandle::set()
{
	::SetEvent(_handle);
}

inline void WaitHandle::reset()
{
	::ResetEvent(_handle);
}

inline bool WaitHandle::is_set() const
{
	return WAIT_OBJECT_0 == ::WaitForSingleObject(_handle, 0);
}

inline WaitResult::type WaitHandle::wait_for_set(DWORD timeoutMS/* = INFINITE*/)
{
	return static_cast<WaitResult::type>(::WaitForSingleObject(_handle, timeoutMS));
}

inline HANDLE WaitHandle::get_handle() const 
{ 
	return _handle; 
}

#endif

}

#endif // __MISC_WAIT_HANDLE_HPP__
