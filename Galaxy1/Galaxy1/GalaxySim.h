#ifndef GalaxySim_h__
#define GalaxySim_h__

#include "BHTree.h"
#include <QImage>
#include <QList>
#include <QVector3D>
#include "opencv2/highgui/highgui.hpp"
#include "boost/thread/mutex.hpp"

#define AU_PER_LIGHTYEAR			63239.6717
#define SOLAR_MASS_IN_EARTH_MASS	332918.215

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

	void initialize_galaxy(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, double space);
	void initialize_universe(const math::Vector3d& center, size_t bodies, double minMass, double maxMass, 
		double size, double initVel);

	void iterate(double t);

	void lock_data() const;
	const std::vector< math::Vector3d >& get_data() const;
	void unlock_data() const;

private:
	bool load_kernel(opencl::CLProgram& program, const std::string& file, const std::string& kernel);

	void initialize_bodies(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, 
		double size, double initVel, SimType::type simType);

	void output_image();
	void output_data();

	void iterate_move( double t );

	void iterate_gravity();

	std::vector<opencl::CLDevice> devices;
	typedef opencl::OpenCLBuffer<cl_double3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_double> OpenCLBufferD;

signals:
	void new_data_available();

private:
	opencl::CLProgram gravityProgram, moveProgram;
	bool _error;
	size_t _bodyCount;
	double bhTheta;
	BHTree bhTree;
	math::AABBd currBounds;
	opencl::CLDevice partitionedDevices[2];
	opencl::CLDevice* activeDevice;

	OpenCLBufferD3 _position, _acceleration, _velocity;
	OpenCLBufferD _mass;

	size_t _currBodyOffset;

	mutable boost::mutex _dataMutex;
	std::vector< math::Vector3d > _data;

	size_t _iteration;
};
#endif // GalaxySim_h__
