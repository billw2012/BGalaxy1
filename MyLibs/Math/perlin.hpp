#ifndef _MATH_PERLIN_HPP
#define _MATH_PERLIN_HPP

#include <stdlib.h>

#include <vector>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "Misc/libdesc.hpp"
#include "sse_utils.hpp"
// #include "Misc/simdtypes.hpp"

#undef min
#undef max

namespace math {;

// basic noise generator, always returns the same value for a given input
template < class FloatType >
struct Noise
{
	typedef FloatType value_type;
	typedef SSETraits< value_type > sse_traits;
	typedef typename sse_traits::sse_type sse_type;
private:
	int p[512];
	value_type ms_grad4[512];

public:
	Noise(const Noise& other);
	Noise(unsigned int seed = 0);
	void init(unsigned int seed);

	// 3d noise function
	value_type operator()(value_type x, value_type y, value_type z) const;

	// 2d noise function
	value_type operator()(value_type x, value_type y) const;

#ifdef MICROSOFT_SSE // ms compiler only
	// SSE version generates 4 noise values based on input
	sse_type operator()(sse_type x, sse_type y, sse_type z) const;

private:
	typename SSETraits< double >::sse_type eval(
		typename SSETraits< double >::sse_type x, 
		typename SSETraits< double >::sse_type y, 
		typename SSETraits< double >::sse_type z) const;
	typename SSETraits< float >::sse_type eval(
		typename SSETraits< float >::sse_type x, 
		typename SSETraits< float >::sse_type y, 
		typename SSETraits< float >::sse_type z) const;

	static inline typename SSETraits< double >::sse_type fadeSSE(
		typename SSETraits< double >::sse_type t);
	static inline typename SSETraits< float >::sse_type fadeSSE(
		typename SSETraits< float >::sse_type t);
	static inline sse_type lerpSSE(sse_type t, sse_type a, sse_type b);
#endif // MICROSOFT_SSE


private:
	static inline value_type fade(value_type t);
	static inline value_type lerp(value_type t, value_type a, value_type b);
};

// a 3d perlin function
// INPUTS:
//  n - octaves
//  p - persistence
//  noisep - the noise provider
//  vx,vy,vz - the 3d vector to calculate noise for
template < class ValueType, class NoiseProvider >
ValueType perlin(const NoiseProvider& noisep, 
				 ValueType vx, ValueType vy, ValueType vz, ValueType p, unsigned int n);

// a 2d perlin function
// INPUTS:
//  n - octaves
//  p - persistence
//  noisep - the noise provider
//  vx,vy - the 3d vector to calculate noise for
template < class ValueType, class NoiseProvider >
ValueType perlin(const NoiseProvider& noisep, ValueType vx, ValueType vy, 
				 ValueType p, unsigned int n);

// This class functions as a base class for all spectral composer classes, and provides the
// two pure virtual functions which need to be implemented.
//template <
//	class FloatType = float,
//	class NoiseProvider = Noise< FloatType >
//>
//struct SpectralComposerBase
//{
//	typedef FloatType value_type;
//	typedef SSETraits<FloatType> sse_traits;
//	typedef typename sse_traits::sse_type sse_type;
//	typedef Vector3<value_type> Vector3Type;
//	typedef typename sse_traits::Vector SSEVectorType;
//	typedef typename SIMDSoAVector3<value_type> SIMDSoAVector3Type;
//public:
//	// standard fractal noise function
//	virtual value_type operator()(const SSEVectorType& exponents, const NoiseProvider* noisep, 
//		value_type lacunarity, value_type octaves, value_type vx, value_type vy, value_type vz) const = 0;
//#ifdef ENABLE_SSE
//	// SSE version of the fractal noise function, note that floats must be passes as pointers to avoid float copies which can reset the registers
//	virtual sse_type operator()(const SSEVectorType& exponents, const NoiseProvider* noisep, 
//		const value_type* lacunarity, int octaves, sse_type vx, sse_type vy, sse_type vz) const = 0;
//#endif
//	SpectralComposerBase* clone() const
//	{
//		return clone_me();
//	}
////protected:
//private:
//	virtual SpectralComposerBase* clone_me() const = 0;
//};
//
//// this class implements SpectralComposerBase to provide ridged multifractal output based on F. Kenton Musgraves work.
//template < 
//	class FloatType,
//	class NoiseProvider = Noise< FloatType >
//>
//struct RidgedMultifractalProvider //: public SpectralComposerBase< FloatType, NoiseProvider >
//{
//	typedef FloatType value_type;
//	typedef SSETraits<FloatType> sse_traits;
//	typedef typename sse_traits::sse_type sse_type;
//	typedef Vector3<value_type> Vector3Type;
//	typedef typename sse_traits::Vector SSEVectorType;
//	typedef typename SIMDSoAVector3<value_type> SIMDSoAVector3Type;
//
//	typedef RidgedMultifractalProvider< FloatType, NoiseProvider > this_type;
//
//private:
//	value_type _offset, _gain;
//
//public:
//	RidgedMultifractalProvider(value_type offset = 1.0, value_type gain = 2.0)
//		: _offset(offset), _gain(gain)
//	{
//	}
//
//	value_type operator()(const SSEVectorType& exponents, const NoiseProvider* noisep,
//		value_type lacunarity, value_type octaves, 
//		value_type vx, value_type vy, value_type vz) const
//	{
//		value_type result, /*frequency, */signal, weight;
//
//		/* get first octave */
//		signal = (*noisep)( vx, vy, vz );
//		/* get absolute value of signal (this creates the ridges) */
//		if ( signal < 0.0 ) signal = -signal;
//		/* invert and translate (note that "offset" should be ~= 1.0) */
//		signal = _offset - signal;
//		/* square the signal, to increase "sharpness" of ridges */
//		signal *= signal;
//		/* assign initial values */
//		result = signal;
//		weight = 1.0;
//
//		for(int i = 1; i < octaves; ++i) {
//			/* increase the frequency */
//			vx *= lacunarity;
//			vy *= lacunarity;
//			vz *= lacunarity;
//
//			/* weight successive contributions by previous signal */
//			weight = signal * _gain;
//			if ( weight > 1.0 ) weight = 1.0;
//			if ( weight < 0.0 ) weight = 0.0;
//			signal = (*noisep)( vx, vy, vz );
//			if ( signal < 0.0 ) signal = -signal;
//			signal = _offset - signal;
//			signal *= signal;
//			/* weight the contribution */
//			signal *= weight;
//			result += signal * exponents[i];
//		}
//
//		return result;
//	}
//
//#ifdef ENABLE_SSE
//	sse_type operator()(const SSEVectorType& exponents, 
//		const NoiseProvider* noisep, const value_type* lacunarity, 
//		int octaves, sse_type vx, sse_type vy, sse_type vz) const
//	{
//		SSE_DOUBLE_LITERAL(_1, 1);
//		SSE_DOUBLE_LITERAL(_0, 0);
//#ifdef MICROSOFT_SSE
//		sse_type offsetSSE = sse_traits::load1(&_offset);
//		sse_type gainSSE = sse_traits::load1(&_gain);
//		/* get first octave */
//		sse_type signal = (*noisep)( vx, vy, vz );
//		/* get absolute value of signal (this creates the ridges) */
//		signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
//		/* invert and translate (note that "offset" should be ~= 1.0) */
//		signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
//		/* square the signal, to increase "sharpness" of ridges */
//		signal = sse_traits::mul(signal, signal);//signal *= signal;
//		/* assign initial values */
//		sse_type result = signal;
//		sse_type weight = SSE_LOAD_LITERALD(_1);//sse_traits::set1(1.0);
//
//		sse_type lacunaritySSE = sse_traits::load1(lacunarity);
//		for(int i = 1; i < octaves; ++i) {
//			/* increase the frequency */
//			vx = sse_traits::mul(vx, lacunaritySSE);//vx *= lacunarity;
//			vy = sse_traits::mul(vy, lacunaritySSE);//vy *= lacunarity;
//			vz = sse_traits::mul(vz, lacunaritySSE);//vz *= lacunarity;
//
//			/* weight successive contributions by previous signal */
//			weight = sse_traits::mul(signal, gainSSE);//weight = signal * _gain;
//			weight = sse_traits::min(weight, SSE_LOAD_LITERALD(_1)); //if ( weight > 1.0 ) weight = 1.0;
//			weight = sse_traits::max(weight, SSE_LOAD_LITERALD(_0)); //if ( weight < 0.0 ) weight = 0.0;
//			signal = (*noisep)( vx, vy, vz );
//			signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
//			signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
//			signal = sse_traits::mul(signal, signal);//signal *= signal;
//			/* weight the contribution */
//			signal = sse_traits::mul(signal, weight);//signal *= weight;
//			sse_type s_exp = sse_traits::mul(signal, sse_traits::load1(&exponents[i])); //result += signal * exponents[i];
//			result = sse_traits::add(result, s_exp);
//		}
//#endif
//		return result;
//	}
//#endif
////private:
////	virtual this_type* clone_me() const
////	{
////		return new RidgedMultifractalProvider(_offset, _gain);
////	}
//};

//// this class implements SpectralComposerBase to provide FBM style noise, based on F. Kenton Musgraves work.
//template < 
//	class NoiseProvider = Noise
//>
//struct FBMProvider : public SpectralComposerBase< NoiseProvider >
//{
//	typedef float value_type;
//	typedef FBMProvider< NoiseProvider > this_type;
//
//	value_type operator()(const misc::SIMDArray& exponents, const NoiseProvider* noisep, value_type lacunarity, value_type octaves, value_type vx, value_type vy, value_type vz) const
//	{
//		value_type result = 0.0, frequency = 1.0;
//
//		int i;
//		for(i = 0; i < octaves; i++) 
//		{
//			result += (*noisep)(vx, vy, vz) * exponents[i];
//			vx *= lacunarity;
//			vy *= lacunarity;
//			vz *= lacunarity;
//		} 
//
//		value_type remainder = octaves - (int)octaves;
//		if ( remainder > 0.0)      /* add in ``octaves''  remainder */
//			/* ``i''  and spatial freq. are preset in loop above */
//			result += remainder * (*noisep)(vx, vy, vz) * exponents[i];
//
//		return result;
//	}
//
//#ifdef ENABLE_SSE
//	virtual SSE_128f operator()(const misc::SIMDArray& exponents, const NoiseProvider* noisep, const value_type* lacunarity, int octaves, SSE_128f vx, SSE_128f vy, SSE_128f vz) const
//	{
//#ifdef MICROSOFT_SSE
//		SSE_128f result = _mm_set1_ps(0.0f);
//		SSE_128f lacunaritySSE = _mm_load1_ps(lacunarity);
//
//		for(int i = 0; i < octaves; ++i) 
//		{
//			SSE_128f signal = (*noisep)( vx, vy, vz );
//			SSE_128f s_exp = _mm_mul_ps(signal, _mm_load1_ps(&exponents[i])); 
//			result = _mm_add_ps(result, s_exp);
//
//			/* increase the frequency */
//			vx = _mm_mul_ps(vx, lacunaritySSE);//vx *= lacunarity;
//			vy = _mm_mul_ps(vy, lacunaritySSE);//vy *= lacunarity;
//			vz = _mm_mul_ps(vz, lacunaritySSE);//vz *= lacunarity;
//		}
//
//		// To-do: add fractional part of octaves here. 
//
//		return result;	
//#endif
//	};
//#endif
//private:
//	virtual SpectralComposerBase<>* clone_me() const
//	{
//		return new FBMProvider();
//	}
//};


}

#include "perlin.inl"

#endif // _MATH_PERLIN_HPP