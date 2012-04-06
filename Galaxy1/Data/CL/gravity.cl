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

REAL distance_to_aabb(const REAL3 nnn, const REAL3 ppp, const REAL3 pos)
{
	REAL3 closest = clamp(pos, nnn, ppp);
	return distance(pos.xyz, closest);
}

REAL bh_calc_s_over_d(const REAL3 nnn, const REAL3 ppp, const REAL3 pos)
{
	REAL distToNode = distance_to_aabb(nnn, ppp, pos);
	REAL regionSize = distance(nnn, ppp);
	return regionSize / distToNode;
}

// [length3] [mass-1] [time-2]
// gravitational constant = 
// 1.56092357 × 10-13 (light years cubed) over (solar masses (years squared))
// g units:
//__constant REAL g = 0.000000000000156;
//__constant REAL g = 0.000000000000156;
// 39.4775743 (au cubed) over (solar masses (years squared))
//__constant REAL g = 39.4775743;
// 0.000118580398 (au cubed) over (earth masses (years squared))
//__constant REAL g = 0.000118580398;
// 1.56092357 × 10-7 gravitational constant in light years cubed over solar masses millenium squared
__constant REAL g = 0.000000156092357f;

REAL3 calculate_acceleration(const REAL3 p0, const REAL mass0, const REAL3 p1, const REAL mass1)
{
	REAL3 v = p1 - p0;
	REAL len = length(v);
	if(len <= 0.000001f)
		return 0.0f;
	return v * (g * mass0 * mass1 / (len * len * mass0));
}

#if 0

__kernel void gravity (
	const REAL bhTheta, 
	__global const REAL3* positons, 
	__global const REAL* mass, 
	__global REAL3* accelaration, 
	__global const int* BHNode_children, 
	__global const REAL3* BHNode_massCenter, 
	__global const REAL* BHNode_mass, 
	__global const REAL3* BHNode_nnn, 
	__global const REAL3* BHNode_ppp,
	const unsigned int totalWorkSize
	)
{
	int currIdx = get_global_id(0);// + currIdxOffs;
	if(currIdx >= totalWorkSize)
		return;

	REAL3 currPos = positons[currIdx];
	REAL currMass = mass[currIdx];

	int stack[1024];
	stack[0] = 0;
	int stackIdx = 1;

	REAL3 accelerationAccum = 0.0f;
	
	while(stackIdx > 0)
	{
		--stackIdx;
		int currNodeIdx = stack[stackIdx];
		__global const int*	currNode_children = &BHNode_children [currNodeIdx * 8];
		if(currNode_children[0] == -1 || 
			bh_calc_s_over_d(BHNode_nnn[currNodeIdx], BHNode_ppp[currNodeIdx], currPos) < bhTheta)
		{
			accelerationAccum += calculate_acceleration(currPos, currMass, BHNode_massCenter[currNodeIdx], BHNode_mass[currNodeIdx]);
		}
		else
		{
			for(int child = 0; child < 8 && currNode_children[child] != -1; ++child)
			{
				stack[stackIdx] = currNode_children[child];
				++stackIdx;
				if(stackIdx >= 1024)
				{
					accelaration[currIdx] = 10000000.0f;
					return;
				}
			}
		}
	}

	accelaration[currIdx] = accelerationAccum;
}

#else
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_extended_atomics : enable

__kernel void gravity (
	const REAL bhTheta, 
	__global const REAL3* positons, 
	__global const REAL* mass, 
	__global REAL3* accelaration, 
	__global const int* BHNode_children, 
	__global const REAL3* BHNode_massCenter, 
	__global const REAL* BHNode_mass, 
	__global const REAL3* BHNode_nnn, 
	__global const REAL3* BHNode_ppp,
	const unsigned int totalWorkSize
	)
{
	size_t localID = get_local_id(0);
	size_t localWorkSize = get_local_size(0);
	size_t groupID = get_group_id(0);
	volatile __local int stack[2048];
	volatile __local int stackIdx;
	__local REAL3 accelerationAccum[1024];

	size_t startItemIdx = groupID * localWorkSize;
	size_t endItemIdx = min(totalWorkSize, (groupID + 1) * localWorkSize);
	for(size_t currIdx = startItemIdx; currIdx < endItemIdx; ++currIdx)
	{
		REAL3 currPos = positons[currIdx];
		REAL currMass = mass[currIdx];
		accelerationAccum[localID] = 0.0f;
		if(localID == 0)
		{
			stack[0] = 0;
			stackIdx = 1;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		// make sure stack is available
		while(stackIdx > 0)
		{
			barrier(CLK_LOCAL_MEM_FENCE);
			int localStackIdx = atomic_dec(&stackIdx) - 1;
			atomic_max(&stackIdx, 0);
			if(localStackIdx >= 0)
			{
				int currNodeIdx = stack[localStackIdx];
				__global const int*	currNode_children = &BHNode_children [currNodeIdx * 8];
				if(currNode_children[0] == -1 || 
					bh_calc_s_over_d(BHNode_nnn[currNodeIdx], BHNode_ppp[currNodeIdx], currPos) < bhTheta)
				{
					accelerationAccum[localID] += calculate_acceleration(currPos, currMass, BHNode_massCenter[currNodeIdx], BHNode_mass[currNodeIdx]);
				}
				else
				{
					for(int child = 0; child < 8 && currNode_children[child] != -1; ++child)
					{
						int childStackIdx = atomic_inc(&stackIdx);
						stack[childStackIdx] = currNode_children[child];
					}
				}
			}
			barrier(CLK_LOCAL_MEM_FENCE);
		}
		if(localID == 0)
		{
			accelaration[currIdx] = 0.0f;
			for(size_t accelIdx = 0; accelIdx < localWorkSize; ++accelIdx)
				accelaration[currIdx] += accelerationAccum[accelIdx];
		}
	}
}

#endif