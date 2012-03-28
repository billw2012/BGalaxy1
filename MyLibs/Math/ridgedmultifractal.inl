namespace math {;

template < class FTy_, class NPTy_ >
RidgedMultifractalProvider<FTy_, NPTy_>::RidgedMultifractalProvider(
	value_type offset, value_type gain)
	: _offset(offset), _gain(gain)
{
}

template < class FTy_, class NPTy_ >
typename RidgedMultifractalProvider<FTy_, NPTy_>::value_type 
RidgedMultifractalProvider<FTy_, NPTy_>::operator()(const SSEVectorType& exponents, 
					const noise_provider* noisep,
					value_type lacunarity, value_type octaves, 
					value_type vx, value_type vy, value_type vz) const
{
	value_type result, /*frequency, */signal, weight;

	/* get first octave */
	signal = (*noisep)( vx, vy, vz );
	/* get absolute value of signal (this creates the ridges) */
	if ( signal < 0.0 ) signal = -signal;
	/* invert and translate (note that "offset" should be ~= 1.0) */
	signal = _offset - signal;
	/* square the signal, to increase "sharpness" of ridges */
	signal *= signal;
	/* assign initial values */
	result = signal;
	weight = 1.0;

	for(int i = 1; i < octaves; ++i) {
		/* increase the frequency */
		vx *= lacunarity;
		vy *= lacunarity;
		vz *= lacunarity;

		/* weight successive contributions by previous signal */
		weight = signal * _gain;
		if ( weight > 1.0 ) weight = 1.0;
		if ( weight < 0.0 ) weight = 0.0;
		signal = (*noisep)( vx, vy, vz );
		if ( signal < 0.0 ) signal = -signal;
		signal = _offset - signal;
		signal *= signal;
		/* weight the contribution */
		signal *= weight;
		result += signal * exponents[i];
	}

	return result;
}

#ifdef ENABLE_SSE
template < class FTy_, class NPTy_ >
typename RidgedMultifractalProvider<FTy_, NPTy_>::sse_type 
RidgedMultifractalProvider<FTy_, NPTy_>::operator()(const SSEVectorType& exponents, 
													const noise_provider* noisep, const value_type* lacunarity, 
													int octaves, sse_type vx, sse_type vy, sse_type vz) const
{
	return eval(exponents, noisep, lacunarity, octaves, vx, vy, vz);
}

template < class FTy_, class NPTy_ >
typename SSETraits<double>::sse_type 
RidgedMultifractalProvider<FTy_, NPTy_>::eval(const SSETraits<double>::Vector& exponents, 
					const noise_provider* noisep, const value_type* lacunarity, 
					int octaves, typename SSETraits<double>::sse_type vx, 
					typename SSETraits<double>::sse_type vy, 
					typename SSETraits<double>::sse_type vz) const
{
	SSE_DOUBLE_LITERAL(_1, 1);
	SSE_DOUBLE_LITERAL(_0, 0);
#ifdef MICROSOFT_SSE
	sse_type offsetSSE = sse_traits::load1(&_offset);
	sse_type gainSSE = sse_traits::load1(&_gain);
	/* get first octave */
	sse_type signal = (*noisep)( vx, vy, vz );
	/* get absolute value of signal (this creates the ridges) */
	signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
	/* invert and translate (note that "offset" should be ~= 1.0) */
	signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
	/* square the signal, to increase "sharpness" of ridges */
	signal = sse_traits::mul(signal, signal);//signal *= signal;
	/* assign initial values */
	sse_type result = signal;
	sse_type weight = SSE_LOAD_LITERALD(_1);//sse_traits::set1(1.0);

	sse_type lacunaritySSE = sse_traits::load1(lacunarity);
	for(int i = 1; i < octaves; ++i) {
		/* increase the frequency */
		vx = sse_traits::mul(vx, lacunaritySSE);//vx *= lacunarity;
		vy = sse_traits::mul(vy, lacunaritySSE);//vy *= lacunarity;
		vz = sse_traits::mul(vz, lacunaritySSE);//vz *= lacunarity;

		/* weight successive contributions by previous signal */
		weight = sse_traits::mul(signal, gainSSE);//weight = signal * _gain;
		weight = sse_traits::min(weight, SSE_LOAD_LITERALD(_1)); //if ( weight > 1.0 ) weight = 1.0;
		weight = sse_traits::max(weight, SSE_LOAD_LITERALD(_0)); //if ( weight < 0.0 ) weight = 0.0;
		signal = (*noisep)( vx, vy, vz );
		signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
		signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
		signal = sse_traits::mul(signal, signal);//signal *= signal;
		/* weight the contribution */
		signal = sse_traits::mul(signal, weight);//signal *= weight;
		sse_type s_exp = sse_traits::mul(signal, sse_traits::load1(&exponents[i])); //result += signal * exponents[i];
		result = sse_traits::add(result, s_exp);
	}
#endif
	return result;
}

template < class FTy_, class NPTy_ >
typename SSETraits<float>::sse_type 
RidgedMultifractalProvider<FTy_, NPTy_>::eval(const SSETraits<float>::Vector& exponents, 
											  const noise_provider* noisep, const value_type* lacunarity, 
											  int octaves, typename SSETraits<float>::sse_type vx, 
											  typename SSETraits<float>::sse_type vy, 
											  typename SSETraits<float>::sse_type vz) const
{
	SSE_FLOAT_LITERAL(_1, 1);
	SSE_FLOAT_LITERAL(_0, 0);
#ifdef MICROSOFT_SSE
	sse_type offsetSSE = sse_traits::load1(&_offset);
	sse_type gainSSE = sse_traits::load1(&_gain);
	/* get first octave */
	sse_type signal = (*noisep)( vx, vy, vz );
	/* get absolute value of signal (this creates the ridges) */
	signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
	/* invert and translate (note that "offset" should be ~= 1.0) */
	signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
	/* square the signal, to increase "sharpness" of ridges */
	signal = sse_traits::mul(signal, signal);//signal *= signal;
	/* assign initial values */
	sse_type result = signal;
	sse_type weight = SSE_LOAD_LITERALF(_1);//sse_traits::set1(1.0);

	sse_type lacunaritySSE = sse_traits::load1(lacunarity);
	for(int i = 1; i < octaves; ++i) {
		/* increase the frequency */
		vx = sse_traits::mul(vx, lacunaritySSE);//vx *= lacunarity;
		vy = sse_traits::mul(vy, lacunaritySSE);//vy *= lacunarity;
		vz = sse_traits::mul(vz, lacunaritySSE);//vz *= lacunarity;

		/* weight successive contributions by previous signal */
		weight = sse_traits::mul(signal, gainSSE);//weight = signal * _gain;
		weight = sse_traits::min(weight, SSE_LOAD_LITERALF(_1)); //if ( weight > 1.0 ) weight = 1.0;
		weight = sse_traits::max(weight, SSE_LOAD_LITERALF(_0)); //if ( weight < 0.0 ) weight = 0.0;
		signal = (*noisep)( vx, vy, vz );
		signal = sse_traits::abs(signal);//if ( signal < 0.0 ) signal = -signal;
		signal = sse_traits::sub(offsetSSE, signal);//signal = _offset - signal;
		signal = sse_traits::mul(signal, signal);//signal *= signal;
		/* weight the contribution */
		signal = sse_traits::mul(signal, weight);//signal *= weight;
		sse_type s_exp = sse_traits::mul(signal, sse_traits::load1(&exponents[i])); //result += signal * exponents[i];
		result = sse_traits::add(result, s_exp);
	}
#endif
	return result;
}
#endif

}