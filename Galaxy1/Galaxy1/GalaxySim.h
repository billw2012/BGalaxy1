#ifndef GalaxySim_h__
#define GalaxySim_h__

#include "BHTree.h"
#include <QImage>
#include <QList>
#include <QVector3D>
#include "opencv2/highgui/highgui.hpp"
#include "boost/thread/mutex.hpp"
#include "Utils/HRTimer.h"

// http://en.wikipedia.org/wiki/Barnes-Hut_simulation
//


struct GalaxySim : public QObject
{
	Q_OBJECT 
		;

public:
	GalaxySim();

	void init_writing(const std::string& outputFile);

	bool init();

	bool resize(size_t bodyCount);

	void reset();

	struct SimType { enum type {
		Galaxy,
		Universe
	};};

	void initialize_galaxy(const pf_Vector3& center, const pf_Vector3& velocity, size_t bodies, pf_real minMass, pf_real maxMass, pf_real space);
	void initialize_universe(const pf_Vector3& center, size_t bodies, pf_real minMass, pf_real maxMass, 
		pf_real size, pf_real initVel);

	void iterate(pf_real t);

	void lock_data() const;
	const std::vector< pf_Vector3 >& get_data() const;
	void unlock_data() const;

private:
	bool load_kernel(opencl::CLProgram& program, const std::string& file, const std::string& kernel);

	void initialize_bodies(const pf_Vector3& center, const pf_Vector3& velocity, size_t bodies, pf_real minMass, pf_real maxMass, 
		pf_real size, pf_real initVel, SimType::type simType);

	void output_image();
	void output_data();

	void iterate_move( pf_real t );

	void iterate_gravity();

	std::vector<opencl::CLDevice> devices;
	typedef opencl::OpenCLBuffer<cl_real3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_real> OpenCLBufferD;

signals:
	void new_data_available();

private:
	opencl::CLProgram gravityProgram, moveProgram;
	bool _error;
	size_t _bodyCount;
	pf_real bhTheta;
	BHTree bhTree;
	math::AABB<pf_real> currBounds;
	opencl::CLDevice partitionedDevices[2];
	opencl::CLDevice* activeDevice;

	OpenCLBufferD3 _position, _acceleration, _velocity;
	OpenCLBufferD _mass;

	size_t _currBodyOffset;

	mutable boost::mutex _dataMutex;
	std::vector< pf_Vector3 > _data;

	size_t _iteration;
	HRTimer _timer;
	unsigned int _lastBHTreeCalcTime, _lastCalcGravityTime, _lastCalcMoveTime;
};
#endif // GalaxySim_h__
