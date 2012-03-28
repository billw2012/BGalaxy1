namespace math {;

template < class FloatType >
Noise<FloatType>::Noise(const Noise& other)
{
	memcpy(p, other.p, sizeof(int)*512);
	memcpy(ms_grad4, other.ms_grad4, sizeof(value_type)*512);
}

template < class FloatType >
Noise<FloatType>::Noise(unsigned int seed)
{
	init(seed);
}

template < class FloatType >
void Noise<FloatType>::init(unsigned int seed)
{
	srand(seed);
	for (int i=0; i < 256 ; i++) 
		p[256+i] = p[i] = rand() % 255;

	float kkf[256];
	for (int i = 0; i < 256; i++)
		kkf[i] = -1.0f + 2.0f * ((float)i / 255.0f);

	for (int i = 0; i < 256; i++)
		ms_grad4[i] = kkf[p[i]] * (1.0f/1.4142135623730950488016887242097f);

	for (int i = 256; i < 512; i++)
		ms_grad4[i] = ms_grad4[i & 255]; 
}

// 3d noise function
template < class FloatType >
typename Noise<FloatType>::value_type Noise<FloatType>::operator()(
	value_type x, value_type y, value_type z) const
{
	int fX = (int)floor(x);
	int fY = (int)floor(y);
	int fZ = (int)floor(z);

	int X = fX & 255;
	int	Y = fY & 255;
	int	Z = fZ & 255;

	x -= (value_type)fX;
	y -= (value_type)fY;
	z -= (value_type)fZ;

	value_type u = fade(x),                              // COMPUTE FADE CURVES
		v = fade(y),                                // FOR EACH OF X,Y,Z.
		w = fade(z);
	int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z;   // HASH COORDINATES 
	int	B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;   // THE 8 CUBE 

	return(lerp(w, 
		lerp(v, lerp(u, ms_grad4[AA], ms_grad4[BA]), lerp(u, ms_grad4[AB], ms_grad4[BB])),
		lerp(v, lerp(u, ms_grad4[AA + 1], ms_grad4[BA + 1]), 
		lerp(u, ms_grad4[AB + 1], ms_grad4[BB + 1])))); 
}

// 2d noise function
template < class FloatType >
typename Noise<FloatType>::value_type Noise<FloatType>::operator()(value_type x, value_type y) const
{
	int fX = floor(x);
	int fY = floor(y);

	int X = fX & 255;
	int	Y = fY & 255;

	x -= (value_type)fX;
	y -= (value_type)fY;

	value_type u = fade(x),  // COMPUTE FADE CURVES
		v = fade(y);
	int A = p[X  ]+Y;   // HASH COORDINATES 
	int	B = p[X+1]+Y;  

	return(lerp(v, 
		lerp(u, ms_grad4[A], ms_grad4[B]), 
		lerp(u, ms_grad4[A+1], ms_grad4[B+1]))); 
}

#ifdef MICROSOFT_SSE // ms compiler only
// SSE version generates 4 noise values based on input
template <class FloatType>
typename Noise<FloatType>::sse_type Noise<FloatType>::operator()(
	sse_type x, sse_type y, sse_type z) const
{
	return eval(x, y, z);
}

template <class FloatType>
typename SSETraits< double >::sse_type Noise<FloatType>::eval(
	typename SSETraits< double >::sse_type x, 
	typename SSETraits< double >::sse_type y, 
	typename SSETraits< double >::sse_type z) const
{
	_mm_prefetch(reinterpret_cast<const char *>(ms_grad4), _MM_HINT_T0);
	sse_type fX = sse_traits::floor(x);
	sse_type fY = sse_traits::floor(y);
	sse_type fZ = sse_traits::floor(z);

	// get the fractional part of the vectors
	x = sse_traits::sub(x, fX); // x -= (float)fX;
	y = sse_traits::sub(y, fY); // y -= (float)fY;
	z = sse_traits::sub(z, fZ); // z -= (float)fZ;

	int X[2]; //= fX & 255;
	int	Y[2]; //= fY & 255;
	int	Z[2]; //= fZ & 255;

	sse_traits::extract_int32s(fX, X[0], X[1]);
	sse_traits::extract_int32s(fY, Y[0], Y[1]);
	sse_traits::extract_int32s(fZ, Z[0], Z[1]);

	//for(int i=0; i<4; ++i)	{  }

	sse_type u = fadeSSE(x), v = fadeSSE(y), w = fadeSSE(z);
	int AA[2], AB[2], BA[2], BB[2];
	for(int i = 0; i < 2; ++i)
	{
		X[i] &= 255; Y[i] &= 255; Z[i] &= 255;
		int A = p[X[i]]+Y[i];
		AA[i] = p[A]+Z[i];
		AB[i] = p[A+1]+Z[i];
		int B = p[X[i]+1]+Y[i];
		BA[i] = p[B]+Z[i];
		BB[i] = p[B+1]+Z[i];
	}

	// the following code could be interspliced more to take advantage of loading consecutive values from the ms_grad4 array
	// first lerp AA to BA using u
	sse_type ms_grad41 = sse_traits::pack_vals(&ms_grad4[AA[0]], &ms_grad4[AA[1]]);
	sse_type ms_grad42 = sse_traits::pack_vals(&ms_grad4[BA[0]], &ms_grad4[BA[1]]);
	sse_type uAABA = lerpSSE(u, ms_grad41, ms_grad42);
	// lerp AB to BB using u
	sse_type ms_grad43 = sse_traits::pack_vals(&ms_grad4[AB[0]], &ms_grad4[AB[1]]);
	sse_type ms_grad44 = sse_traits::pack_vals(&ms_grad4[BB[0]], &ms_grad4[BB[1]]);
	sse_type uABBB = lerpSSE(u, ms_grad43, ms_grad44);
	// lerp AABA to ABBB using v
	sse_type vAABA_ABBB = lerpSSE(v, uAABA, uABBB);

	// lerp AA+1 to BA+1 using u
	sse_type ms_grad45 = sse_traits::pack_vals(&ms_grad4[AA[0]+1], &ms_grad4[AA[1]+1]);
	sse_type ms_grad46 = sse_traits::pack_vals(&ms_grad4[BA[0]+1], &ms_grad4[BA[1]+1]);
	sse_type uAABA1 = lerpSSE(u, ms_grad45, ms_grad46);
	// lerp AB+1 to BB+1 using u
	sse_type ms_grad47 = sse_traits::pack_vals(&ms_grad4[AB[0]+1], &ms_grad4[AB[1]+1]);
	sse_type ms_grad48 = sse_traits::pack_vals(&ms_grad4[BB[0]+1], &ms_grad4[BB[1]+1]);
	sse_type uABBB1 = lerpSSE(u, ms_grad47, ms_grad48);
	// lerp AABA+1 to ABBB+1 using v
	sse_type vAABA1_ABBB1 = lerpSSE(v, uAABA1, uABBB1);
	// lerp AABA_ABBB to AABA+1_ABBB+1 using w
	sse_type finalval = lerpSSE(w, vAABA_ABBB, vAABA1_ABBB1);
	return finalval;
}

template <class FloatType>
typename SSETraits< float >::sse_type Noise<FloatType>::eval(
	typename SSETraits< float >::sse_type x, 
	typename SSETraits< float >::sse_type y, 
	typename SSETraits< float >::sse_type z) const
{
	_mm_prefetch(reinterpret_cast<const char *>(ms_grad4), _MM_HINT_T0);
	sse_type fX = sse_traits::floor(x);
	sse_type fY = sse_traits::floor(y);
	sse_type fZ = sse_traits::floor(z);

	// get the fractional part of the vectors
	x = sse_traits::sub(x, fX); // x -= (float)fX;
	y = sse_traits::sub(y, fY); // y -= (float)fY;
	z = sse_traits::sub(z, fZ); // z -= (float)fZ;

	int X[4]; //= fX & 255;
	int	Y[4]; //= fY & 255;
	int	Z[4]; //= fZ & 255;

	sse_traits::extract_int32s(fX, X[0], X[1], X[2], X[3]);
	sse_traits::extract_int32s(fY, Y[0], Y[1], Y[2], Y[3]);
	sse_traits::extract_int32s(fZ, Z[0], Z[1], Z[2], Z[3]);

	//for(int i=0; i<4; ++i)	{  }

	sse_type u = fadeSSE(x), v = fadeSSE(y), w = fadeSSE(z);
	int AA[4], AB[4], BA[4], BB[4];
	for(int i = 0; i < 4; ++i)
	{
		X[i] &= 255; Y[i] &= 255; Z[i] &= 255;
		int A = p[X[i]]+Y[i];
		AA[i] = p[A]+Z[i];
		AB[i] = p[A+1]+Z[i];
		int B = p[X[i]+1]+Y[i];
		BA[i] = p[B]+Z[i];
		BB[i] = p[B+1]+Z[i];
	}

	// the following code could be interspliced more to take advantage of loading consecutive values from the ms_grad4 array
	// first lerp AA to BA using u
	sse_type ms_grad41 = sse_traits::pack_vals(&ms_grad4[AA[0]], &ms_grad4[AA[1]],
		&ms_grad4[AA[2]], &ms_grad4[AA[3]]);
	sse_type ms_grad42 = sse_traits::pack_vals(&ms_grad4[BA[0]], &ms_grad4[BA[1]],
		&ms_grad4[BA[2]], &ms_grad4[BA[3]]);
	sse_type uAABA = lerpSSE(u, ms_grad41, ms_grad42);
	// lerp AB to BB using u
	sse_type ms_grad43 = sse_traits::pack_vals(&ms_grad4[AB[0]], &ms_grad4[AB[1]],
		&ms_grad4[AB[2]], &ms_grad4[AB[3]]);
	sse_type ms_grad44 = sse_traits::pack_vals(&ms_grad4[BB[0]], &ms_grad4[BB[1]],
		&ms_grad4[BB[2]], &ms_grad4[BB[3]]);
	sse_type uABBB = lerpSSE(u, ms_grad43, ms_grad44);
	// lerp AABA to ABBB using v
	sse_type vAABA_ABBB = lerpSSE(v, uAABA, uABBB);

	// lerp AA+1 to BA+1 using u
	sse_type ms_grad45 = sse_traits::pack_vals(&ms_grad4[AA[0]+1], &ms_grad4[AA[1]+1],
		&ms_grad4[AA[2]+1], &ms_grad4[AA[3]+1]);
	sse_type ms_grad46 = sse_traits::pack_vals(&ms_grad4[BA[0]+1], &ms_grad4[BA[1]+1],
		&ms_grad4[BA[2]+1], &ms_grad4[BA[3]+1]);
	sse_type uAABA1 = lerpSSE(u, ms_grad45, ms_grad46);
	// lerp AB+1 to BB+1 using u
	sse_type ms_grad47 = sse_traits::pack_vals(&ms_grad4[AB[0]+1], &ms_grad4[AB[1]+1],
		&ms_grad4[AB[2]+1], &ms_grad4[AB[3]+1]);
	sse_type ms_grad48 = sse_traits::pack_vals(&ms_grad4[BB[0]+1], &ms_grad4[BB[1]+1],
		&ms_grad4[BB[2]+1], &ms_grad4[BB[3]+1]);
	sse_type uABBB1 = lerpSSE(u, ms_grad47, ms_grad48);
	// lerp AABA+1 to ABBB+1 using v
	sse_type vAABA1_ABBB1 = lerpSSE(v, uAABA1, uABBB1);
	// lerp AABA_ABBB to AABA+1_ABBB+1 using w
	sse_type finalval = lerpSSE(w, vAABA_ABBB, vAABA1_ABBB1);
	return finalval;
}

template < class FloatType >
inline typename SSETraits<double>::sse_type Noise<FloatType>::fadeSSE(
	typename SSETraits<double>::sse_type t)
{ 
	SSE_DOUBLE_LITERAL(_6, 6.0);
	SSE_DOUBLE_LITERAL(_15, 15.0);
	SSE_DOUBLE_LITERAL(_10, 10.0);

	typename SSETraits<double>::sse_type t6 = SSETraits<double>::mul(t, SSE_LOAD_LITERALD(_6));
	t6 = SSETraits<double>::sub(t6, SSE_LOAD_LITERALD(_15));
	t6 = SSETraits<double>::mul(t6, t);
	t6 = SSETraits<double>::add(t6, SSE_LOAD_LITERALD(_10));
	t6 = SSETraits<double>::mul(t6, t);
	t6 = SSETraits<double>::mul(t6, t);
	t6 = SSETraits<double>::mul(t6, t);
	return t6;
}

template < class FloatType >
inline typename SSETraits<float>::sse_type Noise<FloatType>::fadeSSE(
	typename SSETraits<float>::sse_type t)
{ 
	SSE_FLOAT_LITERAL(_6, 6.0);
	SSE_FLOAT_LITERAL(_15, 15.0);
	SSE_FLOAT_LITERAL(_10, 10.0);

	typename SSETraits<float>::sse_type t6 = SSETraits<float>::mul(t, SSE_LOAD_LITERALF(_6));
	t6 = SSETraits<float>::sub(t6, SSE_LOAD_LITERALF(_15));
	t6 = SSETraits<float>::mul(t6, t);
	t6 = SSETraits<float>::add(t6, SSE_LOAD_LITERALF(_10));
	t6 = SSETraits<float>::mul(t6, t);
	t6 = SSETraits<float>::mul(t6, t);
	t6 = SSETraits<float>::mul(t6, t);
	return t6;
}

template < class FloatType >
inline typename Noise<FloatType>::sse_type Noise<FloatType>::lerpSSE(
	sse_type t, sse_type a, sse_type b) 
{ 
	b = sse_traits::sub(b, a);
	b = sse_traits::mul(b, t);
	return sse_traits::add(a, b); 
}

#endif // MICROSOFT_SSE

template < class FloatType >
inline typename Noise<FloatType>::value_type Noise<FloatType>::fade(value_type t)
{ 
	return (t * t * t * (t * (t * (value_type)6 - (value_type)15) + (value_type)10)); 
}

template < class FloatType >
inline typename Noise<FloatType>::value_type Noise<FloatType>::lerp(value_type t, value_type a, value_type b) 
{ 
	return a + t * (b - a); 
}

template < class ValueType, class NoiseProvider >
ValueType perlin(const NoiseProvider& noisep, ValueType vx, ValueType vy, ValueType vz, ValueType p, unsigned int n)
{
	ValueType total = 0;
	ValueType frequency = 1;
	ValueType amplitude = 1;
	for(unsigned int i=0; i<n; ++i)
	{
		ValueType fvx = vx * frequency;
		ValueType fvy = vy * frequency;
		ValueType fvz = vz * frequency;
		total = total + noisep(fvx, fvy, fvz) * amplitude;
		// is this faster than using pow function? probably..
		frequency = (frequency == 1)? 2 : frequency * 2;
		amplitude = (amplitude == 1)? p : amplitude * p;
	}

	return total;
}

template < class ValueType, class NoiseProvider >
ValueType perlin(const NoiseProvider& noisep, 
				 ValueType vx, ValueType vy, ValueType p, unsigned int n)
{
	ValueType total = 0;
	ValueType frequency = 1;
	ValueType amplitude = 1;
	for(unsigned int i=0; i<n; ++i)
	{
		ValueType fvx = vx * frequency;
		ValueType fvy = vy * frequency;
		total = total + noisep(fvx, fvy) * amplitude;
		// is this faster than using pow function? probably..
		frequency = (frequency == 1)? 2 : frequency * 2;
		amplitude = (amplitude == 1)? p : amplitude * p;
	}

	return total;
}

}