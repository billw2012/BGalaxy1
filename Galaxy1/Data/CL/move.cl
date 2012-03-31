#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void move (const double t, __global double3* pos, 
	__global double3* velocity, __global const double3* accelaration,
	const int totalWorkSize)
{
	int iGID = get_global_id(0);
	if(iGID >= totalWorkSize)
		return ;

	velocity[iGID] += t * accelaration[iGID];
	pos[iGID] += t * velocity[iGID];
}