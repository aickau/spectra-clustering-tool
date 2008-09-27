#include "sdsslib/mathhelpers.h"

#include "defines.h"
#include <assert.h>

unsigned int MathHelpers::factorial( unsigned int a )
{
	if ( a > 0 )
	{
		return MathHelpers::factorial(a-1)*a;
	}
	return 1;
}



unsigned int MathHelpers::binomial( unsigned int n, unsigned int k )
{
	assert( k <= n );

	if ( k > 0)
	{
		unsigned int a = factorial(n);
		unsigned int b = factorial(k)*factorial(n-k);
		return a/b;
	}

	return 1;
}


void MathHelpers::binomialFilter1d( float *factors, unsigned int n )
{
	assert( factors != NULL );
	float sum = 0.0f;
	for ( unsigned int i=0;i<=n;i++) 
	{
		factors[i] = static_cast<float>(binomial( n, i ));
		sum += factors[i];
	}
	for ( unsigned int i=0;i<=n;i++) 
	{
		factors[i] /= sum; 
	}
}
