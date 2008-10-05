#ifndef _NEURALGAS_H
#define _NEURALGAS_H

#include "sdsslib/spectra.h"
#include "sdsslib/Vecf.h"
#include "sdsslib/random.h"


#include <fstream>


class SpectraVFS;

// Clusters spectra using neural gas method.
// T. M. Martinetz and K. J. Schulten. A ``neural-gas network learns topologies. In T. Kohonen, K. Mäkisara, O. Simula, and J. Kangas, editors, 
// Artificial Neural Networks, pages 397-402. North-Holland, Amsterdam, 1991.
class NeuralGas
{
public:
	// NG parameters
	class Parameters
	{
	public:
		Parameters( unsigned int _numSteps, unsigned int _randomSeed, unsigned int numAdaptionElements, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd );
		unsigned int numSteps;											// number of learn steps. reasonable: 10..500
		unsigned int randomSeed;										// zero is not allowed
		unsigned int numAdaptionElements;								// top-k elements to form adaption vector from
		float lRateBegin;												// learn rate begin (0.0 .. 1.0) lRateEnd < lRateBegin
		float lRateEnd;													// learn rate end (0.0 .. 1.0)
		float radiusBegin;												// radius begin (0.0 .. ?)
		float radiusEnd;												// radius end (0.0 .. ? ) radiusEnd < radiusBegin

		static Parameters defaultParameters;
	};

	// _pSourceVFS is your input data which you want to cluster
	NeuralGas( SpectraVFS *_pSourceVFS );

	~NeuralGas();

	// resets the network
	void Reset( const Parameters &_params );

	// one learning step
	void Process();


protected:

	Rnd				m_Random;

	unsigned int	m_currentStep;

	Parameters		m_params;


	// training data
	SpectraVFS		*m_pSourceVFS;
	unsigned int	m_numSpectra;

	Vec2f			*m_pPosition;

	std::ofstream	m_logFile;
};


#endif