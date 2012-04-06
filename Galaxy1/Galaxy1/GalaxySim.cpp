
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

#define DEFAULT_DEVICE_INDEX 0
#define EXPORT_IMAGES
#define EXPORT_FRAME_INTERVAL 1

#define USE_OPENCL

#define WORKGROUP_SIZE 128

GalaxySim::GalaxySim()
	:
	_error(false),
	_bodyCount(0),
	bhTheta(0.0), _currBodyOffset(0),
	_iteration(0)
{

}

bool GalaxySim::load_kernel( CLProgram& program, const std::string& file, const std::string& kernel )
{
	CLProgram::ProgramOptions options;
#if defined(GALAXY_DOUBLE_PRECISION)
	options.add_macro("GALAXY_DOUBLE_PRECISION");
#endif
	bool buildSuccess = program.create(file, 
#if defined(KERNEL_DEBUGGING)
		options.debugging(), *activeDevice
#else
		options
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

void GalaxySim::initialize_galaxy(const pf_Vector3& center, const pf_Vector3& velocity, size_t bodies, pf_real minMass, pf_real maxMass, pf_real space)
{
	initialize_bodies(center, velocity, bodies, minMass, maxMass, space, 0.0, SimType::Galaxy);
}

void GalaxySim::initialize_universe(const pf_Vector3& center, size_t bodies, pf_real minMass, pf_real maxMass, 
	pf_real size, pf_real initVel)
{
	initialize_bodies(center, pf_Vector3::Zero, bodies, minMass, maxMass, size, initVel, SimType::Universe);
}

#define PI_VAL 3.14159265

#include "galaxy_c.h"

void GalaxySim::initialize_bodies(const pf_Vector3& center, const pf_Vector3& velocity, size_t bodies, pf_real minMass, pf_real maxMass, pf_real size, 
	pf_real initVel, SimType::type simType)
{
	boost::random::taus88 rng;
	boost::random::uniform_real_distribution<pf_real> massRange(minMass, maxMass);
	boost::random::uniform_real_distribution<pf_real> angleRange(0.0, 2 * PI_VAL);
	boost::random::uniform_real_distribution<pf_real> zrRange(-1.0, 1.0);
	boost::random::exponential_distribution<pf_real> posRange(1.2);
	using namespace math;
	Matrix3d rot(
		0,  1,  1, 
		-1, 0,  1,
		0,  0,  1);

	resize(_currBodyOffset + bodies);

	bhTheta = 3.0;

#if defined(USE_OPENCL)
	gravityProgram.bind_parameter(GRAVITY_KERNEL, 0, (cl_real)bhTheta);
#endif

	for(size_t idx = _currBodyOffset; idx < bodies + _currBodyOffset; ++idx)
	{
		_mass[idx] = massRange(rng);
		cl_real4& currPos = _position[idx];
		cl_real4& currVel = _velocity[idx];

		if(simType == SimType::Galaxy)
		{
			pf_Vector3 currPosT;

			do 
			{
				pf_real distance = posRange(rng) * size * 0.2;
				pf_real angle = angleRange(rng);
				pf_real x = sin(angle);
				pf_real y = cos(angle);
				currPosT = pf_Vector3(x * distance, y * distance, zrRange(rng) * size * 0.01);
			} while (currPosT.length() > size);
			currPos = convert_my_vec3(currPosT + center);
			currVel = convert_my_vec3(pf_Vector3::Zero);
		}
		else
		{
			pf_Vector3 rpos(zrRange(rng), zrRange(rng), zrRange(rng));
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

		iterate_gravity();

#if defined(USE_OPENCL)
		_acceleration.enqueue_read(*activeDevice, true);
#endif

		// determine acceleration for objects
		for(size_t idx = _currBodyOffset; idx < bodies + _currBodyOffset; ++idx)
		{
			// escape velocity = - sqrt(2 * acceleration_due_to_gravity * distance_from_gravitational_center)
			pf_Vector3 currAccelT(convert_to_my_vec3(_acceleration[idx]));
			pf_Vector3 currPosT(convert_to_my_vec3(_position[idx]));
			pf_real initialStability = 1.0; // 1.0 for perfect stability, lower for collapse, higher for explosion
			pf_real escapeVel = - std::sqrt(initialStability * currAccelT.length() * (currPosT - center).length());
			pf_Vector3 newVelT = rot * currAccelT.normal() * escapeVel;
			_velocity[idx] = convert_my_vec3(newVelT + velocity);
		}
#if defined(USE_OPENCL)
		_velocity.enqueue_write(*activeDevice, true);
#endif
	}

	_currBodyOffset += bodies;
}

void GalaxySim::iterate( pf_real t )
{
	iterate_gravity();
	iterate_move(t);
	output_image();
	output_data();

	std::cout << "Iteration " << _iteration << ",  bh time = " << _lastBHTreeCalcTime << "ms, grav time = " << _lastCalcGravityTime << "ms, move time = " << _lastCalcMoveTime << "ms" << std::endl;
	++ _iteration;
}

void GalaxySim::iterate_gravity()
{
	_timer.reset();
	bhTree.build(_position.get_data(), _mass.get_data(), currBounds.min(), currBounds.max());
	_lastBHTreeCalcTime = _timer.get_time();

	_timer.reset();
#if defined(USE_OPENCL)
	bhTree.enqueue_write(*activeDevice);
	bhTree.bind_as_parameters(gravityProgram, GRAVITY_KERNEL, 4);
	gravityProgram.enqueue_work(*activeDevice, GRAVITY_KERNEL, _bodyCount, WORKGROUP_SIZE);
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
	_lastCalcGravityTime = _timer.get_time();
}

void GalaxySim::iterate_move( pf_real t )
{
	_timer.reset();
#if defined(USE_OPENCL)
	moveProgram.bind_parameter(MOVE_KERNEL, 0, (cl_real)t);
	moveProgram.enqueue_work(*activeDevice, MOVE_KERNEL, _bodyCount, WORKGROUP_SIZE);
	_position.enqueue_read(*activeDevice, true);
#else
	for(size_t idx = 0; idx < _bodyCount; ++idx)
	{
		program_move(idx, t, &_position.get_data()[0], &velocity.get_data()[0], 
			&_acceleration.get_data()[0]);
	}
#endif
	_lastCalcMoveTime = _timer.get_time();
}

void GalaxySim::output_image()
{
	const OpenCLBufferD3::vector_type& positionVals = _position.get_data();
	currBounds.reset();
	for(size_t idx = 0; idx < positionVals.size(); ++idx)
	{
		currBounds.expand(convert_to_my_vec3(positionVals[idx]));
	}

	emit new_data_available();
}

void GalaxySim::output_data()
{
	boost::mutex::scoped_lock scopedLock(_dataMutex);
	_data.clear();
	const OpenCLBufferD3::vector_type& positionVals = _position.get_data();
	for(size_t idx = 0; idx < positionVals.size(); ++idx)
	{
		_data.push_back(pf_Vector3(positionVals[idx].s[0], positionVals[idx].s[1], positionVals[idx].s[2]));
	}
}

void GalaxySim::lock_data() const
{
	_dataMutex.lock();
}

const std::vector< pf_Vector3 >& GalaxySim::get_data() const
{
	return _data;
}

void GalaxySim::unlock_data() const
{
	_dataMutex.unlock();
}

void GalaxySim::reset()
{
	resize(0); 
	_currBodyOffset = 0; 
	_iteration = 0;
}
