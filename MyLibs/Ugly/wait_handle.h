
#if !defined(__HCAP_WAIT_HANDLE_H__)
#define __HCAP_WAIT_HANDLE_H__

#if defined(_MSC_VER)

#include <windows.h>
#include <vector>

struct WaitHandle 
{
	WaitHandle()
	{
		_handle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	void set()
	{
		::SetEvent(_handle);
	}

	void reset()
	{
		::ResetEvent(_handle);
	}

	bool is_set() const
	{
		return ::WaitForSingleObject(_handle, 0) == WAIT_OBJECT_0;
	}

	DWORD wait(DWORD timeout = INFINITE)
	{
		return ::WaitForSingleObject(_handle, timeout);
	}

	static DWORD wait(const std::vector<WaitHandle*>& handles, bool waitAll = false, DWORD timeout = INFINITE)
	{
		std::vector<HANDLE> handlesProper(handles.size());
		for(size_t idx = 0; idx < handles.size(); ++idx)
			handlesProper[idx] = handles[idx]->_handle;

		return ::WaitForMultipleObjects(handlesProper.size(), &handlesProper[0], waitAll? TRUE : FALSE, timeout);
	}
private:
	HANDLE _handle;
};

#endif // defined(_MSC_VER)

#endif // !defined(__HCAP_WAIT_HANDLE_H__)