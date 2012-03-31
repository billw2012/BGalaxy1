
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

bool GalaxySim::init( size_t bodyCount )
{
	devices = OpenCL::get_all_devices();

	activeDevice = &devices[DEFAULT_DEVICE_INDEX];
	//// Initialize clCreateSubDevicesEXT and clReleaseDeviceEXT function pointers
	//INIT_CL_EXT_FCN_PTR(clCreateSubDevicesEXT);
	//INIT_CL_EXT_FCN_PTR(clReleaseDeviceEXT);

	//cl_device_partition_property_ext props[] = { (cl_device_partition_property_ext)CL_DEVICE_PARTITION_BY_COUNTS_EXT, 
	//	(cl_device_partition_property_ext)devices[DEFAULT_DEVICE_INDEX].computeUnits - 1, 1, 0 };
	//cl_device_id outDevices[2];
	//cl_uint numDevicesRet;
	//cl_int result = pfn_clCreateSubDevicesEXT(devices[DEFAULT_DEVICE_INDEX].device, props, 2, outDevices, &numDevicesRet);
	//if(result != CL_SUCCESS)
	//{
	//	std::cout << "Partitioning devices failed!" << std::endl;
	//	return false;
	//}
	//partitionedDevices[0] = CLDevice(outDevices[0], devices[DEFAULT_DEVICE_INDEX].context, 
	//	::clCreateCommandQueue(devices[DEFAULT_DEVICE_INDEX].context, outDevices[0], 0, NULL));
	//activeDevice = &partitionedDevices[0];

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
	position.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	position.create(bodyCount);
	acceleration.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	acceleration.create(bodyCount);
	velocity.init(OpenCLBufferD3::Flags::KERNEL_READ_WRITE);
	velocity.create(bodyCount);
	mass.init(OpenCLBufferD::Flags::KERNEL_READ_WRITE);
	mass.create(bodyCount);
	bhTree.init();

#if defined(USE_OPENCL)
	moveProgram.bind_parameter(MOVE_KERNEL, 1, position);
	moveProgram.bind_parameter(MOVE_KERNEL, 2, velocity);
	moveProgram.bind_parameter(MOVE_KERNEL, 3, acceleration);
	moveProgram.bind_parameter(MOVE_KERNEL, 4, bodyCount);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 1, position);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 2, mass);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 3, acceleration);
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 9, bodyCount);
#endif

	_bodyCount = bodyCount;

	return true;
}

void GalaxySim::initialize_galaxy(double minMass, double maxMass, double space)
{
	initialize_bodies(minMass, maxMass, space, 0.0, SimType::Galaxy);
}

void GalaxySim::initialize_universe(double minMass, double maxMass, 
	double size, double initVel)
{
	initialize_bodies(minMass, maxMass, size, initVel, SimType::Universe);
}

#define PI_VAL 3.14159265

void GalaxySim::initialize_bodies( double minMass, double maxMass, double size, 
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
	   -1,  0,  1,
		0,  0,  1);

	massOffset = minMass * 0.5;
	massScale = 255.0 / (maxMass * 0.5 - minMass);

	bhTheta = 5.0;
#if defined(USE_OPENCL)
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 0, (cl_double)bhTheta);
#endif

	currBounds.reset();
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		mass[idx] = massRange(rng);
		cl_double4& currPos = position[idx];
		cl_double4& currVel = velocity[idx];

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
			currPos.s[0] = currPosT.x;
			currPos.s[1] = currPosT.y;
			currPos.s[2] = currPosT.z;

			currBounds.expand(currPosT);

			//double len = currPosT.lengthSquared() * 0.5;
			//Vector3d currPosV(currPos.s[0], currPos.s[1], 1);
			//currPosV.normalize();
			//Vector3d currVelV = rot * (currPosV * size * size / len);

			currVel.s[0] = 0.0;
			currVel.s[1] = 0.0;
			currVel.s[2] = 0.0;
		}
		else
		{
			Vector3d rpos(zrRange(rng), zrRange(rng), zrRange(rng));
			rpos.normalize();
			currPos.s[0] = rpos.x * 1;
			currPos.s[1] = rpos.y * 1;
			currPos.s[2] = rpos.z * 1;
			currVel.s[0] = rpos.x * initVel;
			currVel.s[1] = rpos.y * initVel;
			currVel.s[2] = rpos.z * initVel;
		}
	}
	if(simType == SimType::Galaxy)
	{
		// determine acceleration for objects

#if defined(USE_OPENCL)
		position.enqueue_write(*activeDevice, true);
		mass.enqueue_write(*activeDevice, true);
		acceleration.enqueue_write(*activeDevice, true);
		velocity.enqueue_write(*activeDevice, true);
#endif

		iterate_gravity();

#if defined(USE_OPENCL)
		acceleration.enqueue_read(*activeDevice, true);
#endif

		for(size_t idx = 0; idx < _bodyCount; ++idx)
		{
			// escape velocity = - sqrt(2 * acceleration_due_to_gravity * distance_from_gravitational_center)
			const cl_double4& currAccel		= acceleration[idx];
			Vector3d currAccelT(currAccel.s[0], currAccel.s[1], currAccel.s[2]);
			const cl_double4& currPos	= position[idx];
			Vector3d currPosT(currPos.s[0], currPos.s[1], currPos.s[2]);
			double initialStability = 1.3; // 1.0 for perfect stability, lower for collapse, higher for explosion
			double escapeVel = - std::sqrt(initialStability * currAccelT.length() * currPosT.length());
			//double len = currPosT.lengthSquared() * 0.5;
			//Vector3d currPosV(currPos.s[0], currPos.s[1], 1);
			//currPosV.normalize();

			Vector3d newVelT = rot * currAccelT.normal() * escapeVel;

			cl_double4& currVel = velocity[idx];
			currVel.s[0] = newVelT.x;
			currVel.s[1] = newVelT.y;
			currVel.s[2] = newVelT.z;
		}

		xOffset = currBounds.min().x * 1.5;
		yOffset = currBounds.min().y * 1.5;
		xRange = currBounds.extents().x * 1.5;
		yRange = currBounds.extents().y * 1.5;
		xScale = static_cast<double>(IMAGE_WIDTH - 1) / xRange;
		yScale = static_cast<double>(IMAGE_HEIGHT - 1) / yRange;
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

#if defined(USE_OPENCL)

	position.enqueue_write(*activeDevice, true);
	mass.enqueue_write(*activeDevice, true);
	acceleration.enqueue_write(*activeDevice, true);
	velocity.enqueue_write(*activeDevice, true);
#endif
}

#include "galaxy_c.h"

void GalaxySim::iterate( double t )
{
	static size_t sIteration = 0;

	iterate_gravity();

//#if defined(USE_OPENCL)
//	acceleration.enqueue_read(*activeDevice, true);
//#endif

	iterate_move(t);

	output_image();

	std::cout << "Iteration " << sIteration << std::fixed << ". X Range = " << yRange << ", Y Range = " << yRange << std::endl;
	++ sIteration;
}

void GalaxySim::output_image()
{
	QImage output(IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_RGB888);
	const OpenCLBufferD3::vector_type& positionVals = position.get_data();
	//QRgb col = QColor(Qt::white).rgb();
	output.fill(Qt::black);

	//double zOffset = currBounds.min().z;
	//double zScale = 255 / currBounds.extents().z;

	currBounds.reset();
	math::Vector3d avPos;
	for(size_t idx = 0; idx < positionVals.size(); ++idx)
	{
		int x = (positionVals[idx].s[0] - xOffset) * xScale;
		int y = (positionVals[idx].s[1] - yOffset) * yScale;
		unsigned char c = (mass[idx] - massOffset) * massScale;
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

void GalaxySim::iterate_move( double t )
{
#if defined(USE_OPENCL)

	moveProgram.bind_parameter(MOVE_KERNEL, 0, (cl_double)t);
	moveProgram.enqueue_work(*activeDevice, MOVE_KERNEL, /*0,*/ _bodyCount/*, _bodyCount*/);

	position.enqueue_read(*activeDevice, true);
#else
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		program_move(idx, t, &position.get_data()[0], &velocity.get_data()[0], 
			&acceleration.get_data()[0]);
	}
#endif
}

void GalaxySim::iterate_gravity()
{

	bhTree.build(position.get_data(), mass.get_data(), currBounds.min(), currBounds.max());

#if defined(USE_OPENCL)
	bhTree.enqueue_write(*activeDevice);

	bhTree.bind_as_parameters(gravityProgram, GRAVITY_KERNEL, 4);

	//const int blocksize = 10;
	//div_t divt = ::div(_bodyCount, blocksize);
	
// 	for(size_t idx = 0; idx < divt.quot; ++idx)
// 	{
// 		//gravityProgram.bind_parameter(GRAVITY_KERNEL, 9, (cl_int)(idx * blocksize));
// 		gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, idx * blocksize, _bodyCount, blocksize);
// 	}
// 	//gravityProgram.bind_parameter(GRAVITY_KERNEL, 9, (cl_int)(blocksize * divt.quot));
// 	if(divt.rem != 0)
// 		gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, divt.quot * blocksize, _bodyCount, divt.rem);
	//for(size_t idx = 0; idx < _bodyCount; ++idx)
	//{
	//	gravityProgram.bind_parameter(GRAVITY_KERNEL, 9, (cl_int)idx);
	//	gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, 1);
	//}
	gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, _bodyCount);

	gravityProgram.wait_all();
#else
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		program_gravity(idx, position[idx], mass[idx], bhTheta, &acceleration.get_data()[0], 
			&bhTree.nodes_children.get_data()[0], &bhTree.nodes_massCenter.get_data()[0],
			&bhTree.nodes_mass.get_data()[0], &bhTree.nodes_nnn.get_data()[0],
			&bhTree.nodes_ppp.get_data()[0]);
	}
#endif
}
