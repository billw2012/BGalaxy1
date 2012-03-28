#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void move (const double t, __global double3* pos, 
	__global double3* velocity, __global const double3* accelaration)
{
	int iGID = get_global_id(0);

	velocity[iGID] += t * accelaration[iGID];
	pos[iGID] += t * velocity[iGID];
}