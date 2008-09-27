#ifndef _NEURALGAS_H
#define _NEURALGAS_H

#include "sdsslib/spectra.h"
#include "sdsslib/Vecf.h"
#include "sdsslib/random.h"


#include <fstream>


class SpectraVFS;

// Clusters spectra using neural gas method.
class NeuralGas
{
public:
	// NG parameters
	class Parameters
	{
	public:
		Parameters( unsigned int _numSteps, unsigned int _randomSeed );
		unsigned int numSteps;											// number of learn steps. reasonable: 10..500
		unsigned int randomSeed;										// zero is not allowed

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