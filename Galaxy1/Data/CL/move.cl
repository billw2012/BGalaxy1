#ifdef GALAXY_DOUBLE_PRECISION
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define REAL  double
#define REAL2 double2
#define REAL3 double3
#define REAL4 double4
#else
#define REAL  float
#define REAL2 float2
#define REAL3 float3
#define REAL4 float4
#endif

__kernel void move (const REAL t, __global REAL3* pos, 
	__global REAL3* velocity, __global const REAL3* accelaration, const int totalWorkSize)
{
	int iGID = get_global_id(0);
	if(iGID >= totalWorkSize)
		return ;

	velocity[iGID] += t * accelaration[iGID];
	pos[iGID] += t * velocity[iGID];
}