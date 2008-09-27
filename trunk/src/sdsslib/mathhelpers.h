#include <math.h>

namespace MathHelpers
{
	// compare to float values
	bool floatEq (float a, float b, float epsilon); 

	 // t between 0.0 and 1.0
	float lerp (float a, float b, float t); 


	// logarithm with a certain base
	float logf(float value, float base);
	
	// returns a!
	// warning: no overfow detection
	unsigned int factorial( unsigned int a );

	// returns binomial coefficient n over k
	// k<=n
	unsigned int binomial( unsigned int n, unsigned int k );

	// generate normalized binomial factors 
	// factors = float array of size n+1
	// n = number of factors
	// e.g. for n=4 this would yield to 1/16, 4/16, 6/16, 4/16, 1/16
	void binomialFilter1d( float *factors, unsigned int n );

};

inline
bool MathHelpers::floatEq (float a, float b, float epsilon)
{
	float d = a-b;
	return (d<epsilon && d>-epsilon);
}

inline
float MathHelpers::lerp (float a, float b, float t)
{
	return (a+(b-a)*t);
}

inline
float MathHelpers::logf(float value, float base)
{
	return log(value)/log(base);
}