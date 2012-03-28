#include "OpenCLWrappers.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace opencl {;

template < class Ty_ >
Ty_ get_device_info(cl_device_id device, cl_device_info param_name)
{
	Ty_ val;
	cl_int rerror = ::clGetDeviceInfo(device, param_name, sizeof(Ty_), &val, NULL);
	if(rerror != CL_SUCCESS)
	{
		std::cout << "::clGetDeviceInfo error." << std::endl;
	}
	return val;
}

template < class Ty_ >
void get_device_info_presize_array(cl_device_id device, cl_device_info param_name, std::vector<Ty_>& array)
{
	cl_int rerror = ::clGetDeviceInfo(device, param_name, sizeof(Ty_) * array.size(), &array[0], NULL);
	if(rerror != CL_SUCCESS)
	{
		std::cout << "::clGetDeviceInfo error." << std::endl;
	}
}

template <>
std::string get_device_info<std::string>(cl_device_id device, cl_device_info param_name)
{
	size_t paramLen;
	cl_int rerror = ::clGetDeviceInfo(device, param_name, 0, NULL, &paramLen);
	if(rerror != CL_SUCCESS)
	{
		std::cout << "::clGetDeviceInfo error." << std::endl;
	}
	std::vector<char> tmpArray(paramLen);
	rerror = ::clGetDeviceInfo(device, param_name, paramLen, &tmpArray[0], NULL);
	if(rerror != CL_SUCCESS)
	{
		std::cout << "::clGetDeviceInfo error." << std::endl;
	}

	return std::string(&tmpArray[0]);
}

CLDevice::CLDevice(cl_device_id device_, cl_context context_, cl_command_queue queue_) 
	: device(device_), context(context_), queue(queue_)
{
	//queue				= create_command_queue()
	available			= get_device_info<bool>(device,					CL_DEVICE_AVAILABLE);
	compilerAvailable	= get_device_info<bool>(device,					CL_DEVICE_COMPILER_AVAILABLE);
	memCacheSize		= get_device_info<cl_ulong>(device,				CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
	memSize				= get_device_info<cl_ulong>(device,				CL_DEVICE_GLOBAL_MEM_SIZE);
	clockFrequency		= get_device_info<cl_uint>(device,				CL_DEVICE_MAX_CLOCK_FREQUENCY);

	computeUnits		= get_device_info<cl_uint>(device,				CL_DEVICE_MAX_COMPUTE_UNITS);
	name				= get_device_info<std::string>(device,			CL_DEVICE_NAME);
	profile				= get_device_info<std::string>(device,			CL_DEVICE_PROFILE);
	type				= get_device_info<cl_device_type>(device,		CL_DEVICE_TYPE);
	vendor				= get_device_info<std::string>(device,			CL_DEVICE_VENDOR);
	deviceVersion		= get_device_info<std::string>(device,			CL_DEVICE_VERSION);
	driverVersion		= get_device_info<std::string>(device,			CL_DRIVER_VERSION);
	doubleSupport		= get_device_info<cl_device_fp_config>(device,	CL_DEVICE_DOUBLE_FP_CONFIG);
	extensions			= get_device_info<std::string>(device,			CL_DEVICE_EXTENSIONS);
	maxWorkGroupSize	= get_device_info<size_t>(device,				CL_DEVICE_MAX_WORK_GROUP_SIZE);
	maxWorkItemDimensions = get_device_info<cl_uint>(device,			CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
	maxWorkItemSizes.resize(maxWorkItemDimensions);
	get_device_info_presize_array(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, maxWorkItemSizes);
}

void CLDevice::enqueue_barrier()
{
	::clEnqueueBarrier(queue);
}

std::string get_platform_info(cl_platform_id platform, cl_platform_info param_name)
{
	size_t paramLen;
	::clGetPlatformInfo(platform, param_name, 0, NULL, &paramLen);
	std::vector<char> tmpArray(paramLen);
	::clGetPlatformInfo(platform, param_name, paramLen, &tmpArray[0], NULL);

	return std::string(&tmpArray[0]);
}

//////////////////////////////////////////////////////////////////////////
// CLPlatform
//
CLPlatform::CLPlatform(cl_platform_id platform_) : platform(platform_)
{
	profile		= get_platform_info(platform, CL_PLATFORM_PROFILE);
	version		= get_platform_info(platform, CL_PLATFORM_VERSION);
	name		= get_platform_info(platform, CL_PLATFORM_NAME);
	vendor		= get_platform_info(platform, CL_PLATFORM_VENDOR);
	extensions	= get_platform_info(platform, CL_PLATFORM_EXTENSIONS);

	//cl_uint numDeviceIDs;
	//::clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDeviceIDs);

	//std::vector<cl_device_id> deviceIDs(numDeviceIDs);
	//::clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDeviceIDs, &deviceIDs[0], NULL);

	//for(size_t idx = 0; idx < numDeviceIDs; ++idx)
	//{
	//	devices.insert(DeviceMap::value_type(deviceIDs[idx], CLDevice(deviceIDs[idx])));
	//}
}

//////////////////////////////////////////////////////////////////////////
// CLProgram::ProgramOptions
// 

CLProgram::ProgramOptions::ProgramOptions()
{
#if defined(CL_VERSION_1_2)
	_optionsString += "-cl-kernel-arg-info ";
#endif
}

CLProgram::ProgramOptions& CLProgram::ProgramOptions::add_include_dir(const std::string& includeDir)
{
	_optionsString += "-I \"" + includeDir + "\" ";
	return *this;
}

CLProgram::ProgramOptions& CLProgram::ProgramOptions::add_macro(const std::string& name, const std::string& value)
{
	_optionsString += "-D " + name;
	if(!value.empty())
		_optionsString += "=\"" + value + "\"";
	_optionsString += " ";
	return *this;
}

CLProgram::ProgramOptions& CLProgram::ProgramOptions::debugging()
{
	_optionsString += "-cl-opt-disable -g ";
	return *this;
}

const char* CLProgram::ProgramOptions::c_str() const
{
	return _optionsString.c_str();
}

//////////////////////////////////////////////////////////////////////////
// CLProgram
// 

CLProgram::CLProgram()
	: _context(NULL),
	_program(NULL)
{
}

bool CLProgram::create(const std::string& path, const ProgramOptions& compilerOptions, const CLDevice& device)
{
	std::vector<CLDevice> devices;
	devices.push_back(device);
	return create(path, compilerOptions, devices);
}

bool CLProgram::create(const std::string& path, const ProgramOptions& compilerOptions, const std::vector<CLDevice>& devices /*= std::vector<CLDevice>()*/)
{
	std::ifstream file(path);
	if(file.fail())
	{
		std::cout << "Could not open file " << path << std::endl;
		return false;
	}
	std::vector<std::string> lines;
	while(!file.fail())
	{
		std::string line;
		std::getline(file, line);
		lines.push_back(line + "\n");
	}
	std::vector<const char*> linesChar;
	for(size_t idx = 0; idx < lines.size(); ++idx)
	{
		linesChar.push_back(lines[idx].c_str());
	}
	
	_program = ::clCreateProgramWithSource(OpenCL::get_context(), linesChar.size(), &linesChar[0], NULL, &_lastError);
	if(_lastError != CL_SUCCESS)
	{
		std::cout << "::clCreateProgramWithSource error." << std::endl;
	}
	if(_program == NULL)
		return false;

	_context = OpenCL::get_context();

	std::vector<cl_device_id> deviceIDs;
	std::for_each(devices.begin(), devices.end(), [&deviceIDs](const CLDevice& device) {
		deviceIDs.push_back(device.device);
	});

	if(deviceIDs.empty())
		_lastError = ::clBuildProgram(_program, 0, NULL, compilerOptions.c_str(), NULL, NULL);
	else
		_lastError = ::clBuildProgram(_program, deviceIDs.size(), &deviceIDs[0], compilerOptions.c_str(), NULL, NULL);

	if(_lastError != CL_SUCCESS)
	{
		std::cout << "::clBuildProgram error." << std::endl;
	}

	return _lastError == CL_SUCCESS;
}

std::string CLProgram::get_build_log(const CLDevice& device) const
{
	size_t strSize;
	_lastError = ::clGetProgramBuildInfo(_program, device.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &strSize);
	if(_lastError != CL_SUCCESS)
	{
		std::cout << "::clGetProgramBuildInfo error." << std::endl;
	}
	if(strSize == 0)
		return std::string();
	std::vector<char> log(strSize);
	_lastError = ::clGetProgramBuildInfo(_program, device.device, CL_PROGRAM_BUILD_LOG, log.size(), &log[0], NULL);
	if(_lastError != CL_SUCCESS)
	{
		std::cout << "::clGetProgramBuildInfo error." << std::endl;
	}
	return &log[0];
}

bool CLProgram::create_kernal(const std::string& kernelFnName)
{
	cl_int error = 0;
	cl_kernel kernelHandle = ::clCreateKernel(_program, kernelFnName.c_str(), &error);
	if(error != CL_SUCCESS)
	{
		std::cout << "::clCreateKernel error." << std::endl;
	}
	if(kernelHandle == NULL)
		return false;

	CLKernel kernel;
	kernel.kernel = kernelHandle;
#if defined(CL_VERSION_1_2)
	cl_uint numArgs;
	::clGetKernelInfo(kernelHandle, CL_KERNEL_NUM_ARGS, sizeof(cl_uint), &numArgs, NULL);
	for(size_t idx = 0; idx < numArgs; ++idx)
	{
		size_t nameLen = 0;
		::clGetKernelArgInfo(kernelHandle, idx, CL_KERNEL_ARG_NAME, 0, NULL, &nameLen);
		std::vector<char> name(nameLen);
		::clGetKernelArgInfo(kernelHandle, idx, CL_KERNEL_ARG_NAME, nameLen, &name[0], NULL);
		kernel.arguments[&name[0]] = idx;
	}
#endif

	_kernals[kernelFnName] = kernel;
	return true;
}

std::vector<CLProgram::CLExecutionPtr> CLProgram::enqueue_work(const CLDevice& device, const std::string& kernelFnName,
	size_t globalWorkSize)
{
	std::vector<CLProgram::CLExecutionPtr> executions;
	KernalMap::const_iterator fItr = _kernals.find(kernelFnName);
	if(fItr == _kernals.end())
		return executions;

	size_t workGroupSize = device.maxWorkItemSizes[0] - 1;
	div_t workDiv = ::div((int)globalWorkSize, (int)workGroupSize);

	size_t globalWorkOffset = 0;
	//for(int idx = 0; idx < workDiv.quot; ++idx, globalWorkOffset += workGroupSize)
	//{
	//	cl_event revent;
	//	CLExecutionPtr execution(new CLExecution(globalWorkOffset, globalWorkSize, workGroupSize));
	//	_lastError = ::clEnqueueNDRangeKernel(device.queue, fItr->second.kernel, 1, 
	//		&(execution->globalWorkOffset), &(execution->globalWorkSize), &(execution->localWorkSize),
	//		0, NULL, &(execution->event));
	//	if(_lastError != CL_SUCCESS)
	//	{
	//		std::cout << "::clEnqueueNDRangeKernel error." << std::endl;
	//		return executions;
	//	}
	//	add_event(execution);
	//	executions.push_back(execution);
	//}

	cl_event revent;
	CLExecutionPtr execution(new CLExecution(globalWorkOffset, globalWorkSize, globalWorkSize/*workDiv.rem*/));
	_lastError = ::clEnqueueNDRangeKernel(device.queue, fItr->second.kernel, 1, 
		/*&(execution->globalWorkOffset)*/NULL, &(execution->globalWorkSize), NULL/*&(execution->localWorkSize)*/,
		0, NULL, &(execution->event));
	if(_lastError != CL_SUCCESS)
	{
		std::cout << "::clEnqueueNDRangeKernel error." << std::endl;
		return executions;
	}
	add_event(execution);
	executions.push_back(execution);

	return executions;
}

bool CLProgram::clear_complete()
{
	return commands_complete();
}

bool CLProgram::commands_complete()
{
	bool complete = true;
	for(size_t idx = 0; idx < _executions.size(); ++idx)
	{
		cl_int status = 0;
		::clGetEventInfo(_executions[idx]->event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &status, NULL);
		if(status == CL_COMPLETE)
		{
			_executions.erase(_executions.begin() + idx);
			--idx;
		}
		else
		{
			complete = false;
		}
	}
	//::clWaitForEvents(_activeCommands.size(), &_activeCommands[0]);
	return complete;
}

void CLProgram::wait_all()
{
	if(!_executions.empty())
	{
		std::vector<cl_event> events;
		std::for_each(_executions.begin(), _executions.end(), [&events](const CLExecutionPtr& execution) {
			events.push_back(execution->event);
		});
		::clWaitForEvents(events.size(), &events[0]);
		_executions.clear();
	}
}

void CLProgram::add_event(const CLExecutionPtr& event)
{
	clear_complete();
	_executions.push_back(event);
}

//////////////////////////////////////////////////////////////////////////
// CLEventSet
// 
bool CLEventSet::clear_complete()
{
	return commands_complete();
}

bool CLEventSet::commands_complete()
{
	bool complete = true;
	for(size_t idx = 0; idx < _activeCommands.size(); ++idx)
	{
		cl_int status = 0;
		::clGetEventInfo(_activeCommands[idx], CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &status, NULL);
		if(status == CL_COMPLETE)
		{
			_activeCommands.erase(_activeCommands.begin() + idx);
			--idx;
		}
		else
		{
			complete = false;
		}
	}
	//::clWaitForEvents(_activeCommands.size(), &_activeCommands[0]);
	return complete;
}

void CLEventSet::wait_all()
{
	if(!_activeCommands.empty())
	{
		::clWaitForEvents(_activeCommands.size(), &_activeCommands[0]);
		_activeCommands.clear();
	}
}

void CLEventSet::add_event(cl_event event)
{
	clear_complete();
	_activeCommands.push_back(event);
}

////////////////////////////////////////////////////////////////////////////
//// CLCommandQueue
////
//CLCommandQueue CLContext::create_queue() const
//{
//	cl_int errcode = 0;
//	return CLCommandQueue(::clCreateCommandQueue(handle, CL_DEVICE_TYPE_ALL, 0, &errcode));
//}


////////////////////////////////////////////////////////////////////////////
//// OpenCL
//// 
//std::vector<CLDevice> CLPlatform::get_devices() const
//{
//	std::vector<CLDevice> devices;
//
//	for(DeviceMap::const_iterator dItr = devices.begin(); dItr != devices.end(); ++dItr)
//	{
//		deviceIDs.push_back(dItr->second);
//	}
//
//	return devices;
//}

namespace OpenCL {;

cl_context gContext = NULL;

cl_context get_context()
{
	return gContext;
}


//typedef std::map<cl_platform_id, CLPlatform> PlatformMap;
//PlatformMap gPlatforms;

//void init()
//{
//	cl_uint numPlatforms;
//	::clGetPlatformIDs(0, NULL, &numPlatforms);
//
//	std::vector<cl_platform_id> platformsIDs(numPlatforms);
//	::clGetPlatformIDs(numPlatforms, &platformsIDs[0], NULL);
//
//	for(size_t idx = 0; idx < platformsIDs.size(); ++idx)
//	{
//		gPlatforms.insert(PlatformMap::value_type(platformsIDs[idx], CLPlatform(platformsIDs[idx])));
//	}
//}

void __stdcall context_error_callback(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	std::cout << errinfo << std::endl;
}

cl_context create_context(cl_platform_id platform)
{
	//if(gPlatforms.empty())
	//	return NULL;
	//const CLPlatform& platform = gPlatforms.begin()->second;

	cl_context_properties props[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
	cl_int errcode = 0;
	
	cl_context context = ::clCreateContextFromType(props, CL_DEVICE_TYPE_ALL, context_error_callback, NULL, &errcode);
	if(errcode != CL_SUCCESS)
	{
		std::cout << "::clCreateContextFromType error." << std::endl;
		return NULL;
	}
	return context;
}

std::vector<cl_platform_id> get_platform_ids()
{
	cl_uint numPlatforms;
	::clGetPlatformIDs(0, NULL, &numPlatforms);
	std::vector<cl_platform_id> platformIDs(numPlatforms);
	::clGetPlatformIDs(numPlatforms, &platformIDs[0], NULL);
	return platformIDs;
	//std::vector<cl_platform_id> platformIDs;
	//for(PlatformMap::const_iterator pItr = gPlatforms.begin(); pItr != gPlatforms.end(); ++pItr)
	//{
	//	platformIDs.push_back(pItr->first);
	//}
	//return platformIDs;
}

//
//{
//	PlatformMap::const_iterator fItr = gPlatforms.find(platformID);
//	if(fItr == gPlatforms.end())
//		return std::vector<cl_device_id>();
//
//	const CLPlatform& platform = fItr->second;
//
//	std::vector<CLDevice> deviceIDs;
//	for(CLPlatform::DeviceMap::const_iterator dItr = platform.devices.begin(); dItr != platform.devices.end(); ++dItr)
//	{
//		deviceIDs.push_back(CLDevice(dItr->first, );
//	}
//	return deviceIDs;
//}


std::vector<cl_device_id> get_device_ids(cl_platform_id platformID)
{
	cl_uint numDeviceIDs;
	::clGetDeviceIDs(platformID, CL_DEVICE_TYPE_ALL, 0, NULL, &numDeviceIDs);

	std::vector<cl_device_id> deviceIDs(numDeviceIDs);
	::clGetDeviceIDs(platformID, CL_DEVICE_TYPE_ALL, numDeviceIDs, &deviceIDs[0], NULL);

	return deviceIDs;
}

cl_command_queue create_command_queue(cl_context context, cl_device_id deviceID)
{
	return ::clCreateCommandQueue(context, deviceID, 0, NULL);
}

std::vector<CLDevice> get_all_devices()
{
	std::vector<cl_platform_id> platforms = get_platform_ids();

	std::vector<CLDevice> devices;

	if(platforms.size() == 1)
	{
		gContext = create_context(platforms[0]);
		std::vector<cl_device_id> platformDeviceIDs = get_device_ids(platforms[0]);
		for(size_t idx2 = 0; idx2 < platformDeviceIDs.size(); ++idx2)
		{
			cl_command_queue commandQueue = create_command_queue(gContext, platformDeviceIDs[idx2]);
			devices.push_back(CLDevice(platformDeviceIDs[idx2], gContext, commandQueue));
		}
	}
	return devices;	
}

}

}