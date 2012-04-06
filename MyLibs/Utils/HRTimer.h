#ifndef HRTimer_h__
#define HRTimer_h__

#include <Windows.h>

#undef min
#undef max

struct HRTimer
{
	HRTimer()
	{
		::QueryPerformanceFrequency(&_freq);
		reset();
	}

	void reset()
	{
		::QueryPerformanceCounter(&_startTime);
	}

	unsigned int get_time()
	{
		LARGE_INTEGER currTime;
		::QueryPerformanceCounter(&currTime);
		return static_cast<unsigned int>((currTime.QuadPart - _startTime.QuadPart) * 1000 / _freq.QuadPart);
	}

private:
	LARGE_INTEGER _startTime, _freq;
};

#endif // HRTimer_h__
