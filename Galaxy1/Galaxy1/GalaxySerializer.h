#ifndef GalaxySerializer_h__
#define GalaxySerializer_h__

#include "Math/hermite.hpp"
#include "Math/vector2.hpp"
#include "GalaxyTypes.h"
#include "Utils/HRTimer.h"

#include <vector>
#include <deque>
#include <list>

#include <boost/thread.hpp>

struct GalaxySerializer
{
	typedef math::Hermite<pf_real> pf_Hermite;

	struct BodyKey
	{
		BodyKey(int frame_ = 0, const pf_Vector3& p_ = pf_Vector3(),
			const pf_Vector3& v_ = pf_Vector3()) : frame(frame_), p(p_), v(v_) {}
		pf_Vector3 p, v;
		// v is a unit vector indicating the tangent at p. 
		// Length of m for p[n] to p[n+1] is |p[n+1] - p[n]|
		int frame;

		bool operator<(const BodyKey& other) const
		{
			return frame < other.frame;
		}
	};

	typedef std::vector<BodyKey> BodyKeyVector;
	typedef std::vector<pf_Vector3> V3Vector;

	struct BodyPath
	{
		BodyKeyVector keys;
		V3Vector currentPositions;
		double err;
		pf_Vector3 bestp0, bestp1;
		pf_Vector3 bestv0;
		math::Vector2d bestr0, bestr1;

		BodyPath(const pf_Vector3& pos = pf_Vector3()) 
			: err(std::numeric_limits<double>::max()),
			bestp0(pos), 
			bestp1(pos)
			//bestHermite(pos, pf_Vector3(), pos, pf_Vector3())
		{
			currentPositions.push_back(pos);
		}

		void add(const pf_Vector3& p);

		void commit_key(int frame);

		pf_Vector3 evaluate(float frame) const;
	};

	GalaxySerializer() 
		: _currFrame(0), 
		_keyFrame(200),
		_bestHermitePathLength(0),
		_averageHermitePathLength(0),
		_errorMargin(1e-7)
	{}

	void set_error_margin(double errorMargin) { _errorMargin = errorMargin; }
	void add_data(const std::vector<pf_Vector3>& positions);

// 	size_t get_best_hermite_path_length() const { return _bestHermitePathLength; }
// 	float get_average_hermite_path_length() const { return _averageHermitePathLength; }

	bool save(const std::string& fileName);
	bool load(const std::string& fileName);

	std::vector<pf_Vector3> interpolate(float t) const;
	size_t get_frame_count() const { return _currFrame; }

private:
	typedef std::deque<BodyPath> BodyPathDeque;

	void find_hermites(BodyPathDeque::iterator startItr, BodyPathDeque::iterator endItr, bool keyNow, int currFrame);

	void update_best_hermite(size_t bestHermite)
	{
		boost::mutex::scoped_lock scopedLock(_metricsMutex);
		_bestHermitePathLength = std::max(_bestHermitePathLength, bestHermite);
	}

	void update_average_hermite(size_t hermiteLen)
	{
		boost::mutex::scoped_lock scopedLock(_metricsMutex);
		_averageHermitePathLength += (hermiteLen / (float)_bodyPaths.size());
	}

private:

	BodyPathDeque _bodyPaths;
	int _keyFrame;
	int _currFrame;
	double _errorMargin;

	boost::mutex _metricsMutex;
	// could be atomic...
	size_t _bestHermitePathLength;
	float _averageHermitePathLength;
	std::list<boost::thread> _processThreads;

	HRTimer _timer;
};

#endif // GalaxySerializer_h__
