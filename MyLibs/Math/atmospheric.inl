

template < class FTy_ >
typename const Atmosphere<FTy_>::spectrum_type Atmosphere<FTy_>::LightWavelengths(FTy_(0.000000700), FTy_(0.000000550), FTy_(0.000000460));

template < class FTy_ >
Atmosphere<FTy_>::Atmosphere() : _innerIntegralTableAvailable(false) 
{
	create_quarter_sphere_samples();
}

template < class FTy_ >
void Atmosphere<FTy_>::set_radii(float_type inner, float_type outer)
{
	_innerRadius = inner;
	_outerRadius = outer;
}

// scale height is the thickness of the atmosphere its density
// were uniform (7994m for Earth's atmosphere)
template < class FTy_ >
void Atmosphere<FTy_>::set_scale_height(float_type scaleHeight)
{
	_scaleHeight = scaleHeight;
}

// n is the index of refraction of the air
// Ns is the molecular number density of the standard atmosphere
template < class FTy_ >
void Atmosphere<FTy_>::set_k(float_type n, float_type Ns)
{
	_K = (float_type(2) * float_type(M_PI) * float_type(M_PI) * (n * n - float_type(1)) * (n * n - float_type(1))) / (float_type(3) * Ns);
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::calculate_k(float_type n, float_type Ns)
{
	return (float_type(2) * float_type(M_PI) * float_type(M_PI) * (n * n - float_type(1)) * (n * n - float_type(1))) / (float_type(3) * Ns);
}

template < class FTy_ >
typename Atmosphere<FTy_>::vec3_type Atmosphere<FTy_>::calculate_4PiKOverWavelength4(float_type n, float_type Ns, vec3_type wavelength)
{
	return (float_type(4) * float_type(M_PI) * calculate_k(n, Ns)) / ::pow(wavelength, 4);
}

// wavelength of light entering the atmosphere
template < class FTy_ >
void Atmosphere<FTy_>::set_light_wavelength(const spectrum_type& waveLength)
{	
	_waveLength = waveLength;
	_beta = (float_type(4) * float_type(M_PI) * _K) / 
		::pow(waveLength, 4);
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::get_g(float_type u)
{
	float_type x = float_type(5.0/9.0) * u + float_type(125.0/729.0) * u * u * u + ::sqrt(float_type(64.0/27.0) - float_type(325.0/243.0) * u * u + float_type(1250.0/2187.0) * u * u * u * u);
	float_type xCubeRoot = ::pow(x, float_type(1.0/3.0));
	return float_type(5.0/9.0) * u - (float_type(4.0/3.0) - float_type(25.0/81.0) * u * u) * (float_type(1) / xCubeRoot) + xCubeRoot;
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_optical_depth_table(int samples, int angleSamples, int heightSamples)
{
	_angleSamples = angleSamples;
	_heightSamples = heightSamples;
	_heightFrac = (_outerRadius - _innerRadius) / float_type(heightSamples);
	_angleFrac = float_type(ATMOSPHERE_TABLE_ANGLE_DEGREES/180.0*M_PI) / float_type(angleSamples);

	calculate_optical_depth_table(samples, angleSamples, heightSamples, _opticalDepthTable);
}

template < class FTy_ >
const std::vector<typename Atmosphere<FTy_>::float_type> Atmosphere<FTy_>::get_optical_depth_table() const 
{
	return _opticalDepthTable;
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_optical_depth_table(int samples, int angleSamples, int heightSamples, std::vector<float_type>& results) const
{
	results.resize(angleSamples * heightSamples);

	float_type fullHeight = _outerRadius - _innerRadius;
	float_type heightFrac = fullHeight / float_type(heightSamples);
	float_type angleFrac = float_type(ATMOSPHERE_TABLE_ANGLE_DEGREES*M_PI/180.0) / float_type(angleSamples);

	float_type angle = 0;
	for(int aIdx = 0, rIdx = 0; aIdx < angleSamples; ++aIdx, angle += _angleFrac)
	{
		float_type height = 0;
		for(int hIdx = 0; hIdx < heightSamples; ++hIdx, height += _heightFrac, ++rIdx)
		{
			results[rIdx] = calculate_optical_depth_height_angle(height, angle, _innerRadius, _outerRadius, _scaleHeight, samples);
		}
	}
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_surface_lighting_table(const spectrum_type& Is, float_type mieu, 
	int PaPbSteps, int PPaSteps, int PPcSteps, 
	int angleSamples, int heightSamples, 
	std::vector<spectrum_type>& Isky) const
{
	Isky.resize(angleSamples * heightSamples);

	float_type heightFrac = (_outerRadius - _innerRadius) / float_type(heightSamples);
	float_type angleFrac = float_type(ATMOSPHERE_TABLE_ANGLE_DEGREES*M_PI/180.0) / float_type(angleSamples);

	float_type angle(0);
	vec3_type sunDir(float_type(-1), float_type(0), float_type(0));
	float_type mieg = get_g(mieu);
	for(int aIdx = 0, rIdx = 0; aIdx < angleSamples; ++aIdx, angle += angleFrac)
	{
		float_type height = 0;

		vec3_type eyeDir(cos(angle), sin(angle), 0);
		mat4_type samplePtRotMat(rotatez_rad(angle));
		for(int hIdx = 0; hIdx < heightSamples; ++hIdx, height += heightFrac, ++rIdx)
		{
			vec3_type eyePos(eyeDir * (height + _innerRadius + float_type(EPSILON)));

			spectrum_type rayleigh;
			spectrum_type mie;
			for(size_t sIdx = 0; sIdx < _quarterSphereSamples.size(); ++sIdx)
			{
				vec3_type samplePt(samplePtRotMat * vec4_type(_quarterSphereSamples[sIdx]));
				rayleigh += irradiance_at_view_Iv(Is, sunDir, eyePos, samplePt, float_type(0), PaPbSteps, PPaSteps, PPcSteps);
				mie += irradiance_at_view_Iv(Is, sunDir, eyePos, samplePt, mieg, PaPbSteps, PPaSteps, PPcSteps);
			}

			Isky[rIdx] = float_type(M_PI * 2) * (mie + rayleigh) / float_type(_quarterSphereSamples.size());
		}
	}
}

template < class FTy_ >
void Atmosphere<FTy_>::create_quarter_sphere_samples()
{
	while(_quarterSphereSamples.size() < QUARTER_SPHERE_SAMPLES)
	{
		vec3_type p(float_type(rand()) / float_type(RAND_MAX) - float_type(0.5), 
			float_type(rand()) / float_type(RAND_MAX) - float_type(0.5),
			float_type(rand()) / float_type(RAND_MAX) - float_type(0.5));
		if(p.x >= 0 && p.y >= 0)
		{
			p.normalize();
			_quarterSphereSamples.push_back(p);
		}
	}
}

template < class FTy_ >
void Atmosphere<FTy_>::irradiance_at_view_Iv_table(const spectrum_type& Is, const vec3_type& eyePos, vec3_type upDir, const vec3_type& sunDir, float_type g, int xySamples, int PaPbSteps, int PPaSteps, int PPcSteps, std::vector<spectrum_type>& results) const
{
	results.resize(xySamples * xySamples);

	vec3_type eyeDir = eyePos.normal();
	vec3_type rightDir = eyeDir.crossp(upDir).normal();
	upDir = rightDir.crossp(eyeDir).normal();

	mat4_type orientMat;
	orientMat.setColumnVector(0, vec4_type(rightDir));
	orientMat.setColumnVector(1, vec4_type(upDir));
	orientMat.setColumnVector(2, vec4_type(eyeDir));

	for(int x = 0; x < xySamples; ++x)
	{
		float_type xp = float_type(x) / float_type(xySamples) - float_type(0.5);
		for(int y = 0; y < xySamples; ++y)
		{
			float_type yp = float_type(y) / float_type(xySamples) - float_type(0.5);
			float_type h = ((::abs(xp)-float_type(0.5)) * (::abs(yp)-float_type(0.5))) * float_type(4.0);
			vec3_type vec = vec3_type(xp, yp, h);

			vec3_type orientedEyeDir((orientMat * vec4_type(vec)).normal());

			results[y * xySamples + x] = irradiance_at_view_Iv(Is, sunDir, eyePos, orientedEyeDir, g, PaPbSteps, PPaSteps, PPcSteps);
		}
	}
}

template < class FTy_ >
typename Atmosphere<FTy_>::spectrum_type Atmosphere<FTy_>::irradiance_at_view_Iv(const spectrum_type& Is, const vec3_type& sunDir, const vec3_type& eye, const vec3_type& eyeDir, float_type g, int PaPbSteps, int PPaSteps, int PPcSteps) const
{
	RaySphereIntersectRec atmosphereISect = sphere_ray_isect(eye, eyeDir, _outerRadius);

	if(atmosphereISect.result == RaySphereIntersectRec::Result::None)
		return Is;
	vec3_type startPos = atmosphereISect.t1 < 0 ? eye : atmosphereISect.p1;
	RaySphereIntersectRec planetISect = sphere_ray_isect(eye, eyeDir, _innerRadius);
	// if there is no intersection with the planet or both intersection points are behind the eye
	if(planetISect.result == RaySphereIntersectRec::Result::None || 
		(planetISect.t1 <= 0 && planetISect.t2 <= 0))
	{
		// calculate from either the determined start position (either the eye or the first 
		// intersection with the atmosphere), to the second intersection point with the atmosphere
		return calculate_irradiance_at_view_Iv(Is, _waveLength, _beta, sunDir, startPos, atmosphereISect.p2, _innerRadius, _outerRadius, _scaleHeight, _K, g, PaPbSteps, PPaSteps, PPcSteps);
	}
	// check the eye isn't IN the planet
	assert(!(planetISect.t1 < 0 && planetISect.t2 > 0));
	// calculate from the determined start position to the first intersection with the planet
	return calculate_irradiance_at_view_Iv(Is, _waveLength, _beta, sunDir, startPos, planetISect.p1, _innerRadius, _outerRadius, _scaleHeight, _K, g, PaPbSteps, PPaSteps, PPcSteps);
}


// calculates the scattering phase function Fr for a particular angle
template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::get_scattering_phase_Fr(float_type angle, float_type g)
{
	float_type cosAngle = float_type(::cos(angle));
	return ((float_type(3) * (float_type(1) - g * g)) / (float_type(2) * (float_type(2) + g * g))) * 
		((float_type(1) + cosAngle * cosAngle) / ::pow(float_type(1) + g * g - float_type(2) * g * cosAngle, float_type(3.0/2.0)));
}

// calculates the density ratio rho (p) for a given height (h) and scale height (H0)
template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::get_density_ratio_rho(float_type h, float_type scaleHeight)
{
	return float_type(::exp(-h / scaleHeight));
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::calculate_optical_depth_height_angle(float_type height, float_type angleFromNormal, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int samples)
{
	vec2_type startP(height + innerRadius - float_type(EPSILON), float_type(0));
	mat3_type rotMat(rotate_2D_rad(angleFromNormal));
	vec2_type rayDir(rotMat * vec3_type(startP, float_type(0)));
	vec2_type atmosIsect(circle_ray_isect(startP, rayDir, outerRadius).p2);
	float_type endh(atmosIsect.length() - innerRadius);
	float_type distance((atmosIsect - startP).length());
	return calculate_optical_depth(height, endh, distance, innerRadius, scaleHeight, samples);
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::calculate_optical_depth(float_type starth, float_type endh, float_type distance, float_type innerRadius, float_type scaleHeight, int samples)
{
	float_type sampleWidth = distance / float_type(samples + 1);
	float_type sampleSum = get_density_ratio_rho(starth, scaleHeight) + get_density_ratio_rho(endh, scaleHeight);
	float_type sampleFrac = float_type(1) / float_type(samples + 1);
	float_type frac = sampleFrac;
	for(int sampleIdx = 0; sampleIdx < samples; ++sampleIdx, frac += sampleFrac)
	{
		float_type currh = interpolate_height(starth, endh, innerRadius, distance, frac);
		sampleSum += get_density_ratio_rho(currh, scaleHeight) * 2;
	}
	sampleSum *= (float_type(0.5) * sampleWidth);

	return sampleSum;
}

template < class FTy_ >
typename Atmosphere<FTy_>::vec2_type Atmosphere<FTy_>::calculate_end_pos(float_type starth, float_type endh, float_type radius, float_type distance)
{
	assert(distance < (radius * 2 + starth + endh));
	float_type a = endh + radius, b = distance, c = starth + radius;
	float_type angle = float_type( ::acos( std::max( float_type(-1), 
		std::min( float_type(1), (c*c + a*a - b*b) / (float_type(2) * c * a) ) ) ) );
	float_type Cx = float_type(::cos(angle)) * a;
	float_type Cy = float_type(::sin(angle)) * a;
	return vec2_type(Cx, Cy);
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::interpolate_height(float_type starth, float_type endh, float_type radius, float_type distance, float_type fraction)
{
	/*
	 C
	  +
	  |\
	  |  \
	  |    \  b
	a |      \
	  |        \
	  |angle     \
	  +-----------+ A
	 B      c
	
	using cosine rule for non right angle triangles, get the angle between the points
	formed by starth, endh and distance.
	*/
	vec2_type startPos(starth + radius, float_type(0));
	vec2_type endPos = calculate_end_pos(starth, endh, radius, distance);
	vec2_type pt = (endPos - startPos) * fraction + startPos;
	return pt.length() - radius;
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_KFrThetaOverLambda4Rayleigh_table(spectrum_type waveLength, size_t samples)
{
	calculate_KFrThetaOverLambda4_table(_K, float_type(0), waveLength, samples, _KFrThetaOverLambda4TableRayleigh);
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_KFrThetaOverLambda4Mie_table(float_type g, spectrum_type waveLength, size_t samples)
{
	calculate_KFrThetaOverLambda4_table(_K, g, waveLength, samples, _KFrThetaOverLambda4TableMie);
}

template < class FTy_ >
void Atmosphere<FTy_>::calculate_KFrThetaOverLambda4_table(float_type K, float_type g, spectrum_type waveLength, size_t samples, std::vector<spectrum_type>& table)
{
	float_type da = float_type(M_PI) / float_type(samples);
	float_type angle(0);

	spectrum_type oneOverpowWL4 = float_type(1) / ::pow(waveLength, 4);
	table.resize(samples);
	for(size_t sample = 0; sample < samples; ++sample, angle += da)
	{
		table[sample] = K * get_scattering_phase_Fr(angle, g) * oneOverpowWL4; 
	}
}

// Is - irradiance entering atmosphere
// waveLength - wavelength of light
// beta - result of the beta function
// theta - angle between viewer and sun vectors
// Pa - end of ray nearest viewer (could be view position or intersection with atmosphere)
// Pb - end of ray furthest from viewer (could be intersection with solid object or atmosphere)
template < class FTy_ >
typename Atmosphere<FTy_>::spectrum_type Atmosphere<FTy_>::calculate_irradiance_at_view_Iv(const spectrum_type& Is, const spectrum_type& waveLength, const spectrum_type& beta, const vec3_type& sunDir, vec3_type Pa, vec3_type Pb, float_type innerRadius, float_type outerRadius, float_type scaleHeight, float_type K, float_type g, int PaPbSteps, int PPaSteps, int PPcSteps) const
{
	float_type theta = sunDir.angle((Pa - Pb).normal());
	spectrum_type KFrThetaOverLambda4 = K * get_scattering_phase_Fr(theta, g) / ::pow(waveLength, 4);

	float_type distancePaPb = (Pa - Pb).length();

	// integrate from Pa to Pb
	float_type sampleWidth = distancePaPb / float_type(PaPbSteps + 1);

	spectrum_type sampleSum = 
		calculate_inner_integral(beta, Pa, Pb, Pa, sunDir, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps) + 
		calculate_inner_integral(beta, Pa, Pb, Pb, sunDir, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps);
	float_type sampleFrac = float_type(1) / float_type(PaPbSteps + 1);
	float_type frac = sampleFrac;
	for(int sampleIdx = 0; sampleIdx < PaPbSteps; ++sampleIdx, frac += sampleFrac)
	{
		vec3_type P = Pa + (Pb - Pa) * frac;
		sampleSum += calculate_inner_integral(beta, Pa, Pb, P, sunDir, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps) * 2;
	}
	sampleSum *= (float_type(0.5) * sampleWidth);
	return Is * KFrThetaOverLambda4 * sampleSum;
}

// solve the out scattering from Pc to Pa
template < class FTy_ >
typename Atmosphere<FTy_>::spectrum_type Atmosphere<FTy_>::calculate_inner_integral(
	const spectrum_type& beta, vec3_type Pa, vec3_type Pb, const vec3_type& P, 
	const vec3_type& sunDir, float_type innerRadius, float_type outerRadius, float_type scaleHeight, 
	int PPaSteps, int PPcSteps) const
{
	float_type heightP = P.length() - innerRadius;
	float_type heightPa = Pa.length() - innerRadius;
	float_type heightPb = Pb.length() - innerRadius;

	if(heightPb > heightPa)
	{
		std::swap(Pa, Pb);
		std::swap(heightPa, heightPb);
	}
	//vec3_type Pc = calculate_Pc(P, sunDir, outerRadius);
	//float_type heightPc = Pc.length() - innerRadius;

	//float_type angleFromNormalPc = (Pc - P).normal().angle(P.normal());
	vec3_type pNormal = P.normal();
	float_type angleFromLight = (-sunDir).angle(pNormal);
	spectrum_type tPPc = beta * interpolate_optical_depth(angleFromLight, heightP);

	vec3_type PbPaNormal = (Pa - Pb).normal();
	float_type angleP_PbPa = PbPaNormal.angle(pNormal);
	float_type anglePa_PbPa = PbPaNormal.angle(Pa.normal());
	// DOING: Is this right? heightPc should always be 
	// outer atmosphere height. Should be depth from Pa - 
	// depth from P I think...
	float_type tPPaReminder = interpolate_optical_depth(anglePa_PbPa, heightPa);
	float_type tPPaFull = interpolate_optical_depth(angleP_PbPa, heightP);
	spectrum_type tPPa = beta * (tPPaFull - tPPaReminder);

	float_type rhoP = get_density_ratio_rho(heightP, scaleHeight);
	return rhoP * ::exp(-(tPPc + tPPa));
}

template < class FTy_ >
void Atmosphere<FTy_>::restrict_length(vec3_type& p, float_type len)
{
	float_type pLen = p.lengthSquared();
	if(pLen > len*len)
	{
		pLen = ::sqrt(pLen);
		p = p * (len / pLen);
	}
}

template < class FTy_ >
typename Atmosphere<FTy_>::vec3_type Atmosphere<FTy_>::calculate_Pc(vec3_type P, const vec3_type& sunDir, float_type outerRadius)
{
	restrict_length(P, outerRadius - float_type(EPSILON));
	RaySphereIntersectRec isect = sphere_ray_isect(P, sunDir, outerRadius);
	assert(isect.result != RaySphereIntersectRec::Result::None);
#if defined(_DEBUG)
	//float p1len = isect.p1.length();
	//assert(p1len > outerRadius - float_type(EPSILON * 2) && p1len < outerRadius + float_type(EPSILON * 2));
	float p1Pdot = isect.p1.normal().dotp(P.normal());
	assert(p1Pdot >= 0.0);
#endif
	return isect.p1;
}

// assumes circle is centered on origin
// solution from http://stackoverflow.com/questions/1073336/circle-line-collision-detection
template < class FTy_ >
typename Atmosphere<FTy_>::RayCircleIntersectRec Atmosphere<FTy_>::circle_ray_isect(const vec2_type& rayStart, const vec2_type& rayDir, float_type radius)
{
	float_type a = rayDir.dotp(rayDir);
	float_type b = float_type(2) * rayStart.dotp(rayDir);
	float_type c = rayStart.dotp(rayStart) - radius * radius;

	float_type discriminant = b * b - float_type(4) * a * c;

	discriminant = ::sqrt( discriminant );
	
	if(discriminant < float_type(0))
		return RayCircleIntersectRec();

	float_type t1 = (-b + discriminant) / (float_type(2) * a);
	// not sure this is much use, requires extra branches in calling code
	//if(discriminant == 0)
	//	return RayCircleIntersectRec(RayCircleIntersectRec::Result::T1,
	//		t1, rayStart + rayDir * t1);
	float_type t2 = (-b - discriminant) / (float_type(2) * a);
	if(t1 < t2)
		return RayCircleIntersectRec(RayCircleIntersectRec::Result::T1andT2,
			t1, rayStart + rayDir * t1, t2, rayStart + rayDir * t2);
	return RayCircleIntersectRec(RayCircleIntersectRec::Result::T1andT2,
		t2, rayStart + rayDir * t2, t1, rayStart + rayDir * t1);
}

template < class FTy_ >
typename Atmosphere<FTy_>::RaySphereIntersectRec Atmosphere<FTy_>::sphere_ray_isect(const vec3_type& rayStart, const vec3_type& rayDir, float_type radius)
{
	//Compute A, B and C coefficients
	float_type a = rayDir.dotp(rayDir);
	float_type b = float_type(2) * rayDir.dotp(rayStart);
	float_type c = rayStart.dotp(rayStart) - (radius * radius);

	//Find discriminant
	float_type discriminant = b * b - float_type(4) * a * c;

	// if discriminant is negative there are no real roots, so return 
	// false as ray misses sphere
	if (discriminant < float_type(0))
		return RaySphereIntersectRec();

	if(discriminant == 0)
	{
		float_type q;
		if (b < float_type(0))
			q = (-b - discriminant)/float_type(2);
		else
			q = (-b + discriminant)/float_type(2);

		// compute t0 and t1
		float_type t0 = q / a;
		return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1, t0, rayStart + rayDir * t0, t0, rayStart + rayDir * t0);
	}
	// compute q as described above
	discriminant = ::sqrt(discriminant);

	float_type q;
	if (b < float_type(0))
		q = (-b - discriminant)/float_type(2);
	else
		q = (-b + discriminant)/float_type(2);

	if(q == 0)
	{
		return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1, float_type(0), rayStart, float_type(0), rayStart);
	}
	// compute t0 and t1
	float_type t0 = q / a;
	float_type t1 = c / q;

	if(t0 == t1)
		return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1, t0, rayStart + rayDir * t0, t0, rayStart + rayDir * t0);
	if(t0 < t1)
		return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1andT2, t0, rayStart + rayDir * t0, t1, rayStart + rayDir * t1);
	return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1andT2, t1, rayStart + rayDir * t1, t0, rayStart + rayDir * t0);
}

template < class FTy_ >
typename Atmosphere<FTy_>::float_type Atmosphere<FTy_>::interpolate_optical_depth(float_type angle, float_type height) const
{
	float_type angleFrac = angle / _angleFrac;
	float_type heightFrac = height / _heightFrac;
	float_type floorAngleFrac = floor(angleFrac);
	float_type floorHeightFrac = floor(heightFrac);
	float_type dAngleFrac = angleFrac - floorAngleFrac;
	float_type dHeightFrac = heightFrac - floorHeightFrac;
	int b = std::max(0, std::min(_angleSamples-1, int(floorAngleFrac)));
	int t = std::max(0, std::min(_angleSamples-1, int(ceil(angleFrac))));
	int l = std::max(0, std::min(_heightSamples-1, int(floorHeightFrac)));
	int r = std::max(0, std::min(_heightSamples-1, int(ceil(heightFrac))));
	int blIdx = b * _angleSamples + l;
	int brIdx = b * _angleSamples + r;
	int tlIdx = t * _angleSamples + l;
	int trIdx = t * _angleSamples + r;
	float_type topPt = _opticalDepthTable[tlIdx] + (_opticalDepthTable[trIdx] - _opticalDepthTable[tlIdx]) * dHeightFrac; 
	float_type bottomPt = _opticalDepthTable[blIdx] + (_opticalDepthTable[brIdx] - _opticalDepthTable[blIdx]) * dHeightFrac; 
	return bottomPt + (topPt - bottomPt) * dAngleFrac;
}
