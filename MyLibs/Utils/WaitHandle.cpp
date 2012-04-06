
#include "WaitHandle.h"

const size_t WaitHandle::Infinity = INFINITE;
const size_t WaitHandle::OBJECT_0 = WAIT_OBJECT_0;

WaitHandle::WaitHandle( bool initialState )
{
	_handle = ::CreateEvent(NULL, TRUE, initialState? TRUE : FALSE, NULL);
}

WaitHandle::~WaitHandle()
{
	::CloseHandle(_handle);
}

bool WaitHandle::wait( size_t timeout /*= Infinity*/ ) const
{
	return ::WaitForSingleObject(_handle, (DWORD)timeout) == WAIT_OBJECT_0;
}

size_t WaitHandle::wait( const std::vector<WaitHandle*>& handles, bool waitAll /*= false*/, size_t timeout /*= Infinity*/ )
{
	std::vector<HANDLE> handlesProper(handles.size());
	for(size_t idx = 0; idx < handles.size(); ++idx)
		handlesProper[idx] = handles[idx]->_handle;

	return ::WaitForMultipleObjects(handlesProper.size(), &handlesProper[0], waitAll? TRUE : FALSE, timeout);
}

void WaitHandle::set()
{
	::SetEvent(_handle);
}

void WaitHandle::reset()
{
	::ResetEvent(_handle);
}

bool WaitHandle::is_set() const
{
	return wait(0);
}
