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

	void init_writing(const std::string& outputFile);

	bool load_kernel(opencl::CLProgram& program, const std::string& file, const std::string& kernel);

	bool init(size_t bodyCount);

	struct SimType { enum type {
		Galaxy,
		Universe
	};};

	void initialize_galaxy(double minMass, double maxMass, double space);
	void initialize_universe(double minMass, double maxMass, 
		double size, double initVel);
	void initialize_bodies(double minMass, double maxMass, 
		double size, double initVel, SimType::type simType);

	void iterate(double t);

private:
	void output_image();

	void iterate_move( double t );

	void iterate_gravity();

	std::vector<opencl::CLDevice> devices;
	typedef opencl::OpenCLBuffer<cl_double3> OpenCLBufferD3;
	typedef opencl::OpenCLBuffer<cl_double> OpenCLBufferD;
	OpenCLBufferD3 position, acceleration, velocity, minmax;
	OpenCLBufferD mass;

signals:
	void new_image_available(QImage image);

private:
	opencl::CLProgram gravityProgram, moveProgram;
	bool _error;
	size_t _bodyCount;
	double xOffset, yOffset,  xScale, yScale, xRange, yRange;
	BHTree bhTree;
	math::AABBd currBounds;
	cv::VideoWriter _video;

};
#endif // GalaxySim_h__
