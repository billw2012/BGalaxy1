#ifndef GalaxyTypes_h__
#define GalaxyTypes_h__

#include "math/vector3.hpp"
#include "CL/opencl.h"

//#include <boost/mpl/assert.hpp>
//#include <boost/mpl/or.hpp>
//#include <boost/mpl/logical.hpp>
//#include <boost/type_traits.hpp>

#if defined(GALAXY_DOUBLE_PRECISION)
typedef cl_double  cl_real;
typedef cl_double2 cl_real2;
typedef cl_double3 cl_real3;
typedef cl_double4 cl_real4;
typedef double pf_real;
#else
typedef cl_float  cl_real;
typedef cl_float2 cl_real2;
typedef cl_float3 cl_real3;
typedef cl_float4 cl_real4;
typedef float pf_real;
#endif
typedef math::Vector3 < pf_real > pf_Vector3;

#endif // GalaxyTypes_h__
