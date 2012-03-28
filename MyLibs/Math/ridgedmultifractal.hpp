#ifndef __MATH_RIDGED_MULTI_FRACTAL_HPP__
#define __MATH_RIDGED_MULTI_FRACTAL_HPP__

#include "spatialnoise.hpp"

namespace math {;


// this class implements SpectralComposerBase to provide ridged multifractal output based on F. Kenton Musgraves work.
template < 
	class FloatType,
	class NoiseProvider = Noise< FloatType >
>
struct RidgedMultifractalProvider //: public SpectralComposerBase< FloatType, NoiseProvider >
{
	typedef FloatType value_type;
	typedef NoiseProvider noise_provider;

	typedef SSETraits<FloatType> sse_traits;
	typedef typename sse_traits::sse_type sse_type;
	typedef Vector3<value_type> Vector3Type;
	typedef typename sse_traits::Vector SSEVectorType;
	typedef typename SIMDSoAVector3<value_type> SIMDSoAVector3Type;

	typedef RidgedMultifractalProvider< FloatType, NoiseProvider > this_type;

private:
	value_type _offset, _gain;

public:
	RidgedMultifractalProvider(value_type offset = 1.0, value_type gain = 2.0);

	value_type operator()(const SSEVectorType& exponents, const noise_provider* noisep,
		value_type lacunarity, value_type octaves, 
		value_type vx, value_type vy, value_type vz) const;

#ifdef ENABLE_SSE
	sse_type operator()(const SSEVectorType& exponents, 
		const noise_provider* noisep, const value_type* lacunarity, 
		int octaves, sse_type vx, sse_type vy, sse_type vz) const;
	typename SSETraits<double>::sse_type eval(
		const SSETraits<double>::Vector& exponents, 
		const noise_provider* noisep, const value_type* lacunarity, 
		int octaves, typename SSETraits<double>::sse_type vx, 
		typename SSETraits<double>::sse_type vy, 
		typename SSETraits<double>::sse_type vz) const;
	typename SSETraits<float>::sse_type eval(
		const SSETraits<float>::Vector& exponents, 
		const noise_provider* noisep, const value_type* lacunarity, 
		int octaves, typename SSETraits<float>::sse_type vx, 
		typename SSETraits<float>::sse_type vy, 
		typename SSETraits<float>::sse_type vz) const;
#endif
	//private:
	//	virtual this_type* clone_me() const
	//	{
	//		return new RidgedMultifractalProvider(_offset, _gain);
	//	}
};

}

#include "ridgedmultifractal.inl"

#endif // __MATH_RIDGED_MULTI_FRACTAL_HPP__