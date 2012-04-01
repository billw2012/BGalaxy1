#ifndef OpenCLWrappers_h__
#define OpenCLWrappers_h__

#include "CL/opencl.h"

#include "Utils/stl_aligned_allocator.h"

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "math/vector3.hpp"

//#include <boost/mpl/assert.hpp>
//#include <boost/mpl/or.hpp>
//#include <boost/mpl/logical.hpp>
//#include <boost/type_traits.hpp>

namespace opencl {;

struct CLDevice
{
	CLDevice() {}
	CLDevice(cl_device_id device_, cl_context context_, cl_command_queue queue_);

	void flush() const { ::clFlush(queue); }

	void enqueue_barrier();

	cl_context context;
	cl_command_queue queue;
	cl_device_id device;

	bool available;
	bool compilerAvailable;
	cl_ulong memCacheSize;
	cl_ulong memSize;
	cl_uint clockFrequency;
	cl_uint computeUnits;
	std::string name;
	std::string profile;
	cl_device_type type;
	std::string vendor;
	std::string deviceVersion;
	std::string driverVersion;
	cl_device_fp_config doubleSupport;
	std::string extensions;
	size_t maxWorkGroupSize;
	cl_uint maxWorkItemDimensions;
	std::vector<size_t> maxWorkItemSizes;
	cl_uint maxSubDevices;
	std::vector<cl_device_partition_property_ext> subDevicePartitionProperties;
};

struct CLPlatform
{
	CLPlatform() {}

	CLPlatform(cl_platform_id platform_);

	//std::vector<CLDevice> get_devices() const;

	cl_platform_id platform;
	std::string profile, version, name, vendor, extensions;

	typedef std::map<cl_device_id, CLDevice> DeviceMap;
	DeviceMap devices;
};

struct CLPlainEvent
{
	CLPlainEvent(cl_event event_) : event(event_) {}

	cl_event event;
	cl_event get_event() const { return event; }
};

struct CLEventSet
{
	virtual bool clear_complete();
	virtual bool commands_complete();
	virtual void wait_all();

protected:

	void add_event(cl_event event);

private:

	std::vector<cl_event> _activeCommands;
};


template < class ValTy_ >
struct OpenCLBuffer : public CLEventSet
{
	typedef ValTy_ value_type;

	typedef std::vector< value_type, aligned_allocator<value_type, 8> > vector_type;

	struct Flags { enum type {
		KERNEL_READ			= 1 << 0,
		KERNEL_WRITE		= 1 << 1,
		KERNEL_READ_WRITE	= KERNEL_READ + KERNEL_WRITE
	};};

	OpenCLBuffer();
	~OpenCLBuffer();

	bool init(typename Flags::type flags);

	bool resize(size_t elements);
	void destroy();

	cl_event enqueue_read(const CLDevice& device, bool blocking = false);
	cl_event enqueue_read(const CLDevice& device, bool blocking, size_t elementOffset,
		size_t elementCount);
	cl_event enqueue_write(const CLDevice& device, bool blocking = false);
	cl_event enqueue_write(const CLDevice& device, bool blocking, size_t elementOffset,
		size_t elementCount);

	const value_type& operator[](size_t idx) const;
	value_type& operator[](size_t idx);
	const vector_type& get_data() const;
	vector_type& get_data();
	size_t size() const;

	template < class Ty_ >
	const Ty_& get_as(size_t idx) const;
	template < class Ty_ >
	Ty_& get_as(size_t idx);

	cl_mem get_handle() const;


private:

	cl_mem_flags interpret_flags(typename Flags::type flags);

	cl_mem _handle;
	cl_context _context;
	cl_mem_flags _flags;
	cl_int _lastError;
	vector_type _data;
};

struct CLProgram : public CLEventSet
{
	struct ProgramOptions
	{
		ProgramOptions();
		ProgramOptions& add_include_dir(const std::string& includeDir);
		ProgramOptions& add_macro(const std::string& name, const std::string& value);
		ProgramOptions& debugging();

		const char* c_str() const;
	private:
		std::string _optionsString;
	};
	CLProgram();

	bool create(const std::string& path, const ProgramOptions& compilerOptions, const CLDevice& device);
	bool create(const std::string& path, const ProgramOptions& compilerOptions, const std::vector<CLDevice>& devices = std::vector<CLDevice>());

	std::string get_build_log(const CLDevice& device) const;

	bool create_kernal(const std::string& kernelFnName);
#if defined(CL_VERSION_1_2)
	template < class ValTy_ > 
	bool bind_parameter(const std::string& kernelFnName, const std::string& paramName, const ValTy_& val);
	template < class ValTy_ > 
	bool bind_parameter(const std::string& kernelFnName, const OpenCLBuffer<ValTy_>& buffer);
#endif
	template < class ValTy_ > 
	bool bind_parameter(const std::string& kernelFnName, cl_uint paramIdx, const ValTy_& val);
	template < class ValTy_ > 
	bool bind_parameter(const std::string& kernelFnName, cl_uint paramIdx, const OpenCLBuffer<ValTy_>& buffer);


	struct CLExecution
	{
		CLExecution(size_t globalWorkOffset_, size_t globalWorkSize_, size_t localWorkSize_)
			: globalWorkOffset(globalWorkOffset_), globalWorkSize(globalWorkSize_), localWorkSize(localWorkSize_) {}
		cl_event event;
		size_t globalWorkOffset; 
		size_t globalWorkSize; 
		size_t localWorkSize; 
	};

	typedef std::shared_ptr<CLExecution> CLExecutionPtr;

	std::vector<CLExecutionPtr> enqueue_work(const CLDevice& device, const std::string& kernelFnName, size_t globalWorkSize, size_t localWorkSize = 64);

	virtual bool clear_complete();
	virtual bool commands_complete();
	virtual void wait_all();

protected:

	void add_event(const CLExecutionPtr& event);

private:

	cl_context _context;
	cl_program _program;
	mutable cl_int _lastError;

	struct CLKernel
	{
		typedef std::map<std::string, cl_uint> ArgumentMap;
		ArgumentMap arguments;
		cl_kernel kernel;
	};

	typedef std::map<std::string, CLKernel> KernalMap;
	KernalMap _kernals;	

	std::vector<CLExecutionPtr> _executions;
};


//struct CLCommandQueue
//{
//	CLCommandQueue() {}
//	CLCommandQueue(cl_command_queue handle_) : handle(handle_) {}
//
//	cl_command_queue handle;
//};

//struct CLContext
//{
//	CLContext() {}
//	CLContext(cl_context handle_) : handle(handle_) {}
//
//	bool valid() const { return handle != NULL; }
//
//	CLCommandQueue create_queue() const;
//
//	cl_context handle;
//};

namespace OpenCL {;
//void init();
cl_context get_context();

//cl_context create_context();

//std::vector<cl_platform_id> get_platform_ids();
//std::vector<cl_device_id> get_device_ids(cl_platform_id platformID);

//cl_command_queue create_command_queue(cl_context context, cl_device_id deviceID);

std::vector<CLDevice> get_all_devices();

//cl_mem create_buffer(cl_context context, cl_mem_flags flags, size_t size, void * host_ptr, 
//	cl_int *errcode_ret ) );
}

//
//struct OpenCL
//{
//	//typedef cl_context ContextHandle;
//
//	void init();
//
//	CLContext create_context();
//
//private:
//
//	typedef std::map<cl_platform_id, CLPlatform> PlatformMap;
//	PlatformMap platforms;
//};

}

template < class ValTy_ >
typename opencl::OpenCLBuffer<ValTy_>::Flags::type operator|(const typename opencl::OpenCLBuffer<ValTy_>::Flags::type& lhs, const typename opencl::OpenCLBuffer<ValTy_>::Flags::type& rhs)
{
	return static_cast<typename opencl::OpenCLBuffer<ValTy_>::Flags::type>(static_cast<size_t>(lhs) | static_cast<size_t>(rhs));
}

inline cl_double3 convert_my_vec3(const math::Vector3d& myVec)
{
	cl_double3 tv = {myVec.x, myVec.y, myVec.z, 0.0};
	//for(size_t idx = 0; idx < 3; ++idx)
	//	tv.s[idx] = myVec[idx];
	return tv;
}

inline math::Vector3d convert_to_my_vec3(const cl_double3& vec)
{
	return math::Vector3d(vec.s[0], vec.s[1], vec.s[2]);
}



#include "OpenCLWrappers.inl"


#endif // OpenCLWrappers_h__