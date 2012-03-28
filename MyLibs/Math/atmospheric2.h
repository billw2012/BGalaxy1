
#if !defined(__EXPLORE2_ATMOSPHERIC_H__)
#define __EXPLORE2_ATMOSPHERIC_H__

#include "Math/vector2.hpp"
#include "Math/vector3.hpp"
#include "Math/vector4.hpp"
#include "Math/matrix3.hpp"
#include "Math/matrix4.hpp"
#include "Math/transformation.hpp"

template < class V, class H, class V2 >
math::Vector3<V, H> pow(const math::Vector3<V, H>& _X, V2 _Y)
{
	return math::Vector3<V,H>(::pow(_X.x, _Y), ::pow(_X.y, _Y), ::pow(_X.z, _Y));
}

template < class V, class H >
math::Vector3<V, H> exp(const math::Vector3<V, H>& _X)
{
	return math::Vector3<V,H>(::exp(_X.x), ::exp(_X.y), ::exp(_X.z));
}

namespace math {;

struct Atmosphere 
{
	typedef double float_type;
	typedef math::Vector3<float_type> spectrum_type;
	typedef math::Vector3<float_type> vec3_type;
	typedef math::Vector4<float_type> vec3_type;
	typedef math::Vector2<float_type> vec2_type;
	typedef math::Matrix3<float_type> mat3_type;
	typedef math::Matrix4<float_type> mat4_type;

	void set_radii(float_type inner, float_type outer)
	{
		_innerRadius = inner;
		_outerRadius = outer;
	}

	// scale height is the thickness of the atmosphere its density
	// were uniform (7994m for Earth's atmosphere)
	void set_scale_height(float_type scaleHeight)
	{
		_scaleHeight = scaleHeight;
	}

	// n is the index of refraction of the air
	// Ns is the molecular number density of the standard atmosphere
	void set_k(float_type n, float_type Ns)
	{
		_K = (float_type(2) * float_type(M_PI) * float_type(M_PI) * (n * n - float_type(1)) * (n * n - float_type(1))) / (float_type(3) * Ns);
	}

	// wavelength of light entering the atmosphere
	void set_light_wavelength(const spectrum_type& waveLength)
	{	
		_waveLength = waveLength;
		_beta = (float_type(4) * float_type(M_PI) * _K) / 
			::pow(waveLength, 4);
	}

	spectrum_type inner_integral(const vec2_type& Pa, const vec2_type& Pb, const vec2_type& P, float_type theta, int PPaSteps, int PPcSteps)
	{
		return calculate_inner_integral(_beta, Pa, Pb, P, theta, _innerRadius, _outerRadius, _scaleHeight, PPaSteps, PPcSteps);
	}

	spectrum_type inner_integral(float_type height, float_type angleFromNormal, int samples)
	{
		return calculate_inner_integral_P_Pc_only(_beta, height, angleFromNormal, _innerRadius, _outerRadius, _scaleHeight, samples);
	}

	void calculate_inner_integral_table(int samples, int angleSamples, int heightSamples, std::vector<spectrum_type>& results)
	{
		results.resize(angleSamples * heightSamples);

		float_type heightD = (_outerRadius - _innerRadius) / float_type(heightSamples);
		float_type angleD = float_type(110.0/180.0*M_PI) / float_type(angleSamples);

		float_type angle = 0;
		for(int aIdx = 0, rIdx = 0; aIdx < angleSamples; ++aIdx, angle += angleD)
		{
			float_type height = 0;
			for(int hIdx = 0; hIdx < heightSamples; ++hIdx, height += heightD, ++rIdx)
			{
				results[rIdx] = inner_integral(height, angle, samples);
			}
		}
	}

	void calculate_irradiance_at_view_Iv_table(const vec3_type& eyePos, vec3_type upDir, const vec3_type& sunDir, int xySamples, int PaPbSteps, int PPaSteps, int PPcSteps, std::vector<spectrum_type>& results)
	{
		results.resize(xySamples * xySamples);

		vec3_type eyeDir = eyePos.normal();
		vec3_type rightDir = eyeDir.crossp(upDir).normal();
		vec3_type upDir = rightDir.crossp(eyeDir).normal();

		mat4_type orientMat;
		orientMat.setColumnVector(0, vec4_type(rightDir));
		orientMat.setColumnVector(1, vec4_type(upDir));
		orientMat.setColumnVector(2, vec4_type(eyeDir));

		std::vector<std::vector<vec3_type> > grid(xySamples, std::vector<vec3_type>(xySamples));

		for(int x = 0; x < xySamples; ++x)
		{
			float_type xp = float_type(x) / float_type(xySamples) - float_type(0.5);
			for(int y = 0; y < xySamples; ++y)
			{
				float_type yp = float_type(y) / float_type(xySamples) - float_type(0.5);
				float_type h = float_type(1) - ::pow(::abs(xp) + ::abs(yp), 2);
				vec3_type vec = vec3_type(xp, yp, h);

				vec3_type orientedEyeDir((orientMat * vec4_type(vec)).normal());

				float_type theta = M_PI - orientedEyeDir.angle(sunDir);

				vec2_type eyePos2d(eyePos.length(), 0);
				vec2_type eyeDir2d();
			}
		}

// 		float_type angleInc = float_type(M_PI) / float_type(xySamples);
// 
// 		float_type xAngle(0);
// 
// 		float_type heightD = (_outerRadius - _innerRadius) / float_type(heightSamples);
// 		float_type angleD = float_type(110.0/180.0*M_PI) / float_type(angleSamples);
// 
// 		float_type angle = 0;
// 		for(int aIdx = 0, rIdx = 0; aIdx < angleSamples; ++aIdx, angle += angleD)
// 		{
// 			float_type height = 0;
// 			for(int hIdx = 0; hIdx < heightSamples; ++hIdx, height += heightD, ++rIdx)
// 			{
// 				results[rIdx] = inner_integral(height, angle, samples);
// 			}
// 		}
	}


	spectrum_type irradiance_at_view_Iv(const spectrum_type& Is, float_type theta, const vec2_type& eye, const vec2_type& eyeDir, int PaPbSteps, int PPaSteps, int PPcSteps)
	{
		RayCircleIntersectRec atmosphereISect = circle_ray_isect(eye, eyeDir, _outerRadius);

		if(atmosphereISect.result == RayCircleIntersectRec::Result::None)
			return Is;
		vec2_type startPos = atmosphereISect.t1 < 0 ? eye : atmosphereISect.p1;
		RayCircleIntersectRec planetISect = circle_ray_isect(eye, eyeDir, _innerRadius);
		// if there is no intersection with the planet or both intersection points are behind the eye
		if(planetISect.result == RayCircleIntersectRec::Result::None || 
			(planetISect.t1 <= 0 && planetISect.t2 <= 0))
		{
			// calculate from either the determined start position (either the eye or the first 
			// intersection with the atmosphere), to the second intersection point with the atmosphere
			return calculate_irradiance_at_view_Iv(Is, _waveLength, _beta, theta, startPos, atmosphereISect.p2, _innerRadius, _outerRadius, _scaleHeight, _K, PaPbSteps, PPaSteps, PPcSteps);
		}
		// check the eye isn't IN the planet
		assert(!(planetISect.t1 < 0 && planetISect.t2 > 0));
		// calculate from the determined start position to the first intersection with the planet
		return calculate_irradiance_at_view_Iv(Is, _waveLength, _beta, theta, startPos, planetISect.p1, _innerRadius, _outerRadius, _scaleHeight, _K, PaPbSteps, PPaSteps, PPcSteps);
	}

private:
	// calculates the scattering phase function Fr for a particular angle
	static inline float_type get_scattering_phase_Fr(float_type angle)
	{
		float_type cosAngle = float_type(::cos(angle));
		return float_type(0.75) * (float_type(1) + cosAngle * cosAngle);
	}

	// calculates the density ratio rho (p) for a given height (h) and scale height (H0)
	static inline float_type get_density_ratio_rho(float_type h, float_type scaleHeight)
	{
		return float_type(::exp(-h / scaleHeight));
	}

	static spectrum_type calculate_inner_integral_height_angle(const spectrum_type& beta, float_type height, float_type angleFromNormal, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int samples)
	{
		vec2_type startP(height + innerRadius, float_type(0));
		mat3_type rotMat(rotate_2D_rad(angleFromNormal));
		vec2_type rayDir(rotMat * vec3_type(startP, float_type(0)));
		vec2_type atmosIsect(circle_ray_isect(startP, rayDir, outerRadius).p2);
		float_type endh(atmosIsect.length() - innerRadius);
		float_type distance((atmosIsect - startP).length());
		return calculate_inner_integral(beta, height, endh, distance, innerRadius, scaleHeight, samples);
	}

	// over height instead of position
	// beta - result of the beta function
	// starth - starting height
	// endh - ending height
	// distance - distance from start point (where height is starth) to end point
	// innerRadius - the radius of the planet at height = 0
	// scaleHeight - the set scaleHeight
	// samples - number of samples to use during integration (trapezium) 
	static spectrum_type calculate_inner_integral(const spectrum_type& beta, float_type starth, float_type endh, float_type distance, float_type innerRadius, float_type scaleHeight, int samples)
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

		return beta * sampleSum;
	}

	static inline vec2_type calculate_end_pos(float_type starth, float_type endh, float_type radius, float_type distance)
	{
		assert(distance < (radius * 2 + starth + endh));
		float_type a = endh + radius, b = distance, c = starth + radius;
		float_type angle = float_type(::acos((c*c + a*a - b*b) / (2 * c * a)));
		float_type Cx = float_type(::cos(angle)) * a;
		float_type Cy = float_type(::sin(angle)) * a;
		return vec2_type(Cx, Cy);
	}

	static float_type interpolate_height(float_type starth, float_type endh, float_type radius, float_type distance, float_type fraction)
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

	// Is - irradiance entering atmosphere
	// waveLength - wavelength of light
	// beta - result of the beta function
	// theta - angle between viewer and sun vectors
	// Pa - end of ray nearest viewer (could be view position or intersection with atmosphere)
	// Pb - end of ray furthest from viewer (could be intersection with solid object or atmosphere)
	static spectrum_type calculate_irradiance_at_view_Iv(const spectrum_type& Is, const spectrum_type& waveLength, const spectrum_type& beta, float_type theta, const vec2_type& Pa, const vec2_type& Pb, float_type innerRadius, float_type outerRadius, float_type scaleHeight, float_type K, int PaPbSteps, int PPaSteps, int PPcSteps)
	{
		spectrum_type KFrThetaOverLambda4 = K * get_scattering_phase_Fr(theta) / ::pow(waveLength, 4);

		float_type distancePaPb = (Pa - Pb).length();

		// integrate from Pa to Pb
		float_type sampleWidth = distancePaPb / float_type(PaPbSteps + 1);

		spectrum_type sampleSum = 
			calculate_inner_integral(beta, Pa, Pb, Pa, theta, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps) + 
			calculate_inner_integral(beta, Pa, Pb, Pb, theta, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps);
		float_type sampleFrac = float_type(1) / float_type(PaPbSteps + 1);
		float_type frac = sampleFrac;
		for(int sampleIdx = 0; sampleIdx < PaPbSteps; ++sampleIdx, frac += sampleFrac)
		{
			vec2_type P = Pa + (Pb - Pa) * frac;
			sampleSum += calculate_inner_integral(beta, Pa, Pb, P, theta, innerRadius, outerRadius, scaleHeight, PPaSteps, PPcSteps) * 2;
		}
		sampleSum *= (float_type(0.5) * sampleWidth);
		return Is * KFrThetaOverLambda4 * sampleSum;
	}

	// solve the out scattering from Pc to Pa
	static spectrum_type calculate_inner_integral(const spectrum_type& beta, const vec2_type& Pa, const vec2_type& Pb, const vec2_type& P, float_type theta, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int PPaSteps, int PPcSteps)
	{
		float_type heightP = P.length() - innerRadius;
		float_type heightPa = Pa.length() - innerRadius;

		vec2_type Pc = calculate_Pc(Pb, Pa, theta, outerRadius);
		float_type heightPc = Pc.length() - innerRadius;

		spectrum_type tPPc = calculate_inner_integral(beta, heightP, heightPc, (Pc - P).length(), innerRadius, scaleHeight, 
PPcSteps);
		spectrum_type tPPa = calculate_inner_integral(beta, heightP, heightPa, (Pa - P).length(), innerRadius, scaleHeight, PPaSteps);
		float_type rhoP = get_density_ratio_rho(heightP, scaleHeight);
		return rhoP * ::exp(-(tPPc + tPPa));
	}

	// solve the out scattering from P to Pc only (ignoring P to Pa scattering)
	// height - height of P
	// angleFromNormal - angle of ray P->Pc from vertical
	static spectrum_type calculate_inner_integral_P_Pc_only(const spectrum_type& beta, float_type height, float_type angleFromNormal, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int PPcSteps)
	{
		spectrum_type tPPc = calculate_inner_integral_height_angle(beta, height, angleFromNormal, innerRadius, outerRadius, scaleHeight, PPcSteps);
		float_type rhoP = get_density_ratio_rho(height, scaleHeight);
		return rhoP * ::exp(-tPPc);
	}

	static vec2_type calculate_Pc(const vec2_type& Pa, const vec2_type& P, float_type theta, float_type outerRadius)
	{
		// get vector rotated from P->Pa by theta
		vec2_type dirP = (Pa - P).normal();
		mat3_type rotateToSunDir = math::rotate_2D_rad(theta);
		vec2_type sunDir(rotateToSunDir * vec3_type(dirP, float_type(0)));
		return circle_ray_isect(P, sunDir, outerRadius).p1;
	}

	struct RayCircleIntersectRec
	{
		struct Result { enum type {
			None,
			T1,
			T1andT2
		};};

		RayCircleIntersectRec(Result::type result_ = Result::None, float_type t1_ = 0, const vec2_type& p1_ = vec2_type(), float_type t2_ = 0, const vec2_type& p2_ = vec2_type()) 
			: result(result_), t1(t1_), t2(t2_), p1(p1_), p2(p2_) {}


		Result::type result;
		float_type t1, t2;
		vec2_type p1, p2;
	};

	// assumes circle is centered on origin
	// solution from http://stackoverflow.com/questions/1073336/circle-line-collision-detection
	static inline RayCircleIntersectRec circle_ray_isect(const vec2_type& rayStart, const vec2_type& rayDir, float_type radius)
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

	struct RaySphereIntersectRec
	{
		struct Result { enum type {
			None,
			T1,
			T1andT2
		};};

		RaySphereIntersectRec(Result::type result_ = Result::None, float_type t1_ = 0, const vec3_type& p1_ = vec3_type(), float_type t2_ = 0, const vec3_type& p2_ = vec3_type()) 
			: result(result_), t1(t1_), t2(t2_), p1(p1_), p2(p2_) {}


		Result::type result;
		float_type t1, t2;
		vec3_type p1, p2;
	};

	static inline RaySphereIntersectRec circle_ray_isect(const vec3_type& rayStart, const vec3_type& rayDir, float_type radius)
	{
		//Compute A, B and C coefficients
		float_type a = dot(rayDir, rayDir);
		float_type b = float_type(2) * dot(rayDir, rayStart);
		float_type c = dot(rayStart, rayStart) - (radius * radius);

		//Find discriminant
		float_type discriminant = b * b - float_type(4) * a * c;

		// if discriminant is negative there are no real roots, so return 
		// false as ray misses sphere
		if (discriminant < float_type(0))
			return RaySphereIntersectRec();

		// compute q as described above
		discriminant = ::sqrt(discriminant);
		float_type q;
		if (b < float_type(0))
			q = (-b - discriminant)/float_type(2);
		else
			q = (-b + discriminant)/float_type(2);

		// compute t0 and t1
		float_type t0 = q / a;
		float_type t1 = c / q;

		if(t0 == t1)
			return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1, t0, rayStart + rayDir * t0);
		if(t0 < t1)
			return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1andT2, t0, rayStart + rayDir * t0, t1, rayStart + rayDir * t1);
		return RaySphereIntersectRec(RaySphereIntersectRec::Result::T1andT2, t1, rayStart + rayDir * t1, t0, rayStart + rayDir * t0);
	}

private:

	float_type _innerRadius, _outerRadius,
		_K, _scaleHeight;
	spectrum_type _waveLength, _beta;
};

}

#endif // __EXPLORE2_ATMOSPHERIC_H__