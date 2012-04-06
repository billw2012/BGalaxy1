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
	const int totalWorkSize,
	int currIdx
	)
{
	if(currIdx >= totalWorkSize)
		return;

	REAL3 currPos = positons[currIdx];
	REAL currMass = mass[currIdx];

	for(int idx = 0; idx < 
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