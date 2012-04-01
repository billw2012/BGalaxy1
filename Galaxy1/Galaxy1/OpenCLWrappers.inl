
namespace opencl {;
//////////////////////////////////////////////////////////////////////////
// OpenCLBuffer
//

template < class ValTy_ >
OpenCLBuffer<ValTy_>::OpenCLBuffer()
	: _context(NULL),
	_flags(0),
	_handle(NULL),
	_lastError(NULL)
{

}

template < class ValTy_ >
bool OpenCLBuffer<ValTy_>::init(typename Flags::type flags)
{
	_context = OpenCL::get_context();
	_flags = interpret_flags(flags);
	return _context != NULL;
}

template < class ValTy_ >
OpenCLBuffer<ValTy_>::~OpenCLBuffer()
{
	destroy();
}

template < class ValTy_ >
bool OpenCLBuffer<ValTy_>::resize(size_t elements)
{
	if(_handle != NULL)
	{
		wait_all();
		::clReleaseMemObject(_handle);
		_handle = NULL;
	}
	_data.resize(elements);
	_handle = ::clCreateBuffer(_context, _flags, sizeof(value_type) * elements, NULL, &_lastError);
	return _handle != NULL;
}

template < class ValTy_ >
cl_event OpenCLBuffer<ValTy_>::enqueue_read(const CLDevice& device_, bool blocking)
{
	return enqueue_read(device_, blocking, 0, _data.size());
}

template < class ValTy_ >
cl_event OpenCLBuffer<ValTy_>::enqueue_read(const CLDevice& device_, bool blocking, size_t elementOffset,
	size_t elementCount)
{
	cl_event event = NULL;
	_lastError = ::clEnqueueReadBuffer(device_.queue, _handle, blocking? CL_TRUE : CL_FALSE, 
		elementOffset * sizeof(value_type), elementCount * sizeof(value_type), &_data[elementOffset], 0, NULL, &event);
	if(_lastError != CL_SUCCESS)
		return NULL;
	add_event(event);
	return event;
}

template < class ValTy_ >
cl_event OpenCLBuffer<ValTy_>::enqueue_write(const CLDevice& device_, bool blocking)
{
	return enqueue_write(device_, blocking, 0, _data.size());
}

template < class ValTy_ >
cl_event OpenCLBuffer<ValTy_>::enqueue_write(const CLDevice& device_, bool blocking, size_t elementOffset,
	size_t elementCount)
{
	cl_event event = NULL;
	_lastError = ::clEnqueueWriteBuffer(device_.queue, _handle, blocking? CL_TRUE : CL_FALSE, 
		elementOffset * sizeof(value_type), elementCount * sizeof(value_type), &_data[elementOffset], 0, NULL, &event);
	if(_lastError != CL_SUCCESS)
		return NULL;
	add_event(event);
	return event;
}

template < class ValTy_ >
const typename OpenCLBuffer<ValTy_>::value_type& OpenCLBuffer<ValTy_>::operator[](size_t idx) const
{
	wait_all();
	return _data[idx];
}

template < class ValTy_ >
typename OpenCLBuffer<ValTy_>::value_type& OpenCLBuffer<ValTy_>::operator[](size_t idx)
{
	wait_all();
	return _data[idx];
}

template < class ValTy_ >
const typename OpenCLBuffer<ValTy_>::vector_type& OpenCLBuffer<ValTy_>::get_data() const
{
	wait_all();
	return _data;
}

template < class ValTy_ >
typename OpenCLBuffer<ValTy_>::vector_type& OpenCLBuffer<ValTy_>::get_data()
{
	wait_all();
	return _data;
}

template < class ValTy_ >
size_t OpenCLBuffer<ValTy_>::size() const
{
	return _data.size();
}

template < class ValTy_ >
template < class Ty_ >
const Ty_& OpenCLBuffer<ValTy_>::get_as(size_t idx) const
{
	return *(const Ty_*)&(*this)[idx];
}

template < class ValTy_ >
template < class Ty_ >
Ty_& OpenCLBuffer<ValTy_>::get_as(size_t idx)
{
	return *(Ty_*)&(*this)[idx];
}

template < class ValTy_ >
cl_mem OpenCLBuffer<ValTy_>::get_handle() const
{
	return _handle;
}

template < class ValTy_ >
void OpenCLBuffer<ValTy_>::destroy()
{
	if(_handle != NULL)
	{
		wait_all();
		::clReleaseMemObject(_handle);
		_data.clear();
		_handle = NULL;
	}
}

template < class ValTy_ >
cl_mem_flags OpenCLBuffer<ValTy_>::interpret_flags(typename Flags::type flags)
{
	if(flags == Flags::KERNEL_READ)
		return CL_MEM_READ_ONLY /*| CL_MEM_HOST_READ_ONLY*/;
	if(flags == Flags::KERNEL_WRITE)
		return CL_MEM_WRITE_ONLY /*| CL_MEM_HOST_READ_ONLY*/;
	//if(flags == Flags::READ | Flags::WRITE)
	// default to read write
	return CL_MEM_READ_WRITE /*| CL_MEM_HOST_READ_ONLY*/;

}

#if defined(CL_VERSION_1_2)
template < class ValTy_ > 
bool CLProgram::bind_parameter<ValTy_>(const std::string& kernelFnName, const std::string& paramName, const ValTy_& val)
{

}

template < class ValTy_ > 
bool CLProgram::bind_parameter<ValTy_>(const std::string& kernelFnName, const OpenCLBuffer<ValTy_>& buffer)
{

}
#endif

template < class ValTy_ > 
bool CLProgram::bind_parameter(const std::string& kernelFnName, cl_uint paramIdx, const ValTy_& val)
{
	KernalMap::const_iterator fItr = _kernals.find(kernelFnName);
	if(fItr == _kernals.end())
		return false;
	_lastError = ::clSetKernelArg(fItr->second.kernel, paramIdx, sizeof(ValTy_), &val);
	return _lastError == CL_SUCCESS;
}

template < class ValTy_ > 
bool CLProgram::bind_parameter(const std::string& kernelFnName, cl_uint paramIdx, const OpenCLBuffer<ValTy_>& buffer)
{
	return bind_parameter(kernelFnName, paramIdx, buffer.get_handle());
}

};