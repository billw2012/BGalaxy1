
#include "GalaxySim.h"
#include <iostream>
#include <boost/random/taus88.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <QColor>

using namespace opencl;

#define GRAVITY_PROGRAM	"../Data/CL/gravity.cl"
#define GRAVITY_KERNEL	"gravity"
#define MOVE_PROGRAM	"../Data/CL/move.cl"
#define MOVE_KERNEL		"move"

#define IMAGE_WIDTH		1024
#define IMAGE_HEIGHT	1024

#define DEFAULT_DEVICE_INDEX 0
#define EXPORT_IMAGES
#define EXPORT_FRAME_INTERVAL 1

#define USE_OPENCL

GalaxySim::GalaxySim()
	:
	_error(false),
	_bodyCount(0),
	xOffset(0.0), yOffset(0.0), 
	xScale(0.0), yScale(0.0), 
	xRange(0.0), yRange(0.0),
	massOffset(0.0), massScale(0.0), 
	bhTheta(0.0), _currBodyOffset(0)
{

}

void GalaxySim::init_writing(const std::string& outputFile)
{
	// CV_FOURCC('x','v','i','d')
	_video.open(outputFile, CV_FOURCC('m','p','e','g'), 30.0, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
}

bool GalaxySim::load_kernel( CLProgram& program, const std::string& file, const std::string& kernel )
{
	bool buildSuccess = program.create(file, 
#if defined(KERNEL_DEBUGGING)
		CLProgram::ProgramOptions().debugging(), *activeDevice
#else
		CLProgram::ProgramOptions()
#endif
		);

#if defined(KERNEL_DEBUGGING)
	std::cout << file << " build log for device " << *activeDevice.name << std::endl <<
		program.get_build_log(*activeDevice) << std::endl;
#else
	std::for_each(devices.begin(), devices.end(), [&](const CLDevice& device) {
		std::cout << file << " build log for device " << device.name << std::endl <<
			program.get_build_log(device) << std::endl;
	});
#endif
	if(!program.create_kernal(kernel))
	{
		std::cout << "Could not create kernel " << kernel << std::endl;
		return false;
	}
	return buildSuccess;
}

// Init extension function pointers
#define INIT_CL_EXT_FCN_PTR(name) \
	if(!pfn_##name) { \
	pfn_##name = (name##_fn) clGetExtensionFunctionAddress(#name); \
	if(!pfn_##name) { \
	std::cout << "Cannot get pointer to ext. fcn. " #name << std::endl; \
	return false; \
	} \
	}

static clCreateSubDevicesEXT_fn pfn_clCreateSubDevicesEXT = NULL;
static clReleaseDeviceEXT_fn pfn_clReleaseDeviceEXT = NULL;

bool GalaxySim::resize( size_t bodyCount )
{
	_bodyCount = bodyCount;

// 	position.enqueue_read(*activeDevice, true);
// 	acceleration.enqueue_read(*activeDevice, true);
// 	velocity.enqueue_read(*activeDevice, true);
// 	mass.enqueue_read(*activeDevice, true);

	_position.resize(bodyCount);
	_acceleration.resize(bodyCount);
	_velocity.resize(bodyCount);
	_mass.resize(bodyCount);

#if defined(USE_OPENCL)
	moveProgram.bind_parameter(MOVE_KERNEL, 1, _position);
	moveProgram.bind_parameter(MOVE_KERNEL, 2, _velocity);
	moveProgram.bind_parameter(MOVE_KERNEL, 3, _acceleration);
	moveProgram.bind_parameter(MOVE_KERNEL, 4, bodyCount);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 1, _position);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 2, _mass);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 3, _acceleration);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 9, bodyCount);
#endif

	return true;
}

bool GalaxySim::init()
{
	devices = OpenCL::get_all_devices();

	activeDevice = &devices[DEFAULT_DEVICE_INDEX];
#if defined(USE_OPENCL)
	if(!load_kernel(gravityProgram, GRAVITY_PROGRAM, GRAVITY_KERNEL))
	{
		_error = true;
		std::cout << "ERROR: cannot initialize gravity kernel!" << std::endl;
		return false;
	}
	if(!load_kernel(moveProgram, MOVE_PROGRAM, MOVE_KERNEL))
	{
		_error = true;
		std::cout << "ERROR: cannot initialize move kernel!" << std::endl;
		return false;
	}
#endif
	_position.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	_acceleration.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	_velocity.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	_mass.init(OpenCLBufferD::Flags::KERNEL_READ_WRITE);
	bhTree.init();

	return true;
}

void GalaxySim::initialize_galaxy(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, double space)
{
	initialize_bodies(center, velocity, bodies, minMass, maxMass, space, 0.0, SimType::Galaxy);
}

void GalaxySim::initialize_universe(const math::Vector3d& center, size_t bodies, double minMass, double maxMass, 
	double size, double initVel)
{
	initialize_bodies(center, math::Vector3d::Zero, bodies, minMass, maxMass, size, initVel, SimType::Universe);
}

#define PI_VAL 3.14159265

#include "galaxy_c.h"

void GalaxySim::initialize_bodies(const math::Vector3d& center, const math::Vector3d& velocity, size_t bodies, double minMass, double maxMass, double size, 
	double initVel, SimType::type simType)
{
	boost::random::taus88 rng;
	boost::random::uniform_real_distribution<double> massRange(minMass, maxMass);
	boost::random::uniform_real_distribution<double> angleRange(0.0, 2 * PI_VAL);
	boost::random::uniform_real_distribution<double> zrRange(-1.0, 1.0);
	boost::random::exponential_distribution<double> posRange(1.2);
	using namespace math;
	Matrix3d rot(
		0,  1,  1, 
		-1, 0,  1,
		0,  0,  1);

	massOffset = minMass * 0.5;
	massScale = 255.0 / (maxMass * 0.5 - minMass);

	resize(_currBodyOffset + bodies);

	bhTheta = 5.0;

#if defined(USE_OPENCL)
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 0, (cl_double)bhTheta);
#endif

	for(size_t idx = _currBodyOffset; idx < bodies + _currBodyOffset; ++idx)
	{
		_mass[idx] = massRange(rng);
		cl_double4& currPos = _position[idx];
		cl_double4& currVel = _velocity[idx];

		if(simType == SimType::Galaxy)
		{
			Vector3d currPosT;

			do 
			{
				double distance = posRange(rng) * size * 0.2;
				double angle = angleRange(rng);
				double x = sin(angle);
				double y = cos(angle);
				currPosT = Vector3d(x * distance, y * distance, zrRange(rng) * size * 0.01);
			} while (currPosT.length() > size);
			currPos = convert_my_vec3(currPosT + center);
			currVel = convert_my_vec3(Vector3d::Zero);
		}
		else
		{
			Vector3d rpos(zrRange(rng), zrRange(rng), zrRange(rng));
			rpos.normalize();
			currPos = convert_my_vec3(rpos);
			currVel = convert_my_vec3(rpos * initVel);
		}
	}

#if defined(USE_OPENCL)
	_position.enqueue_write(*activeDevice, true);
	_mass.enqueue_write(*activeDevice, true);
	_acceleration.enqueue_write(*activeDevice, true);
	_velocity.enqueue_write(*activeDevice, true);
#endif

	if(simType == SimType::Galaxy)
	{
		// recalculate bounds
		currBounds.reset();
		for(size_t idx = 0; idx < _bodyCount; ++idx)
		{
			currBounds.expand(convert_to_my_vec3(_position[idx]));
		}

		xOffset = currBounds.min().x * 1.5;
		yOffset = currBounds.min().y * 1.5;
		xRange = currBounds.extents().x * 1.5;
		yRange = currBounds.extents().y * 1.5;
		xScale = static_cast<double>(IMAGE_WIDTH - 1) / xRange;
		yScale = static_cast<double>(IMAGE_HEIGHT - 1) / yRange;

		iterate_gravity();

#if defined(USE_OPENCL)
		_acceleration.enqueue_read(*activeDevice, true);
#endif

		// determine acceleration for objects
		for(size_t idx = _currBodyOffset; idx < bodies + _currBodyOffset; ++idx)
		{
			// escape velocity = - sqrt(2 * acceleration_due_to_gravity * distance_from_gravitational_center)
			Vector3d currAccelT(convert_to_my_vec3(_acceleration[idx]));
			Vector3d currPosT(convert_to_my_vec3(_position[idx]));
			double initialStability = 1.0; // 1.0 for perfect stability, lower for collapse, higher for explosion
			double escapeVel = - std::sqrt(initialStability * currAccelT.length() * (currPosT - center).length());
			Vector3d newVelT = rot * currAccelT.normal() * escapeVel;
			_velocity[idx] = convert_my_vec3(newVelT + velocity);
		}
#if defined(USE_OPENCL)
		_velocity.enqueue_write(*activeDevice, true);
#endif
	}
	else
	{
		xOffset = -size * 0.5;
		yOffset = -size * 0.5;
		xRange = size;
		yRange = size;
		xScale = static_cast<double>(IMAGE_WIDTH - 1) / xRange;
		yScale = static_cast<double>(IMAGE_HEIGHT - 1) / yRange;
	}

	_currBodyOffset += bodies;
}

void GalaxySim::iterate( double t )
{
	static size_t sIteration = 0;

	iterate_gravity();
	iterate_move(t);
	output_image();

	std::cout << "Iteration " << sIteration << std::fixed << ". X Range = " << yRange << ", Y Range = " << yRange << std::endl;
	++ sIteration;
}

void GalaxySim::iterate_gravity()
{
	bhTree.build(_position.get_data(), _mass.get_data(), currBounds.min(), currBounds.max());
#if defined(USE_OPENCL)
	bhTree.enqueue_write(*activeDevice);
	bhTree.bind_as_parameters(gravityProgram, GRAVITY_KERNEL, 4);
	gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, _bodyCount);
	gravityProgram.wait_all();
#else
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		program_gravity(idx, _position[idx], _mass[idx], bhTheta, &_acceleration.get_data()[0], 
			&bhTree.nodes_children.get_data()[0], &bhTree.nodes_massCenter.get_data()[0],
			&bhTree.nodes_mass.get_data()[0], &bhTree.nodes_nnn.get_data()[0],
			&bhTree.nodes_ppp.get_data()[0]);
	}
#endif
}

void GalaxySim::iterate_move( double t )
{
#if defined(USE_OPENCL)
	moveProgram.bind_parameter(MOVE_KERNEL, 0, (cl_double)t);
	moveProgram.enqueue_work(*activeDevice, MOVE_KERNEL, _bodyCount);
	_position.enqueue_read(*activeDevice, true);
#else
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		program_move(idx, t, &_position.get_data()[0], &velocity.get_data()[0], 
			&_acceleration.get_data()[0]);
	}
#endif
}

void GalaxySim::output_image()
{
	QImage output(IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_RGB888);
	const OpenCLBufferD3::vector_type& positionVals = _position.get_data();
	output.fill(Qt::black);

	currBounds.reset();
	math::Vector3d avPos;
	for(size_t idx = 0; idx < positionVals.size(); ++idx)
	{
		int x = (positionVals[idx].s[0] - xOffset) * xScale;
		int y = (positionVals[idx].s[1] - yOffset) * yScale;
		unsigned char c = (_mass[idx] - massOffset) * massScale;
		if(x >= 0 && x < IMAGE_WIDTH && y >= 0 && y < IMAGE_HEIGHT)
		{
			output.setPixel(x, y, (c << 16) + (c << 8) + c);
		}
		currBounds.expand(convert_to_my_vec3(positionVals[idx]));
		avPos += convert_to_my_vec3(positionVals[idx]) / (double)_bodyCount;
	}
	xOffset = avPos.x - xRange * 0.5;
	yOffset = avPos.y - yRange * 0.5;

	if(_video.isOpened())
	{
		cv::Mat img(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);
		assert(output.byteCount() == img.total() * img.elemSize());
		memcpy_s(img.ptr(), img.total() * img.elemSize(), output.bits(), output.byteCount());
		_video << img;
	}
	emit new_image_available(output);
}
