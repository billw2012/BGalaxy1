
#ifndef WaitHandle_h__
#define WaitHandle_h__

#include <Windows.h>
#include <vector>


struct WaitHandle
{
	static const size_t Infinity;

	static const size_t OBJECT_0;

	WaitHandle(bool initialState = false);

	~WaitHandle();

	bool wait(size_t timeout = Infinity) const;

	void set();

	void reset();

	bool is_set() const;
	
	static size_t wait(const std::vector<WaitHandle*>& handles, bool waitAll = false, size_t timeout = Infinity);

private:
	HANDLE _handle;
};

#endif // WaitHandle_h__