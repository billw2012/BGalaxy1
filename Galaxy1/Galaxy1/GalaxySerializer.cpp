
#include "GalaxySerializer.h"

#include "dlib/matrix.h"
#include "dlib/optimization.h"
#include "dlib/matrix/matrix_exp_abstract.h"
#include "Math/vector4.hpp"
#include "Math/matrix4.hpp"
#include "Math/transformation.hpp"

using namespace dlib;

typedef matrix<double, 0, 1> column_vector;

void GalaxySerializer::add_data( const std::vector<pf_Vector3>& positions )
{
	_timer.reset();
	std::for_each(_processThreads.begin(), _processThreads.end(), [&](boost::thread& processThread) {
		processThread.join();
	});
	std::cout << "Serializer waited " << _timer.get_time() << "ms for join." << std::endl;

	BodyPathDeque::iterator pathItr = _bodyPaths.begin();
	size_t idx = 0;
	for(; pathItr != _bodyPaths.end(); ++idx, ++pathItr)
	{
		BodyPath& path = *pathItr;
		path.add(positions[idx]);
	}
	for(; idx < positions.size(); ++idx)
	{
		_bodyPaths.push_back(BodyPath(positions[idx]));
	}

	_processThreads.clear();
	size_t threadCount = std::max<size_t>(1, boost::thread::hardware_concurrency() - 2);
	size_t bodiesPerThread = _bodyPaths.size() / threadCount;
	//size_t bodiesRemainder = _bodyPaths.size() - bodiesPerThread * (threadCount - 1);

	bool keyNow = (_currFrame != 0 && (_currFrame % _keyFrame == 0));
	if(keyNow)
		std::cout << "Keying." << std::endl;

	std::cout << "Hermite best = " << _bestHermitePathLength << ", average = " << _averageHermitePathLength << std::endl;
	_bestHermitePathLength = 0;
	_averageHermitePathLength = 0.0f;

	//BodyPathDeque::iterator bItr = _bodyPaths.begin();
	size_t firstIdx = 0, secondIdx = bodiesPerThread;
	for(size_t idx = 0; idx < threadCount - 1; ++idx, firstIdx += bodiesPerThread, secondIdx += bodiesPerThread)
	{
		_processThreads.push_back(boost::thread(std::bind(&GalaxySerializer::find_hermites, this,
			_bodyPaths.begin() + firstIdx, _bodyPaths.begin() + secondIdx, keyNow, _currFrame)));
	}
	_processThreads.push_back(boost::thread(std::bind(&GalaxySerializer::find_hermites, this,
		_bodyPaths.begin() + firstIdx, _bodyPaths.end(), keyNow, _currFrame)));
	++ _currFrame;
	//find_hermites();
}

pf_Vector3 convert_rot_to_vec(double a, double b)
{
	return pf_Vector3((math::rotatex(a) * math::rotatey(b)) * math::Vector4d(1, 0, 0, 1));
}

GalaxySerializer::pf_Hermite make_hermite(const pf_Vector3& p0, const pf_Vector3& v0, 
	const pf_Vector3& p1, const pf_Vector3& v1)
{
	pf_real len = (p1 - p0).length();
	return GalaxySerializer::pf_Hermite(p0, v0 * len, p1, v1 * len);
}

// calculate Hermite from 4 element column vector (both v0 and v1 specified)
GalaxySerializer::pf_Hermite calc_hermite4(const pf_Vector3& p0, const pf_Vector3& p1, 
	const column_vector& testVal)
{
	pf_Vector3 v0 = convert_rot_to_vec(testVal(0), testVal(1));
	pf_Vector3 v1 = convert_rot_to_vec(testVal(2), testVal(3));
	return make_hermite(p0, v0, p1, v1);
}

GalaxySerializer::pf_Hermite calc_hermite2(const pf_Vector3& p0, const pf_Vector3& p1, const pf_Vector3& v0, 
	const column_vector& testVal)
{
	pf_Vector3 v1 = convert_rot_to_vec(testVal(0), testVal(1));
	return make_hermite(p0, v0, p1, v1);
}

double evaluate_hermite_body_path(const column_vector& testVal, const GalaxySerializer::BodyPath& bodyPath)
{
	if(bodyPath.currentPositions.size() < 3)
		return 0.0;

	/*
	Pure Hermites don't work because:
	When a long Hermite transitions to a short Hermite the incoming m length breaks the next spline.
	Need a spline based on orientation of m not its length.
	*/
	GalaxySerializer::pf_Hermite currHermite;
	if(testVal.size() == 4)
	{
		currHermite = calc_hermite4(bodyPath.bestp0, bodyPath.bestp1, testVal);
	}
	else
	{
		currHermite = calc_hermite2(bodyPath.bestp0, bodyPath.bestp1, bodyPath.bestv0, testVal);
	}
	double t = 0, td = 1 / static_cast<double>(bodyPath.currentPositions.size() - 1);
	double diff = 0;
	std::for_each(bodyPath.currentPositions.begin(), bodyPath.currentPositions.end(), [&](const pf_Vector3& v) {
		pf_Vector3 p = currHermite.eval(t);
		diff = std::max<double>(diff, (p - v).lengthSquared());
		t += td;
	});
	return diff;
}

void GalaxySerializer::find_hermites(BodyPathDeque::iterator startItr, BodyPathDeque::iterator endItr, bool keyNow, int currFrame)
{
	bfgs_search_strategy searchStrategy;
	objective_delta_stop_strategy stopStrategy(1e-8, 100000);
	column_vector vec2(2), vec4(4);

	std::for_each(startItr, endItr, [&, this](BodyPath& path)
	{
		double minF = _errorMargin * _errorMargin, derivativeEps = 15.0;
		update_average_hermite(path.currentPositions.size());
		if(keyNow)
		{
			update_best_hermite(path.currentPositions.size() - 1);
			// commit all Hermites if we are key-framing
			path.commit_key(currFrame);
		}
		// if we have already added the first two keys then the second key m value is usable
		else if(path.keys.size() > 1)
		{
			vec2(0) = path.bestr1.x;
			vec2(1) = path.bestr1.y;
			double bestErr = find_min_using_approximate_derivatives(searchStrategy, stopStrategy, 
				std::bind(evaluate_hermite_body_path, std::placeholders::_1, path), vec2, minF, derivativeEps);
			// if we exceeded the max error then we should commit the last Hermite and start a new one
			if(bestErr > minF)
				path.commit_key(currFrame);
			else
				update_best_hermite(path.currentPositions.size() - 1);
			path.bestr1.x = vec2(0);
			path.bestr1.y = vec2(1);
			path.err = bestErr;
		}
		else
		{
			vec4(0) = path.bestr0.x;
			vec4(1) = path.bestr0.y;
			vec4(2) = path.bestr1.x;
			vec4(3) = path.bestr1.y;
			double bestErr = find_min_using_approximate_derivatives(searchStrategy, stopStrategy, 
				std::bind(evaluate_hermite_body_path, std::placeholders::_1, path), vec4, minF, derivativeEps);
			// if we exceeded the max error then we should commit the last Hermite and start a new one
			if(bestErr > minF)
				path.commit_key(currFrame);
			else
				update_best_hermite(path.currentPositions.size() - 1);
			path.bestr0.x = vec4(0);
			path.bestr0.y = vec4(1);
			path.bestr1.x = vec4(2);
			path.bestr1.y = vec4(3);
			path.bestv0 = convert_rot_to_vec(path.bestr0.x, path.bestr0.y);
			path.err = bestErr;
		}
	});
}

bool GalaxySerializer::save(const std::string& fileName)
{
	using namespace std;
	ofstream str(fileName.c_str(), ofstream::out | ofstream::binary);
	str.write("GXY1", 4);
	char typeSize = sizeof(pf_real);
	str.write(reinterpret_cast<const char*>(&typeSize), 1);
	str.write(reinterpret_cast<const char*>(&_currFrame), sizeof(_currFrame));
	size_t bodyPathsSize = _bodyPaths.size();
	str.write(reinterpret_cast<const char*>(&bodyPathsSize), sizeof(size_t));
	for_each(_bodyPaths.begin(), _bodyPaths.end(), [&, this](const BodyPath& bodyPath)
	{
		size_t keysSize = bodyPath.keys.size();
		str.write(reinterpret_cast<const char*>(&keysSize), sizeof(size_t));
		std::for_each(bodyPath.keys.begin(), bodyPath.keys.end(), [&](const GalaxySerializer::BodyKey& key) {
			str.write(reinterpret_cast<const char*>(&key), sizeof(GalaxySerializer::BodyKey));
		});
	});
	return true;
}

std::vector<pf_Vector3> GalaxySerializer::interpolate( float t ) const
{
	std::vector<pf_Vector3> pts;
	pts.reserve(_bodyPaths.size());
	std::for_each(_bodyPaths.begin(), _bodyPaths.end(), [&](const BodyPath& bodyPath) {
		pts.push_back(bodyPath.evaluate(t));
	});
	return pts;
}


bool GalaxySerializer::load(const std::string& fileName)
{
	using namespace std;
	ifstream str(fileName.c_str(), ifstream::in | ifstream::binary);
	char hdr[4];
	str.read(hdr, 4);
	if(::memcmp(hdr, "GXY1", 4) != 0)
		return false;
	char typeSize;
	str.read(reinterpret_cast<char*>(&typeSize), 1);
	if(typeSize != sizeof(pf_real))
	{
		std::cout << "The file was recorded with " << typeSize << " float size that this program was compiled for!" << std::endl;
		std::cout << "Try recompiling with GALAXY_DOUBLE_PRECISION " << 
			((sizeof(pf_real) == sizeof(float))? "enabled" : "disabled") << std::endl;
		return false;
	}
	str.read(reinterpret_cast<char*>(&_currFrame), sizeof(_currFrame));
	size_t bodyPathCount;
	str.read(reinterpret_cast<char*>(&bodyPathCount), sizeof(size_t));
	if(str.fail())
		return false;
	_bodyPaths.clear();
	_bodyPaths.resize(bodyPathCount);
	for_each(_bodyPaths.begin(), _bodyPaths.end(), [&, this](BodyPath& bodyPath)
	{
		size_t keyCount;
		str.read(reinterpret_cast<char*>(&keyCount), sizeof(size_t));
		if(str.fail())
			return ;
		bodyPath.keys.resize(keyCount);
		std::for_each(bodyPath.keys.begin(), bodyPath.keys.end(), [&](GalaxySerializer::BodyKey& key) {
			str.read(reinterpret_cast<char*>(&key), sizeof(GalaxySerializer::BodyKey));
			if(str.fail())
				return ;
		});
		if(str.fail())
			return ;
	});
	return true;
}

void GalaxySerializer::BodyPath::add( const pf_Vector3& p )
{
	currentPositions.push_back(p);
	bestp1 = p;
}

void GalaxySerializer::BodyPath::commit_key( int frame )
{
	pf_Vector3 bestv1 = convert_rot_to_vec(bestr1.x, bestr1.y);
	// if we are on the first key then we have calculated the first and second p0 and m0
	if(keys.empty())
		keys.push_back(BodyKey(frame - (currentPositions.size() - 1), bestp0, bestv0));
	keys.push_back(BodyKey(frame, bestp1, bestv1));
	// reset the Hermite
	bestp0 = bestp1;
	bestr0 = bestr1;
	bestv0 = bestv1;
	//bestHermite.m0 = bestHermite.m1;
	currentPositions.clear();
	currentPositions.push_back(bestp1);
}

pf_Vector3 GalaxySerializer::BodyPath::evaluate(float frame) const
{
	if(keys.empty())
		return pf_Vector3();
	int iFrame = std::floorf(frame);
	if(iFrame < keys.front().frame)
		return keys.front().p;
	if(iFrame >= keys.back().frame)
		return keys.back().p;
	BodyKeyVector::const_iterator lwrItr = std::upper_bound(keys.begin(), keys.end(), BodyKey(iFrame));
	if(lwrItr != keys.begin())
		--lwrItr;
	if(lwrItr == keys.end() || lwrItr + 1 == keys.end())
		return lwrItr->p;
	BodyKeyVector::const_iterator uprItr = lwrItr + 1;
	pf_Hermite hermite = make_hermite(lwrItr->p, lwrItr->v, uprItr->p, uprItr->v);
	float frameFraction = (frame - lwrItr->frame) / (float)(uprItr->frame - lwrItr->frame);
	return hermite.eval(frameFraction);
}