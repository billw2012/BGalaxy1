#pragma OPENCL EXTENSION cl_amd_fp64 : enable

double distance_to_aabb(const double3 nnn, const double3 ppp, const double3 pos)
{
	double3 closest = clamp(pos, nnn, ppp);
	return distance(pos.xyz, closest);
}

double bh_calc_s_over_d(const double3 nnn, const double3 ppp, const double3 pos)
{
	double distToNode = distance_to_aabb(nnn, ppp, pos);
	double regionSize = distance(nnn, ppp);
	return regionSize / distToNode;
}

// [length3] [mass-1] [time-2]
// gravitational constant = 
// 1.56092357 × 10-13 (light years cubed) over (solar masses (years squared))
// g units:
//__constant double g = 0.000000000000156;
//__constant double g = 0.000000000000156;
// 39.4775743 (au cubed) over (solar masses (years squared))
//__constant double g = 39.4775743;
// 0.000118580398 (au cubed) over (earth masses (years squared))
__constant double g = 0.000118580398;

double3 calculate_acceleration(const double3 p0, const double mass0, const double3 p1, const double mass1)
{
	double3 v = p1 - p0;
	double len = length(v);
	if(len <= 0.0001)
		return 0.0;
	return v * (g * mass0 * mass1 / (len * len * mass0));
}

__kernel void gravity (
	const double bhTheta, 
	__global const double3* positons, 
	__global const double* mass, 
	__global double3* accelaration, 
	__global const int* BHNode_children, 
	__global const double3* BHNode_massCenter, 
	__global const double* BHNode_mass, 
	__global const double3* BHNode_nnn, 
	__global const double3* BHNode_ppp,
	const int totalWorkSize
	)
{
	int currIdx = get_global_id(0);// + currIdxOffs;
	if(currIdx >= totalWorkSize)
		return;

	double3 currPos = positons[currIdx];
	double currMass = mass[currIdx];

	int stack[2048];
	stack[0] = 0;
	int stackIdx = 1;

	double3 accelerationAccum = 0.0;
	
	while(stackIdx > 0)
	{
		--stackIdx;
		int currNodeIdx = stack[stackIdx];
		__global const int*	currNode_children	= &BHNode_children	[currNodeIdx * 8];
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
				if(stackIdx >= 2048)
				{
					accelaration[currIdx] = 1000000000.0;
					return;
				}
			}
		}
	}

	accelaration[currIdx] = accelerationAccum;
}