#include <float.h>
#include <math.h>
#include <stddef.h>

#include "include/AFASpectra.h"
#include "include/AFAProcessing.h"
#include "include/AFADefines.h"
#include "include/AFATypes.h"
#include "include/AFARandom.h"

#include "malloc.h"




// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
static void setBestMatch( volatile AFASpectra *_networkSpectrum, size_t _networkIndex, volatile AFASpectra *_bestMatchSpectrum, size_t _bestMatchIndex )
{
	//assert( _networkSpectrum.isEmpty() );
	// set best matching related info.
	_networkSpectrum->m_SpecObjID	= _bestMatchSpectrum->m_SpecObjID;
	_networkSpectrum->m_Index		= _bestMatchIndex;
	_networkSpectrum->m_version		= _bestMatchSpectrum->m_version;

	// remember best match position to NW for faster search
	_bestMatchSpectrum->m_Index = _networkIndex;

}




AFAProcessing::AFAProcessing(
    volatile void *srcSpectra,	    // array with all source spectra to cluster 
    volatile void *somNetwork,      // network, binary written to "sofmnet.bin" after each learning step
    uint32_t numSpectra,        // number of spectra stored in array
    bool bContinueComputation )
    : m_pSourceSpectra(( volatile AFASpectra * ) srcSpectra )
    , m_pNet(( volatile AFASpectra * ) somNetwork )
    , m_numSpectra( numSpectra )
    , m_params( AFAParameters::defaultParameters )
	, m_currentStep(0)
	, m_pSpectraIndexList(NULL)
{

    reset( m_params );

    AFASpectra::setOperationRange( m_params.useBOSSWavelengthRange );

    calcFluxAndNormalizeInputDS();
    calcMinMaxInputDS();


    if ( !bContinueComputation )
    {
        //
        // start new computation 
        //
        m_gridSize = static_cast<size_t>(ceilf(sqrtf((float)m_numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
        m_gridSizeSqr = m_gridSize*m_gridSize;

        // generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
        initNetwork( m_gridSize, m_Min, m_Max*0.01f );


        // fill network with random spectra, this improves the convergence times.
        // it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
        AFARnd r(m_params.randomSeed);
        for ( int i=0;i<m_gridSizeSqr;i++ )
        {
            size_t xp = i % m_gridSize;
            size_t yp = i / m_gridSize;
            const size_t inset = 0;
            if ( xp >= inset && xp <= (m_gridSize-inset) && yp >= inset && yp <= (m_gridSize-inset) )
            {
                volatile AFASpectra *a = &m_pNet[ i ];

                size_t spectraIndex = r.randomInt( m_numSpectra - 1 );
                volatile AFASpectra *b = &m_pSourceSpectra[ spectraIndex ];
                a->set( b );
            }
        }
    }
    else
    {
#if 0
        //
        // continue old computation 
        //

        m_pNet = new SpectraVFS( sstrSOFMFileName, false );

        m_gridSizeSqr = m_pNet->getNumSpectra();
        m_gridSize = sqrtf(m_gridSizeSqr);

        if ( m_gridSizeSqr == 0 )
        {
            // error
            assert(0);
            exit(0);
        }
#endif
    }
	m_pSpectraIndexList = (int*)malloc( m_gridSizeSqr*sizeof(int) );


	m_localSearchSpectraVec = (volatile AFASpectra**)malloc( m_gridSizeSqr*sizeof(AFASpectra*) );
	m_localSearchIndexVec = (int*)malloc( m_gridSizeSqr*sizeof(int) );
	m_localSearchErrorVec = (float*)malloc( m_gridSizeSqr*sizeof(float) );

}



AFAProcessing::~AFAProcessing()
{
	free( m_pSpectraIndexList );
	free( m_localSearchSpectraVec );
	free( m_localSearchIndexVec );
	free( m_localSearchErrorVec );
}




int AFAProcessing::getSrcSpectraIndex( int index ) const
{
	if ( m_pNet[index].isEmpty() ) 
		return -1;

	return m_pNet[index].m_Index;
}

volatile AFASpectra *AFAProcessing::getSrcSpectrum( int index ) const
{
	if ( m_pNet[index].isEmpty() ) 
		return NULL;

	const int srcIndex = m_pNet[index].m_Index;
	if ( srcIndex < 0 || srcIndex >= m_numSpectra ) {
		// should not happen
		return NULL;
	}
	return &m_pSourceSpectra[srcIndex];
}


bool AFAProcessing::process()
{
	if ( m_currentStep > m_params.numSteps )
	{
		//Clustering finished (success).
		return true;
	}

	const float lPercent = static_cast<float>(m_currentStep)/static_cast<float>(m_params.numSteps);
	const float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	const float adaptionThreshold = m_params.lRateEnd*0.01f;
	const float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
	const float sigmaSqr = sigma*sigma;
	bool bFullSearch = true;
	size_t searchRadius = 1;

	// determine search strategy for BMUs for the current learning step
	if ( m_params.searchMode == AFAParameters::AFANET_SETTINGS_SEARCHMODE_localfast )
	{
		// always use a constant search radius, never do a global search
		bFullSearch = (m_currentStep<1);
		searchRadius = 2;
	}
	else if ( m_params.searchMode == AFAParameters::AFANET_SETTINGS_SEARCHMODE_local )
	{
		// global search for the first 5 steps, decreasing search radius for increasing number of learning steps
		bFullSearch = (m_currentStep<5);
		searchRadius = static_cast<size_t>(((1.f-lPercent)*0.5f*static_cast<float>(m_gridSize)))+2;
	}
	else // SOFMNET_SETTINGS_SEARCHMODE_global
	{
		// always use global search, never go local.
		// slow but guarantees optimal results in every case
		bFullSearch = true;
	}


	// select random spectra from spectra dataset

	// store all indicies in a list
	for ( int i=0;i<m_numSpectra;i++) 
	{
		m_pSpectraIndexList[i] = i;
	}

	// shake well
	for ( int i=0;i<m_numSpectra*2;i++)
	{
		int ind0 = m_Random.randomInt(m_numSpectra-1);
		int ind1 = m_Random.randomInt(m_numSpectra-1);

		// switch indices
		int hui = m_pSpectraIndexList[ind0];
		m_pSpectraIndexList[ind0] = m_pSpectraIndexList[ind1];
		m_pSpectraIndexList[ind1] = hui;
	}

	// clear names
	for ( int i=0;i<m_gridSizeSqr;i++)
	{
		volatile AFASpectra *a = &m_pNet[i];
		a->m_SpecObjID = 0;
		a->m_Index = -1;
	}


	// for each training spectra..
	double avgDist = 0.0;
	for ( int j=0;j<m_numSpectra;j++ )
	{
		// initialize best match batch
		BestMatch bmu;

		const int spectraIndex = m_pSpectraIndexList[j];
		volatile AFASpectra *currentSourceSpectrum = &m_pSourceSpectra[spectraIndex];


		// retrieve best match neuron for a source spectra
		if (bFullSearch) 
		{
			bmu = searchBestMatchComplete( currentSourceSpectrum );
		}
		else
		{
			bmu = searchBestMatchLocal( currentSourceSpectrum, searchRadius );
		}

		// mark best match neuron
		volatile AFASpectra *bmuSpectrum = &m_pNet[bmu.index];
		setBestMatch( bmuSpectrum, bmu.index, currentSourceSpectrum, spectraIndex );

		// adapt neighborhood
		// hint: this takes long.
		adaptNetwork( currentSourceSpectrum, bmu.index, adaptionThreshold, sigmaSqr, lRate );
	}


	m_currentStep++;

	// clustering not yet finished, need another learning step
	return false;

}




void AFAProcessing::calcFluxAndNormalizeInputDS()
{
    m_flux = 0.0f;
    for ( int i=0;i<m_numSpectra;i++ )
    {
        volatile AFASpectra *a = &m_pSourceSpectra[ i ];
        a->normalizeByFlux();
        m_flux = AFAMAX( a->m_flux, m_flux );
    }
}

void AFAProcessing::reset( const AFAParameters &_params )
{
    // input from outside
    m_params = _params;
    m_Random.initRandom( m_params.randomSeed );

    m_currentStep = 0;
}

void AFAProcessing::calcMinMax(
    volatile AFASpectra *_vfs,
    float &_outMin,
    float &_outMax )
{
    _outMin = FLT_MAX;
    _outMax = -FLT_MAX;

    // calc min/max
    for ( int i=0;i<m_numSpectra;i++ )
    {
       volatile  AFASpectra *a = &m_pSourceSpectra[ i ];

        if ( _outMin > a->m_Min ) 
        {
            _outMin = a->m_Min;
        }
        if ( _outMax < a->m_Max) 
        {
            _outMax = a->m_Max;
        }
    }
    // for sdds: -560 .. ~20.000 Angström
}


void AFAProcessing::calcMinMaxInputDS()
{
    calcMinMax( m_pSourceSpectra, m_Min, m_Max );
}


int AFAProcessing::getNetworkIndex( int _cellX, int _cellY )
{
	return _cellX+_cellY*m_gridSize;
}



void AFAProcessing::initNetwork( size_t _gridSize, float _minPeak, float _maxPeak )
{
	size_t gridSizeSqr = _gridSize*_gridSize;
	float strengthScale = static_cast<float>(gridSizeSqr)*2.f;


	AFASpectra spec;
	spec.clear();
	for ( size_t i=0;i<gridSizeSqr;i++)
	{
		volatile AFASpectra *sp = &m_pNet[ i ];

		size_t x = i%_gridSize;
		size_t y = i/_gridSize;
		float strength = (static_cast<float>(x*x+y*y)*0.25f)/strengthScale;
		spec.randomize( _minPeak*strength, _maxPeak*strength );

		sp->set( &spec);
	}
}


/*

// Code just for illustration purposes how the search for the best match works in trivial way: 
BestMatch AFAProcessing::searchBestMatchCompleteNonBatchMode( const AFASpectra &_src )
{
	BestMatch bestMatch;
	bestMatch.reset();

	float err;

	for (int i=0;i<m_gridSizeSqr;i++)
	{
		err = FLT_MAX;
		if ( m_pNet[i].isEmpty() )
		{
			err = _src.compare( m_pNet[i] );
		}

		if (err < bestMatch.error )
		{
			bestMatch.error = err[k];
			bestMatch.index = i;
		}
	}
}
*/
// use any large number you like
#define AFA_COMPARE_BATCH 140000

BestMatch AFAProcessing::searchBestMatchComplete( volatile AFASpectra *_src )
{
	// see for the same outcome only slightly easier to understand..
	// searchBestMatchCompleteNonBatchMode()

	float err[AFA_COMPARE_BATCH];

	BestMatch bestMatch;
	bestMatch.reset();


	int j=0;
	while (j<m_gridSizeSqr)
	{
		const int jInc = AFAMIN( AFA_COMPARE_BATCH, (AFAMIN(m_gridSizeSqr, j+AFA_COMPARE_BATCH)-j));

		// calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
		volatile AFASpectra *a = &m_pNet[j];
		compareSpectra( _src, a, jInc, err );

		// find smallest error (i.e. winning neuron/ network spectrum) in batch
		for ( int k=0;k<jInc;k++ )
		{
			if (err[k] < bestMatch.error )
			{
				bestMatch.error = err[k];
				bestMatch.index = j+k;
			}
		}

		j += jInc;
	}

	return bestMatch;
}


BestMatch AFAProcessing::searchBestMatchLocal( volatile AFASpectra *_src, const int _searchRadius )
{
	//assert(_searchRadius > 0);
	BestMatch bestMatch;
	bestMatch.reset();


	if ( _src->m_Index < 0 )
	{
		// no old position, due to continue computation, we have to use complete search!
		bestMatch = searchBestMatchComplete( _src );
		return bestMatch;
	}

	const int xpBestMatchOld = _src->m_Index % m_gridSize;
	const int ypBestMatchOld = _src->m_Index / m_gridSize;

	// calc boundaries
	const int xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
	const int yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
	const int xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, m_gridSize );
	const int yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, m_gridSize );

	const int numSpectraToSearch( (xMax-xMin)*(yMax-yMin) );


	// read spectra from vfs
	size_t c=0;
	for ( int y=yMin;y<yMax;y++ )
	{
		for ( int x=xMin;x<xMax;x++ )
		{
			const int spectraIndex = getNetworkIndex( x, y );
			m_localSearchIndexVec[c] = spectraIndex;
			m_localSearchSpectraVec[c] = &m_pNet[spectraIndex];
			c++;
		}
	}

	// calculate errors/distances
	compareSpectra( _src, *m_localSearchSpectraVec, c, &m_localSearchErrorVec[0] );

	// find bmu from error vector
	for ( int i=0;i<numSpectraToSearch;i++ )
	{
		const int spectraIndex = m_localSearchIndexVec[i];
		const float err = m_localSearchErrorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
		}
	}



	return bestMatch;
}


void AFAProcessing::compareSpectra(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors )
{
	//assert( _pB != NULL );
	//assert( _pOutErrors != NULL );
	const int numElements = static_cast<int>(_nCount);

	// hint this can run in parallel
	for (int i=0;i<numElements;i++)
	{
		if ( _pB[i].isEmpty() )
		{
			_pOutErrors[i] = _a->compare( &_pB[i] );
		}
		else
		{
			_pOutErrors[i] = FLT_MAX;
		}
	}
}


void AFAProcessing::adaptNetwork( volatile AFASpectra *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	const size_t xpBestMatch = _bestMatchIndex % m_gridSize;
	const size_t ypBestMatch = _bestMatchIndex / m_gridSize;
	const float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	const float fGridSizeSqr = static_cast<float>(m_gridSizeSqr);

	const int gridSize = static_cast<int>(m_gridSize);

	// hint: this should be parallelized
	// adjust weights of the whole network
	for ( int y=0;y<gridSize;y++)
	{
		const float distY1 = static_cast<float>(y)-static_cast<float>(ypBestMatch);
		const float distY1Sqr = distY1*distY1;
		const float distYSqr = distY1Sqr;

		for ( int x=0;x<gridSize;x++)
		{
			const float distX1 = static_cast<float>(x)-static_cast<float>(xpBestMatch);
			const float distX1Sqr = distX1*distX1;
			const float distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);
			const float distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize

			// calculate neighborhood function
			const float hxy = exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
			const float lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				const size_t spectraAdress = y*m_gridSize+x;
				volatile AFASpectra *a = &m_pNet[spectraAdress];
				a->adapt( _srcSpectrum, lratehsx );
			}
		}
	}
}

