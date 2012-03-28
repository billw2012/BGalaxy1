
namespace math {;

template < class FTy_, class SCTy_, class NPTy_ >
SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::SpatialNoiseGenerator(
						const noise_provider& noisep, const spectral_composer* sc, 
						value_type lacunarity, value_type octaves, value_type h = 1.0) 
					  : _threadData(1),
#ifdef ENABLE_SSE
					  _useSSE(true), 
#else
					  _useSSE(false), 
#endif
					  _threadCount(1)
{
	//assert(noisep != NULL);
	assert(sc != NULL);

	_threadData[0].sc.reset(new spectral_composer(*sc));
	_threadData[0].noise = noisep;
	_threadData[0].lacunarity = lacunarity;
	_threadData[0].octaves = octaves;
	_threadData[0].octavesi = static_cast<int>(octaves);
	_threadData[0].exponents.resize(static_cast<unsigned int>(octaves+1));

	value_type frequency = 1.0;
	for (int i=0; i<=octaves; i++) {
		// compute weight for each frequency
		_threadData[0].exponents[i] = ::pow( frequency, -h );
		frequency *= lacunarity;
	}
	determineCenterAndScale();
}

template < class FTy_, class SCTy_, class NPTy_ >
typename SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::value_type 
SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::operator()(const Vector3Type& vec) const
{
	return (*this)(vec.x, vec.y, vec.z);
}

template < class FTy_, class SCTy_, class NPTy_ >
typename SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::value_type 
SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::operator()(value_type x, value_type y, value_type z) const
{
	const ThreadData* data = &_threadData[0];
	return ((*(data->sc))(data->exponents, &data->noise, data->lacunarity, 
		data->octaves, x, y, z) - data->center) * data->scale;
}

// finish converting to template choose between sse and sse2.
template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::eval(SSEVectorType& results, const SIMDSoAVector3Type& v, 
		  value_type vscale = 1) const
{
	results.resize(v.size());
	if(v.size() == 0)
		return ;

	if(_threadCount == 1)
		asynchCalculate(static_cast<unsigned int>(v.size()), &results[0], 
		&v.x(0), &v.y(0), &v.z(0), vscale, &_threadData[0]);
	else
	{
		// resize thread data array so each thread can have its own copy
		while(_threadData.size() < _threadCount)
		{
			_threadData.push_back(_threadData[0]);
			//_providers.push_back(Providers::ptr(new Providers(*_threadData[0].sc, *_threadData[0].noise)));
			//_threadData.back().sc = _providers.back()->sc.get();
			//_threadData.back().noise = &_providers.back()->noise;
		}

		typedef boost::shared_ptr< boost::thread > ThreadPtr;
		std::vector< ThreadPtr > threads;

		unsigned int elementsPerThread = static_cast<unsigned int>((v.size() / 
			(_threadCount * sse_traits::VEC_SIZE)) * sse_traits::VEC_SIZE);
		unsigned int totalElements = 0, threadIdx = 0;
		for( ; threadIdx < _threadCount - 1; ++threadIdx, totalElements += elementsPerThread)
		{
			if(_useSSE)
			{
				ThreadPtr newThread(new boost::thread(boost::bind(&this_type::callSSE, 
					this, elementsPerThread, &results[totalElements],
					&v.x(totalElements), &v.y(totalElements), &v.z(totalElements), 
					vscale, &_threadData[threadIdx])));
				threads.push_back(newThread);
			}
			else
			{
				ThreadPtr newThread(new boost::thread(boost::bind(&this_type::callNonSSE, 
					this, elementsPerThread, &results[totalElements], 
					&v.x(totalElements), &v.y(totalElements), &v.z(totalElements), 
					vscale, &_threadData[threadIdx])));
				threads.push_back(newThread);
			}
		}
		if(_useSSE)
		{
			callSSE(static_cast<unsigned int>(v.size()) - totalElements, 
				&results[totalElements], 
				&v.x(totalElements), &v.y(totalElements), &v.z(totalElements), 
				vscale, &_threadData[threadIdx]);
		}
		else
		{
			callNonSSE(static_cast<unsigned int>(v.size()) - totalElements, 
				&results[totalElements], 
				&v.x(totalElements), &v.y(totalElements), &v.z(totalElements), 
				vscale, &_threadData[threadIdx]);
		}

		for(std::vector< ThreadPtr >::iterator tItr = threads.begin(); 
			tItr != threads.end(); ++tItr)
		{
			if((*tItr)->joinable())
				(*tItr)->join();
		}
		threads.clear();
	}
}

template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::setNoiseProvider(
	const noise_provider* noisep)
{
	assert(noisep != NULL);
	_threadData[0].noise = noisep;
	for(std::vector<Providers::ptr>::iterator itr = _providers.begin(); 
		itr != _providers.end(); ++itr)
	{
		(*itr)->noise = *noisep;
	}
	determineCenterAndScale();
}

template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::setSpectralComposer(
	const spectral_composer* sc)
{
	assert(sc != NULL);
	_threadData[0].sc = sc;
	for(std::vector<Providers::ptr>::iterator itr = _providers.begin(); 
		itr != _providers.end(); ++itr)
	{
		(*itr)->sc = *sc;
	}
	determineCenterAndScale();
}

template < class FTy_, class SCTy_, class NPTy_ >
typename SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::value_type 
SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::operator()(value_type x, value_type y, value_type z, const ThreadData* data) const
{
	return ((*data->sc)(data->exponents, &data->noise, data->lacunarity, 
		data->octaves, x, y, z) - data->center) * data->scale;
}

template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::asynchCalculate(unsigned int count, value_type* results, 
	const value_type* x, const value_type* y, const value_type* z, 
	value_type vscale, const ThreadData* data) const
{
#ifdef ENABLE_SSE
	if(_useSSE)
		callSSE(count, results, x, y, z, vscale, data);
	else
#endif
		callNonSSE(count, results, x, y, z, vscale, data);
}

#ifdef ENABLE_SSE
// generates 4 fractal noise values
template < class FTy_, class SCTy_, class NPTy_ >
typename SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::sse_type 
SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::eval(sse_type x, sse_type y, sse_type z, const ThreadData* data) const
{
	//#ifdef MICROSOFT_SSE
	// call the fractal noise function
	sse_type result = (*data->sc)(data->exponents, &data->noise, 
		&data->lacunarity, data->octavesi, x, y, z);
	// load the center and scale
	sse_type centerSSE = sse_traits::load1(&data->center); // move these out of the inner loop!
	sse_type scaleSSE = sse_traits::load1(&data->scale);
	// apply the center and scale to 'normalize' the noise 
	result = sse_traits::sub(result, centerSSE);
	result = sse_traits::mul(result, scaleSSE);
	return result;
	//#endif
}

//void callSSE( misc::SIMDArray &results, const misc::SIMDSoAVector3 &v, const float& vscale ) const
template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::callSSE( unsigned int count, value_type* results, 
	const value_type* x, const value_type* y, const value_type* z, 
	value_type vscale, const ThreadData* data ) const
{
	// how many sets of four can we calculate?
	size_t fours = count / sse_traits::VEC_SIZE;
	// how many remaining calculations will there be?
	size_t remainder = count - (fours * sse_traits::VEC_SIZE);
	//// pre-size the output array
	//results.resize(v.size());

#ifdef MICROSOFT_SSE
	_mm_prefetch(reinterpret_cast<const char*>(x), _MM_HINT_T0);
	_mm_prefetch(reinterpret_cast<const char*>(y), _MM_HINT_T0);
	_mm_prefetch(reinterpret_cast<const char*>(z), _MM_HINT_T0);
	// load the scale value
	sse_type vscaleSSE = sse_traits::load1(&vscale);
	// save the current rounding mode
	//int rmode = _MM_GET_ROUNDING_MODE();
	// switch to round down rounding mode so that the SSE floor function works correctly
	//_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);
	// calculate all our sets of four
	for(size_t i = 0; i < fours; ++i)
	{
		// load our data
		sse_type xv = sse_traits::pack_vals_aligned(x + i*sse_traits::VEC_SIZE);
		sse_type yv = sse_traits::pack_vals_aligned(y + i*sse_traits::VEC_SIZE);
		sse_type zv = sse_traits::pack_vals_aligned(z + i*sse_traits::VEC_SIZE);
		// scale by the required scale value
		xv = sse_traits::mul(xv, vscaleSSE);
		yv = sse_traits::mul(yv, vscaleSSE);
		zv = sse_traits::mul(zv, vscaleSSE);
		// calculate our result (call the fractal noise function)
		sse_type result = this->eval(xv, yv, zv, data);
		// store our results
		sse_traits::stream(results + i*sse_traits::VEC_SIZE, result);
		//unpackFloatsAligned(result, &results[i*4]);
	}
	// if there are some values still to calculate do it
	if(remainder > 0)
	{
		_MM_ALIGN16 value_type remainsx[sse_traits::VEC_SIZE], 
			remainsy[sse_traits::VEC_SIZE], remainsz[sse_traits::VEC_SIZE];
		for(size_t i = 0, j = count-remainder; i < remainder; ++i, ++j)
		{
			remainsx[i] = x[j];
			remainsy[i] = y[j];
			remainsz[i] = z[j];
		}
		// EMMS here as the loop above may reset the register states
		_mm_empty();
		// load our remaining data
		sse_type xv = sse_traits::pack_vals_aligned(remainsx);
		sse_type yv = sse_traits::pack_vals_aligned(remainsy);
		sse_type zv = sse_traits::pack_vals_aligned(remainsz);
		// scale 
		xv = sse_traits::mul(xv, vscaleSSE);
		yv = sse_traits::mul(yv, vscaleSSE);
		zv = sse_traits::mul(zv, vscaleSSE);
		// call the noise function
		sse_type result = this->eval(xv, yv, zv, data);
		// store our results
		_MM_ALIGN16 value_type fvs[sse_traits::VEC_SIZE];
		sse_traits::unpack_vals_aligned(result, fvs);
		_mm_empty();
		for(size_t i = 0, j = count-remainder; i < remainder; ++i, ++j)
		{
			results[j] = fvs[i];
		}
	}
	// reset the rounding mode to what it was before
	//_MM_SET_ROUNDING_MODE(rmode);
	// empty the registers
	_mm_empty();
#endif
}
#endif

template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::callNonSSE( unsigned int count, value_type* results, 
	const value_type* x, const value_type* y, const value_type* z, 
	value_type vscale, const ThreadData* data ) const
{
	//results.resize(v.size());
	for(unsigned int i=0; i<count; ++i)
	{
		results[i] = (*this)(x[i] * vscale, y[i] * vscale, z[i] * vscale, data);
	}
}

// this function uses a random sampling of the noise generator to determine a center and scale
// value which can be applied to 'normalize' the noise to a value of -0.5 to 0.5
template < class FTy_, class SCTy_, class NPTy_ >
void SpatialNoiseGenerator<FTy_, SCTy_, NPTy_>::determineCenterAndScale()
{
	_threadData[0].scale = 1.0;
	_threadData[0].center = 0.0;
	value_type minh=std::numeric_limits<value_type>::max(), 
		maxh=std::numeric_limits<value_type>::min();
	for(unsigned int i = 0; i < 1000; ++i)
	{
		value_type rx = static_cast<value_type>(rand()/static_cast<value_type>(RAND_MAX) - 0.5);
		value_type ry = static_cast<value_type>(rand()/static_cast<value_type>(RAND_MAX) - 0.5);
		value_type rz = static_cast<value_type>(rand()/static_cast<value_type>(RAND_MAX) - 0.5);
		value_type h = (*this)(math::Vector3<value_type>(rx, ry, rz).normal());
		minh = MY_MIN(h, minh);
		maxh = MY_MAX(h, maxh);
	}

	_threadData[0].scale = 1 / (maxh - minh);
	_threadData[0].center = static_cast<value_type>((maxh + minh) * 0.5);
}

}