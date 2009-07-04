uniform sampler2D t0;
uniform sampler2D t1;

/*
	float error=0.0f;
	for (size_t i=0;i<Spectra::numSamples;i++)
	{
		float d = m_Amplitude[i]-_spectra.m_Amplitude[i];
		error += d*d;
	}
*/


void main (void)
{ 
	vec4 a0 = texture2D( t0,  gl_TexCoord[0].xy).xyzw;
	vec4 a1 = texture2D( t1,  gl_TexCoord[0].xy).xyzw;
	vec4 d = a0-a1;
	d *= d;
	gl_FragColor = d;	
}


