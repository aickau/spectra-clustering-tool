#include <float.h>
#include <math.h>
#include <stddef.h>

#include "include/AFASpectra.h"
#include "include/AFAProcessing.h"
#include "include/AFADefines.h"
#include "include/AFATypes.h"
#include "include/AFARandom.h"

#include "malloc.h"


// use any large number you like
#define AFA_COMPARE_BATCH 140000

typedef struct BestMatch_
{
	unsigned int index;		//< index in the map
	float error;			//< euclidean distance
} BestMatch;

void resetBM( BestMatch *bmu)
{
	bmu->error = FLT_MAX;
	bmu->index = 0;
}


// code book spectra
volatile AFASpectra	*m_pNet;

// training data
volatile AFASpectra	*m_pSourceSpectra;

volatile AFASpectra **m_localSearchSpectraVec;
int *m_localSearchIndexVec;
float *m_localSearchErrorVec;

// number of source spectra
int m_numSpectra;


// grid size in cells of the map
int			    m_gridSize;

// squared grid size, number of neurons
int			    m_gridSizeSqr;

// current learning step
int			    m_currentStep;

AFAParameters	m_params;

// maximum amplitude (before normalization)
float			m_Min;

// minimum amplitude (before normalization)
float			m_Max;

// maximum flux of input spectra
float			m_flux;

// determine processing order. must be randomized every learning step
// contains m_gridSize * m_gridSize  elements
int				*m_pSpectraIndexList;


// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
void setBestMatch( volatile AFASpectra *_networkSpectrum, unsigned int _networkIndex, volatile AFASpectra *_bestMatchSpectrum, unsigned int _bestMatchIndex )
{
	//assert( _networkSpectrum.isEmpty() );
	// set best matching related info.
	_networkSpectrum->m_SpecObjID	= _bestMatchSpectrum->m_SpecObjID;
	_networkSpectrum->m_Index		= _bestMatchIndex;
	_networkSpectrum->m_version		= _bestMatchSpectrum->m_version;

	// remember best match position to NW for faster search
	_bestMatchSpectrum->m_Index = _networkIndex;

}


void reset( AFAParameters *params )
{
	// input from outside
	m_params = *params;
	AFARandomInitRandom( m_params.randomSeed );

	m_currentStep = 0;
}




void calcFluxAndNormalizeInputDS()
{
	volatile AFASpectra *a;
	int i;
	m_flux = 0.0f;

	for ( i=0;i<m_numSpectra;i++ )
	{
		a = &m_pSourceSpectra[ i ];
		AFASpectraNormalizeByFlux(a);
		m_flux = AFAMAX( a->m_flux, m_flux );
	}
}




void calcMinMaxSp(
	volatile AFASpectra *_vfs,
	float *_outMin,
	float *_outMax )
{
	volatile  AFASpectra *a;
	int i;
	*_outMin = FLT_MAX;
	*_outMax = -FLT_MAX;

	// calc min/max
	for ( i=0;i<m_numSpectra;i++ )
	{
		a = &m_pSourceSpectra[ i ];

		if ( *_outMin > a->m_Min ) 
		{
			*_outMin = a->m_Min;
		}
		if ( *_outMax < a->m_Max) 
		{
			*_outMax = a->m_Max;
		}
	}
	// for sdds: -560 .. ~20.000 Angström
}


void calcMinMaxInputDS()
{
	calcMinMaxSp( m_pSourceSpectra, &m_Min, &m_Max );
}



void initNetwork( unsigned int _gridSize, float _minPeak, float _maxPeak )
{
	unsigned int i, x, y;
	float strength;
	unsigned int gridSizeSqr = _gridSize*_gridSize;
	float strengthScale = (float)gridSizeSqr*2.f;
	AFASpectra spec;
	volatile AFASpectra *sp;
	
	AFASpectraClear( &spec );
	
	for ( i=0;i<gridSizeSqr;i++)
	{
		sp = &m_pNet[ i ];

		x = i%_gridSize;
		y = i/_gridSize;
		strength = ((float)(x*x+y*y))*0.25f/strengthScale;
		AFASpectraRandomize( &spec, _minPeak*strength, _maxPeak*strength );

		AFASpectraSet( sp, &spec);
	}
}


void AFAInitProcessing(
	volatile void *spectraArray,
	volatile void *spectraArray2,  // "sofmnet.bin"
	uint32_t numSpectra,
	bool_t continueProcessing,
	AFAParameters *params
	)
{
	int i;
	unsigned int xp, yp;
	unsigned int inset, spectraIndex;
	volatile AFASpectra *a, *b;

	m_currentStep = 0;
	m_numSpectra = numSpectra;
	m_pSourceSpectra =( volatile AFASpectra * ) spectraArray;
	m_pNet = ( volatile AFASpectra * ) spectraArray2;
	m_pSpectraIndexList = NULL;

	reset(params);

	AFASpectraSetOperationRange( m_params.useBOSSWavelengthRange );
	calcFluxAndNormalizeInputDS();
	calcMinMaxInputDS();

	if ( !continueProcessing )
	{
		//
		// start new computation 
		//
		m_gridSize = (unsigned int)(ceilf(sqrtf((float)m_numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
		m_gridSizeSqr = m_gridSize*m_gridSize;

		// generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
		initNetwork( m_gridSize, m_Min, m_Max*0.01f );


		// fill network with random spectra, this improves the convergence times.
		// it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
		AFARandomInitRandom( m_params.randomSeed );

		for ( i=0;i<m_gridSizeSqr;i++ )
		{
			xp = i % m_gridSize;
			yp = i / m_gridSize;
			inset = 0;
			if ( xp >= inset && xp <= (m_gridSize-inset) && yp >= inset && yp <= (m_gridSize-inset) )
			{
				a = &m_pNet[ i ];

				spectraIndex = AFARandomIntRange( m_numSpectra - 1 );
				b = &m_pSourceSpectra[ spectraIndex ];
				AFASpectraSet( a, b );
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





void AFARandomDeinitProcessing()
{
	free( m_pSpectraIndexList );
	free( m_localSearchSpectraVec );
	free( m_localSearchIndexVec );
	free( m_localSearchErrorVec );
}


/*
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
*/




int getNetworkIndex( int _cellX, int _cellY )
{
	return _cellX+_cellY*m_gridSize;
}



void adaptNetwork( volatile AFASpectra *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	int x,y;
	float distY1, distY1Sqr, distYSqr;
	float distX1, distX1Sqr, distXSqr, distSqr;
	float hxy, lratehsx;
	unsigned int xpBestMatch = _bestMatchIndex % m_gridSize;
	unsigned int ypBestMatch = _bestMatchIndex / m_gridSize;
	float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	float fGridSizeSqr = (float)m_gridSizeSqr;
	int gridSize = (int) m_gridSize;
	unsigned int spectraAdress;
	volatile AFASpectra *a;

	// hint: this should be parallelized
	// adjust weights of the whole network
	for ( y=0;y<gridSize;y++)
	{
		distY1 = (float)y-(float)ypBestMatch;
		distY1Sqr = distY1*distY1;
		distYSqr = distY1Sqr;

		for ( x=0;x<gridSize;x++)
		{
			distX1 = (float)x-(float)xpBestMatch;
			distX1Sqr = distX1*distX1;
			distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);
			distSqr = (distXSqr+distYSqr)/fGridSizeSqr;					// normalize squared distance with gridsize

			// calculate neighborhood function
			hxy = exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
			lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				spectraAdress = y*m_gridSize+x;
				a = &m_pNet[spectraAdress];
				AFASpectraAdapt( a, _srcSpectrum, lratehsx );
			}
		}
	}
}



void compareSpectra(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors )
{
	//assert( _pB != NULL );
	//assert( _pOutErrors != NULL );
	int i;
	int numElements = (int)_nCount;

	// hint this can run in parallel
	for (i=0;i<numElements;i++)
	{
		if ( AFASpectraIsEmpty( &_pB[i] ) )
		{
			_pOutErrors[i] = AFASpectraCompare( _a, &_pB[i] );
		}
		else
		{
			_pOutErrors[i] = FLT_MAX;
		}
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

void searchBestMatchComplete( volatile AFASpectra *_src, BestMatch *outbm )
{
	// see for the same outcome only slightly easier to understand..
	// searchBestMatchCompleteNonBatchMode()
	int j=0;
	int k;
	float err[AFA_COMPARE_BATCH];
	BestMatch bestMatch;
	volatile AFASpectra *a;
	resetBM(&bestMatch);


	while (j<m_gridSizeSqr)
	{
		const int jInc = AFAMIN( AFA_COMPARE_BATCH, (AFAMIN(m_gridSizeSqr, j+AFA_COMPARE_BATCH)-j));

		// calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
		a = &m_pNet[j];
		compareSpectra( _src, a, jInc, err );

		// find smallest error (i.e. winning neuron/ network spectrum) in batch
		for ( k=0;k<jInc;k++ )
		{
			if (err[k] < bestMatch.error )
			{
				bestMatch.error = err[k];
				bestMatch.index = j+k;
			}
		}

		j += jInc;
	}

	*outbm = bestMatch;
}


void searchBestMatchLocal( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm )
{
	//assert(_searchRadius > 0);
	int i,x,y;
	int xpBestMatchOld, ypBestMatchOld;
	int xMin, yMin, xMax, yMax;
	int numSpectraToSearch;
	unsigned int c;
	int spectraIndex;
	float err;

	BestMatch bestMatch;
	resetBM(&bestMatch);


	if ( _src->m_Index < 0 )
	{
		// no old position, due to continue computation, we have to use complete search!
		searchBestMatchComplete( _src, outbm );
		return;
	}

	xpBestMatchOld = _src->m_Index % m_gridSize;
	ypBestMatchOld = _src->m_Index / m_gridSize;

	// calc boundaries
	xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
	yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
	xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, m_gridSize );
	yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, m_gridSize );

	numSpectraToSearch = ( (xMax-xMin)*(yMax-yMin) );


	// read spectra from vfs
	c=0;
	for ( y=yMin;y<yMax;y++ )
	{
		for ( x=xMin;x<xMax;x++ )
		{
			spectraIndex = getNetworkIndex( x, y );
			m_localSearchIndexVec[c] = spectraIndex;
			m_localSearchSpectraVec[c] = &m_pNet[spectraIndex];
			c++;
		}
	}

	// calculate errors/distances
	compareSpectra( _src, *m_localSearchSpectraVec, c, &m_localSearchErrorVec[0] );

	// find bmu from error vector
	for ( i=0;i<numSpectraToSearch;i++ )
	{
		spectraIndex = m_localSearchIndexVec[i];
		err = m_localSearchErrorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
		}
	}


	*outbm = bestMatch;
}


bool_t AFARandomProcess()
{
	BestMatch bmu;
	volatile AFASpectra *bmuSpectrum=NULL;
	volatile AFASpectra *currentSourceSpectrum;
	volatile AFASpectra *a;
	float lPercent = (float)(m_currentStep)/(float)(m_params.numSteps);
	float lRate = m_params.lRateBegin*pow(m_params.lRateEnd/m_params.lRateBegin,lPercent);
	float adaptionThreshold = m_params.lRateEnd*0.01f;
	float sigma = m_params.radiusBegin*pow(m_params.radiusEnd/m_params.radiusBegin,lPercent);
	float sigmaSqr = sigma*sigma;
	double avgDist = 0.0;
	bool_t bFullSearch = TRUE;
	unsigned int searchRadius = 1;
	int i,j;
	int spectraIndex=0;
	int ind0, ind1, tmp;

	if ( m_currentStep > m_params.numSteps )
	{
		//Clustering finished (success).
		return TRUE;
	}


	// determine search strategy for BMUs for the current learning step
	if ( m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_localfast )
	{
		// always use a constant search radius, never do a global search
		bFullSearch = (m_currentStep<1);
		searchRadius = 2;
	}
	else if ( m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_local )
	{
		// global search for the first 5 steps, decreasing search radius for increasing number of learning steps
		bFullSearch = (m_currentStep<5);
		searchRadius = (unsigned int)(((1.f-lPercent)*0.5f*(float)(m_gridSize)))+2;
	}
	else // SOFMNET_SETTINGS_SEARCHMODE_global
	{
		// always use global search, never go local.
		// slow but guarantees optimal results in every case
		bFullSearch = TRUE;
	}


	// select random spectra from spectra dataset

	// store all indicies in a list
	for ( i=0;i<m_numSpectra;i++) 
	{
		m_pSpectraIndexList[i] = i;
	}

	// shake well
	for ( i=0;i<m_numSpectra*2;i++)
	{
		ind0 = AFARandomIntRange(m_numSpectra-1);
		ind1 = AFARandomIntRange(m_numSpectra-1);

		// switch indices
		tmp = m_pSpectraIndexList[ind0];
		m_pSpectraIndexList[ind0] = m_pSpectraIndexList[ind1];
		m_pSpectraIndexList[ind1] = tmp;
	}

	// clear names
	for ( i=0;i<m_gridSizeSqr;i++)
	{
		a = &m_pNet[i];
		a->m_SpecObjID = 0;
		a->m_Index = -1;
	}


	// for each training spectra..
	for ( j=0;j<m_numSpectra;j++ )
	{
		// initialize best match batch
		resetBM(&bmu);

		spectraIndex = m_pSpectraIndexList[j];
		currentSourceSpectrum = &m_pSourceSpectra[spectraIndex];


		// retrieve best match neuron for a source spectra
		if (bFullSearch) 
		{
			searchBestMatchComplete( currentSourceSpectrum, &bmu );
		}
		else
		{
			searchBestMatchLocal( currentSourceSpectrum, searchRadius, &bmu );
		}

		// mark best match neuron
		bmuSpectrum = &m_pNet[bmu.index];
		setBestMatch( bmuSpectrum, bmu.index, currentSourceSpectrum, spectraIndex );

		// adapt neighborhood
		// hint: this takes long.
		adaptNetwork( currentSourceSpectrum, bmu.index, adaptionThreshold, sigmaSqr, lRate );
	}


	m_currentStep++;

	// clustering not yet finished, need another learning step
	return FALSE;
}



