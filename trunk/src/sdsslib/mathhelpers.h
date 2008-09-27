
namespace MathHelpers
{

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