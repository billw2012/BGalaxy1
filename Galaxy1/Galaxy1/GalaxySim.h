#ifndef GalaxySim_h__
#define GalaxySim_h__

#include "BHTree.h"
#include <QImage>
#include "opencv2/highgui/highgui.hpp"

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

	void reset() { resize(0); _currBodyOffset = 0; }

	struct SimType { enum type {
		Galaxy,
		Universe
	};};

	void initialize_galaxy(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, double space);
	void initialize_universe(const math::Vector3d& center, size_t bodies, double minMass, double maxMass, 
		double size, double initVel);

	void iterate(double t);

private:
	bool load_kernel(opencl::CLProgram& program, const std::string& file, const std::string& kernel);

	void initialize_bodies(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, 
		double size, double initVel, SimType::type simType);

	void output_image();

	void iterate_move( double t );

	void iterate_gravity();

	std::vector<opencl::CLDevice> devices;
	typedef opencl::OpenCLBuffer<cl_double3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_double> OpenCLBufferD;

signals:
	void new_image_available(QImage image);

private:
	opencl::CLProgram gravityProgram, moveProgram;
	bool _error;
	size_t _bodyCount;
	double xOffset, yOffset,  xScale, yScale, xRange, yRange;
	double massOffset, massScale;
	double bhTheta;
	BHTree bhTree;
	math::AABBd currBounds;
	cv::VideoWriter _video;
	opencl::CLDevice partitionedDevices[2];
	opencl::CLDevice* activeDevice;

	OpenCLBufferD3 _position, _acceleration, _velocity;
	OpenCLBufferD _mass;

	size_t _currBodyOffset;
};
#endif // GalaxySim_h__
