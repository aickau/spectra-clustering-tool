#include "analyze/NeuralGas.h"

#include "sdsslib/random.h"
#include "sdsslib/defines.h"
#include "sdsslib/helpers.h"
#include "sdsslib/Timer.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/spectraHelpers.h"
#include "sdsslib/Vecf.h"

#include <map>


NeuralGas::Parameters NeuralGas::Parameters::defaultParameters(100,23,10,0.25,0.01,1.0,0.5); // default parameters


NeuralGas::Parameters::Parameters(  unsigned int _numSteps, unsigned int _randomSeed, unsigned int _numAdaptionElements, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd  )
:numSteps(_numSteps)
,randomSeed(_randomSeed)	
,numAdaptionElements(_numAdaptionElements)
,lRateBegin(_lRateBegin)
,lRateEnd(_lRateEnd)
,radiusBegin(_radiusBegin)
,radiusEnd(_radiusEnd)
{
}



NeuralGas::NeuralGas( SpectraVFS *_pSourceVFS )
:m_pSourceVFS(_pSourceVFS)
,m_numSpectra(_pSourceVFS->getNumSpectra())
,m_currentStep(0)
,m_params( Parameters::defaultParameters )
,m_logFile("ng_log.txt")
{
	m_pPosition = new Vec2f[m_numSpectra];

	Rnd r(m_params.randomSeed);

	for (unsigned int i=0;i<m_numSpectra;i++) 
	{
		m_pPosition[i].set( r.randomFloat(), r.randomFloat() );
	}
}

NeuralGas::~NeuralGas()
{
	delete[] m_pPosition;
}

void NeuralGas::reset( const Parameters &_params )
{
	m_currentStep = 0;
	m_params = _params;
}


void NeuralGas::process()
{
	if ( m_currentStep > m_params.numSteps )
	{
		Helpers::print( std::string("Clustering finished (success).\n"), &m_logFile );
		exit(1);
	}

	std::string sstrLog("Calculating step ");
	sstrLog += Helpers::numberToString( m_currentStep ) + " / " + Helpers::numberToString( m_params.numSteps ) + "\n";
	Helpers::print( sstrLog, &m_logFile );


	// select random spectra from spectra dataset
	std::vector<unsigned int> spectraIndexList;

	// store all indicies in a list
	for ( unsigned int i=0;i<m_numSpectra;i++) 
	{
		spectraIndexList.push_back(i);
	}

	// shake well
	for ( unsigned int i=0;i<m_numSpectra*2;i++)
	{
		unsigned int ind0 = m_Random.randomInt(m_numSpectra-1);
		unsigned int ind1 = m_Random.randomInt(m_numSpectra-1);

		unsigned int t = spectraIndexList[ind0];
		spectraIndexList[ind0] = spectraIndexList[ind1];
		spectraIndexList[ind1] = t;
	}
	
	float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
	float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	float lambda = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);


	// for each training spectra..
	for ( unsigned int j=0;j<m_numSpectra;j++ )
	{
		unsigned int spectraIndex = spectraIndexList.at(j);
		Spectra *a = m_pSourceVFS->beginRead( spectraIndex );

		// would be sufficient to do once for half the data
		std::map<float,unsigned int> errorMap;

		// get errors
		for ( unsigned int i=0;i<m_numSpectra;i++)
		{
			if ( i==spectraIndex )
			{
				// omit own
				continue;
			}

			// compare with spectrum and insert into error to index map
			Spectra *b = m_pSourceVFS->beginRead( i );
			float err = a->compare(*b);
			errorMap.insert( std::pair<float,unsigned int>(err,i) );
			m_pSourceVFS->endRead( i );
		}

		// compute adaption vector
		Vec2f pos = m_pPosition[j];
		Vec2f adaptionVec;
		adaptionVec.set(0.f, 0.f);


		std::map<float,unsigned int>::iterator it = errorMap.begin();
		std::map<float,unsigned int>::iterator itEnd = errorMap.end();

		unsigned int rank=1;
		while ( it != itEnd && rank <= m_params.numAdaptionElements  )
		{
			//Spectra *b = m_pSourceVFS->beginRead( it->second );
			//m_pSourceVFS->endRead( it->second );
			float nb = exp(-static_cast<float>(rank)/lambda);
			adaptionVec += (m_pPosition[it->second]-pos)*lRate*nb;

			rank++;
			it++;
		}
		m_pPosition[j] += adaptionVec;


		m_pSourceVFS->endRead( j );
	}



	m_currentStep++;
}




