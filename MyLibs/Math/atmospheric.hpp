
#if !defined(__EXPLORE2_ATMOSPHERIC_H__)
#define __EXPLORE2_ATMOSPHERIC_H__

#include "Math/vector2.hpp"
#include "Math/vector3.hpp"
#include "Math/vector4.hpp"
#include "Math/matrix3.hpp"
#include "Math/matrix4.hpp"
#include "Math/transformation.hpp"

#define EPSILON							200.0
#define QUARTER_SPHERE_SAMPLES			50
#define ATMOSPHERE_TABLE_ANGLE_DEGREES	110.0

#undef min
#undef max

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

template < class FTy_ >
struct Atmosphere 
{
	typedef FTy_ float_type;
	typedef math::Vector3<float_type> spectrum_type;
	typedef math::Vector2<float_type> vec2_type;
	typedef math::Vector3<float_type> vec3_type;
	typedef math::Vector4<float_type> vec4_type;
	typedef math::Matrix3<float_type> mat3_type;
	typedef math::Matrix4<float_type> mat4_type;

	static const spectrum_type LightWavelengths;
	Atmosphere();

	void set_radii(float_type inner, float_type outer);

	// scale height is the thickness of the atmosphere its density
	// were uniform (7994m for Earth's atmosphere)
	void set_scale_height(float_type scaleHeight);

	// n is the index of refraction of the air
	// Ns is the molecular number density of the standard atmosphere
	void set_k(float_type n, float_type Ns);

	static float_type calculate_k(float_type n, float_type Ns);

	static vec3_type calculate_4PiKOverWavelength4(float_type n, float_type Ns, vec3_type wavelength);

	// wavelength of light entering the atmosphere
	void set_light_wavelength(const spectrum_type& wavelength);

	static float_type get_g(float_type u);

	const vec3_type& get_beta() const { return _beta; }

	void calculate_optical_depth_table(int samples, int angleSamples, int heightSamples);

	const std::vector<float_type> get_optical_depth_table() const ;

	void calculate_optical_depth_table(int samples, int angleSamples, int heightSamples, std::vector<float_type>& results) const;

 	void calculate_surface_lighting_table(const spectrum_type& Is, float_type mieu, 
		int PaPbSteps, int PPaSteps, int PPcSteps, 
		int angleSamples, int heightSamples, std::vector<spectrum_type>& results) const;

	void calculate_KFrThetaOverLambda4Rayleigh_table(spectrum_type waveLength, size_t samples);

	void calculate_KFrThetaOverLambda4Mie_table(float_type g, spectrum_type waveLength, size_t samples);

	const std::vector<spectrum_type>& get_KFrThetaOverLambda4Rayleigh_table() const { return _KFrThetaOverLambda4TableRayleigh; }

	const std::vector<spectrum_type>& get_KFrThetaOverLambda4Mie_table() const { return _KFrThetaOverLambda4TableMie; }

	void calculate_KFrThetaOverLambda4_table(float_type K, float_type g, spectrum_type waveLength, size_t samples, std::vector<spectrum_type>& table);

	void irradiance_at_view_Iv_table(const spectrum_type& Is, const vec3_type& eyePos, vec3_type upDir, const vec3_type& sunDir, float_type g, int xySamples, int PaPbSteps, int PPaSteps, int PPcSteps, std::vector<spectrum_type>& results) const;

	spectrum_type irradiance_at_view_Iv(const spectrum_type& Is, const vec3_type& sunDir, const vec3_type& eye, const vec3_type& eyeDir, float_type g, int PaPbSteps, int PPaSteps, int PPcSteps) const;

private:
	void create_quarter_sphere_samples();

	// calculates the scattering phase function Fr for a particular angle
	static inline float_type get_scattering_phase_Fr(float_type angle, float_type g);

	// calculates the density ratio rho (p) for a given height (h) and scale height (H0)
	static inline float_type get_density_ratio_rho(float_type h, float_type scaleHeight);

	static float_type calculate_optical_depth_height_angle(float_type height, float_type angleFromNormal, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int samples);

	// over height instead of position
	// beta - result of the beta function
	// starth - starting height
	// endh - ending height
	// distance - distance from start point (where height is starth) to end point
	// innerRadius - the radius of the planet at height = 0
	// scaleHeight - the set scaleHeight
	// samples - number of samples to use during integration (trapezium) 
	static float_type calculate_optical_depth(float_type starth, float_type endh, float_type distance, float_type innerRadius, float_type scaleHeight, int samples);

	static inline vec2_type calculate_end_pos(float_type starth, float_type endh, float_type radius, float_type distance);

	static float_type interpolate_height(float_type starth, float_type endh, float_type radius, float_type distance, float_type fraction);

	// Is - irradiance entering atmosphere
	// waveLength - wavelength of light
	// beta - result of the beta function
	// theta - angle between viewer and sun vectors
	// Pa - end of ray nearest viewer (could be view position or intersection with atmosphere)
	// Pb - end of ray furthest from viewer (could be intersection with solid object or atmosphere)
	spectrum_type calculate_irradiance_at_view_Iv(const spectrum_type& Is, const spectrum_type& waveLength, const spectrum_type& beta, const vec3_type& sunDir, vec3_type Pa, vec3_type Pb, float_type innerRadius, float_type outerRadius, float_type scaleHeight, float_type K, float_type g, int PaPbSteps, int PPaSteps, int PPcSteps) const;

	// solve the out scattering from Pc to Pa
	spectrum_type calculate_inner_integral(const spectrum_type& beta, vec3_type Pa, vec3_type Pb, const vec3_type& P, const vec3_type& sunDir, float_type innerRadius, float_type outerRadius, float_type scaleHeight, int PPaSteps, int PPcSteps) const;

	static inline void restrict_length(vec3_type& p, float_type len);

	static vec3_type calculate_Pc(vec3_type P, const vec3_type& sunDir, float_type outerRadius);

	struct RayCircleIntersectRec
	{
		struct Result { enum type {
			None,
			T1,
			T1andT2
		};};

		RayCircleIntersectRec(typename Result::type result_ = Result::None, float_type t1_ = 0, const vec2_type& p1_ = vec2_type(), float_type t2_ = 0, const vec2_type& p2_ = vec2_type()) 
			: result(result_), t1(t1_), t2(t2_), p1(p1_), p2(p2_) {}


		typename Result::type result;
		float_type t1, t2;
		vec2_type p1, p2;
	};

	// assumes circle is centered on origin
	// solution from http://stackoverflow.com/questions/1073336/circle-line-collision-detection
	static inline RayCircleIntersectRec circle_ray_isect(const vec2_type& rayStart, const vec2_type& rayDir, float_type radius);

	struct RaySphereIntersectRec
	{
		struct Result { enum type {
			None,
			T1,
			T1andT2
		};};

		RaySphereIntersectRec(typename Result::type result_ = Result::None, float_type t1_ = 0, const vec3_type& p1_ = vec3_type(), float_type t2_ = 0, const vec3_type& p2_ = vec3_type()) 
			: result(result_), t1(t1_), t2(t2_), p1(p1_), p2(p2_) {}

		typename Result::type result;
		float_type t1, t2;
		vec3_type p1, p2;
	};

	static inline RaySphereIntersectRec sphere_ray_isect(const vec3_type& rayStart, const vec3_type& rayDir, float_type radius);

	float_type interpolate_optical_depth(float_type angle, float_type height) const;

	spectrum_type interpolate_KFrThetaOverLambda4(float_type theta);

private:

	float_type _innerRadius, _outerRadius,
		_K, _scaleHeight;
	spectrum_type _waveLength, _beta;

	std::vector<vec3_type> _quarterSphereSamples;
	std::vector<spectrum_type> _KFrThetaOverLambda4TableMie,
		_KFrThetaOverLambda4TableRayleigh;
	std::vector<float_type> _opticalDepthTable;
	int _angleSamples, _heightSamples;
	float_type _angleFrac, _heightFrac;
	bool _innerIntegralTableAvailable;
};

#include "atmospheric.inl"

typedef Atmosphere<float> Atmospheref;
typedef Atmosphere<double> Atmosphered;

}

#endif // __EXPLORE2_ATMOSPHERIC_H__