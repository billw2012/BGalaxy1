
// [length3] [mass-1] [time-2]
// gravitational constant = 
// 1.56092357 × 10-13 (light years cubed) over (solar masses (years squared))
// g units:
//__constant double g = 0.000000000000156;
//__constant double g = 0.000000000000156;
// 39.4775743 (au cubed) over (solar masses (years squared))
static const double g = 39.4775743;

cl_double3 clamp(const cl_double3& pos, const cl_double3& nnn, const cl_double3& ppp)
{
	cl_double3 ret;
	for(size_t idx = 0; idx < 3; ++idx)
		ret.s[idx] = std::min(ppp.s[idx], std::max(pos.s[idx], nnn.s[idx]));
	return ret;
}

inline cl_double3 operator-(const cl_double3& lhs, const cl_double3& rhs)
{
	cl_double3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] - rhs.s[idx];
	return r;
}

inline cl_double3 operator*(const cl_double3& lhs, const cl_double& rhs)
{
	cl_double3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] * rhs;
	return r;
}

inline cl_double3 operator+(const cl_double3& lhs, const cl_double3& rhs)
{
	cl_double3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] + rhs.s[idx];
	return r;
}

inline cl_double length(const cl_double3& v)
{
	return sqrt(v.s[0] * v.s[0] + v.s[1] * v.s[1] + v.s[2] * v.s[2]);
}

cl_double distance(const cl_double3& p0, const cl_double3& p1)
{
	return length(p1 - p0);
}

cl_double distance_to_aabb(const cl_double3 nnn, const cl_double3 ppp, const cl_double3 pos)
{
	cl_double3 closest = clamp(pos, nnn, ppp);
	return distance(pos, closest);
}


cl_double3 calculate_acceleration(const cl_double3 p0, const cl_double mass0, const cl_double3 p1, const cl_double mass1)
{
	static cl_double3 zero = {0.0, 0.0, 0.0, 0.0};
	cl_double3 v = p1 - p0;
	cl_double len = length(v);
	if(len < 0.1)
		return zero;
	return v * (0.000118580398 * mass1 * mass0 / (pow(length(v), 2) * mass1));
}

void program_gravity (
	cl_int currIdx,
	const cl_double3 currPos, 
	const cl_double currMass, 
	const cl_double minDistance, 
	cl_double3* accelaration, 
	const cl_int* BHNode_children, 
	const cl_double3* BHNode_massCenter, 
	const cl_double* BHNode_mass, 
	const cl_double3* BHNode_nnn, 
	const cl_double3* BHNode_ppp

	)
{
	int stack[2048];
	stack[0] = 0;
	int stackIdx = 1;

	cl_double3 accelerationAccum = {0.0, 0.0, 0.0};

	while(stackIdx > 0)
	{
		--stackIdx;
		int currNodeIdx = stack[stackIdx];
		const int*	currNode_children	= &BHNode_children	[currNodeIdx * 8];
		if(currNode_children[0] == -1 || 
			distance_to_aabb(BHNode_nnn[currNodeIdx], BHNode_ppp[currNodeIdx], currPos) > minDistance)
		{
			accelerationAccum = accelerationAccum + calculate_acceleration(currPos, currMass, BHNode_massCenter[currNodeIdx], BHNode_mass[currNodeIdx]);
		}
		else
		{
			for(int child = 0; child < 8 && currNode_children[child] != -1; ++child)
			{
				stack[stackIdx] = currNode_children[child];
				++stackIdx;
			}
		}
	}

	accelaration[currIdx] = accelerationAccum;
}

void program_move (int currIdx, const cl_double t, cl_double3* pos, 
	cl_double3* velocity, const cl_double3* accelaration)
{
	velocity[currIdx] = velocity[currIdx] + (accelaration[currIdx] * t);
	pos[currIdx] = pos[currIdx] + (velocity[currIdx] * t);
}
