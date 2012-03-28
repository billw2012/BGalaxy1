#ifndef __MATH_SPATIAL_NOISE_HPP__
#define __MATH_SPATIAL_NOISE_HPP__

#include "perlin.hpp"

namespace math {;

// this class generates fractal noise using the provided generator objects
template < 
	class FloatType,
	class SpectralComposer/* = SpectralComposerBase< FloatType >*/,
	class NoiseProvider = Noise<FloatType>
		//= SpectralComposerBase<NoiseProvider>
>
struct SpatialNoiseGenerator
{
	typedef FloatType value_type;
	typedef SpectralComposer spectral_composer;
	typedef NoiseProvider noise_provider;
	typedef SSETraits<FloatType> sse_traits;
	typedef typename sse_traits::sse_type sse_type;
	typedef Vector3<value_type> Vector3Type;
	typedef typename sse_traits::Vector SSEVectorType;
	typedef typename SIMDSoAVector3<value_type> SIMDSoAVector3Type;
private:
	typedef SpatialNoiseGenerator<FloatType, SpectralComposer, NoiseProvider> this_type;
	//typedef math::Vector3<value_type> Vector3Type;

	struct Providers
	{
		typedef boost::shared_ptr<Providers> ptr;
		boost::shared_ptr<spectral_composer> sc;
		noise_provider noise;
		Providers(const spectral_composer& sc_, const noise_provider& noise_) 
			: sc(sc_.clone()), noise(noise_) {}
	};

	struct ThreadData
	{
		ThreadData(const ThreadData& from)
			: noise(from.noise),
			exponents(from.exponents),
			lacunarity(from.lacunarity), 
			octaves(from.octaves),
			scale(from.scale), 
			center(from.center),
			octavesi(from.octavesi) 
		{
			if(from.sc)
				sc.reset(new spectral_composer(*from.sc)/*from.sc->clone()*/);
		}

		ThreadData() {}

		boost::shared_ptr<spectral_composer> sc;
		NoiseProvider noise;
		SSEVectorType exponents;
		value_type lacunarity, octaves;
		value_type scale, center;
		int octavesi;

		//ThreadData(const SpectralComposer* sc_, const NoiseProvider* noise_, misc::SIMDArray exponents_, value_type lacunarity_, value_type octaves_, value_type scale_, value_type center_, int octavesi_)
		//	:  sc(sc_), noise(noise_), exponents(exponents_), lacunarity(lacunarity_), octaves(octaves_), scale(scale_), center(center_), octavesi(octavesi_) {}
	};

	mutable std::vector<typename Providers::ptr> _providers;
	mutable std::vector<ThreadData> _threadData;
	//const SpectralComposer* _sc;
	//const NoiseProvider* _noise;
	//misc::SIMDArray _exponents;
	//value_type _lacunarity, _octaves;
	//value_type _scale, _center;
	//int _octavesi;
#ifdef ENABLE_SSE
	bool _useSSE;
#endif
	unsigned int _threadCount;

public:
	// constructor:
	// INPUTS:
	//  noisep - the simple noise generator object
	//  sc - the spectral composer object
	SpatialNoiseGenerator(const noise_provider& noisep, const spectral_composer* sc, 
		value_type lacunarity, value_type octaves, value_type h = 1.0);

#ifdef ENABLE_SSE
	void setSSE(bool sse)						{ _useSSE = sse; }
#endif

	bool sse() const							{ return _useSSE; }
	void setThreadCount(unsigned int threads)	{ _threadCount = std::max(1U, threads); }
	unsigned int threads() const				{ return _threadCount; }

	value_type operator()(const Vector3Type& vec) const;
	value_type operator()(value_type x, value_type y, value_type z) const;

	// generates fractal noise values for an array of input values
	// INPUTS: 
	//  xv - all the x coordinates
	//  yv - the y coordinates
	//  zv - the z coordinates
	//  vscale - a value to scale the coordinates by
	// OUTPUTS:
	//  results - the fractal noise values calculated from the inputs

	// finish converting to template choose between sse and sse2.
	void eval(SSEVectorType& results, const SIMDSoAVector3Type& v, 
		value_type vscale = 1) const;

	void setNoiseProvider(const noise_provider* noisep);

	void setSpectralComposer(const spectral_composer* sc);

private:
	value_type operator()(value_type x, value_type y, value_type z, 
		const ThreadData* data) const;

	void asynchCalculate(unsigned int count, value_type* results, 
		const value_type* x, const value_type* y, const value_type* z, 
		value_type vscale, const ThreadData* data) const;

#ifdef ENABLE_SSE
	// generates 4 fractal noise values
	sse_type eval(sse_type x, sse_type y, sse_type z, const ThreadData* data) const;

	//void callSSE( misc::SIMDArray &results, const misc::SIMDSoAVector3 &v, const float& vscale ) const
	void callSSE( unsigned int count, value_type* results, 
		const value_type* x, const value_type* y, const value_type* z, 
		value_type vscale, const ThreadData* data ) const;
#endif

	void callNonSSE( unsigned int count, value_type* results, 
		const value_type* x, const value_type* y, const value_type* z, 
		value_type vscale, const ThreadData* data ) const;

	// this function uses a random sampling of the noise generator to determine a center and scale
	// value which can be applied to 'normalize' the noise to a value of -0.5 to 0.5
	void determineCenterAndScale();
};

};

#include "spatialnoise.inl"

#endif // __MATH_SPATIAL_NOISE_HPP__