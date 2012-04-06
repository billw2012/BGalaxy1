
// [length3] [mass-1] [time-2]
// gravitational constant = 
// 1.56092357 × 10-13 (light years cubed) over (solar masses (years squared))
// g units:
//__constant pf_real g = 0.000000000000156;
//__constant pf_real g = 0.000000000000156;
// 39.4775743 (au cubed) over (solar masses (years squared))
static const pf_real g = 39.4775743;

cl_real3 clamp(const cl_real3& pos, const cl_real3& nnn, const cl_real3& ppp)
{
	cl_real3 ret;
	for(size_t idx = 0; idx < 3; ++idx)
		ret.s[idx] = std::min(ppp.s[idx], std::max(pos.s[idx], nnn.s[idx]));
	return ret;
}

inline cl_real3 operator-(const cl_real3& lhs, const cl_real3& rhs)
{
	cl_real3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] - rhs.s[idx];
	return r;
}

inline cl_real3 operator*(const cl_real3& lhs, const cl_real& rhs)
{
	cl_real3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] * rhs;
	return r;
}

inline cl_real3 operator+(const cl_real3& lhs, const cl_real3& rhs)
{
	cl_real3 r;
	for(size_t idx = 0; idx < 3; ++idx)
		r.s[idx] = lhs.s[idx] + rhs.s[idx];
	return r;
}

inline cl_real length(const cl_real3& v)
{
	return sqrt(v.s[0] * v.s[0] + v.s[1] * v.s[1] + v.s[2] * v.s[2]);
}

cl_real distance(const cl_real3& p0, const cl_real3& p1)
{
	return length(p1 - p0);
}

cl_real distance_to_aabb(const cl_real3 nnn, const cl_real3 ppp, const cl_real3 pos)
{
	cl_real3 closest = clamp(pos, nnn, ppp);
	return distance(pos, closest);
}

cl_real bh_calc_s_over_d(const cl_real3 nnn, const cl_real3 ppp, const cl_real3 pos)
{
	pf_real distToNode = distance_to_aabb(nnn, ppp, pos);
	pf_real regionSize = distance(nnn, ppp);
	return regionSize / distToNode;
}

cl_real3 calculate_acceleration(const cl_real3 p0, const cl_real mass0, const cl_real3 p1, const cl_real mass1)
{
	static cl_real3 zero = {0.0, 0.0, 0.0, 0.0};
	cl_real3 v = p1 - p0;
	cl_real len = length(v);
	if(len <= 0.0001)
		return zero;
	return v * (0.000118580398 * mass0 * mass1 / (len * len * mass0));
}

void program_gravity (
	cl_int currIdx,
	const cl_real3 currPos, 
	const cl_real currMass, 
	const cl_real bhTheta, 
	cl_real3* accelaration, 
	const cl_int* BHNode_children, 
	const cl_real3* BHNode_massCenter, 
	const cl_real* BHNode_mass, 
	const cl_real3* BHNode_nnn, 
	const cl_real3* BHNode_ppp
	)
{
	int stack[2048];
	stack[0] = 0;
	int stackIdx = 1;

	cl_real3 accelerationAccum = {0.0, 0.0, 0.0};

	while(stackIdx > 0)
	{
		--stackIdx;
		int currNodeIdx = stack[stackIdx];
		const int*	currNode_children	= &BHNode_children	[currNodeIdx * 8];
		if(currNode_children[0] == -1 || 
			bh_calc_s_over_d(BHNode_nnn[currNodeIdx], BHNode_ppp[currNodeIdx], currPos) < bhTheta)
		{
			accelerationAccum = accelerationAccum + calculate_acceleration(currPos, currMass, BHNode_massCenter[currNodeIdx], BHNode_mass[currNodeIdx]);
		}
		else
		{
			for(int child = 0; child < 8 && currNode_children[child] != -1; ++child)
			{
				stack[stackIdx] = currNode_children[child];
				++stackIdx;
				if(stackIdx >= 2048)
				{
					accelaration[currIdx].s[0] = 1000000000.0;
					return;
				}
			}
		}
	}

	accelaration[currIdx] = accelerationAccum;
}

void program_move (int currIdx, const cl_real t, cl_real3* pos, 
	cl_real3* velocity, const cl_real3* accelaration)
{
	velocity[currIdx] = velocity[currIdx] + (accelaration[currIdx] * t);
	pos[currIdx] = pos[currIdx] + (velocity[currIdx] * t);
}
