#include <float.h>
#include <math.h>
#include <stddef.h>
#include <memory.h>

#include "AFASpectra.h"
#include "AFAProcessing.h"
#include "AFADefines.h"
#include "AFATypes.h"

#include "malloc.h"


extern AFAProcessingParam_t	AFAPP_hw;
AFAProcessingParam_t	    AFAPP_sw;

// local variables to this file
// ============================

// maximum flux of input spectra
static float m_flux;

// maximum amplitude (before normalization)
static float m_Min;

// minimum amplitude (before normalization)
static float m_Max;

void
AFAProcessSetParamBlockParameters()
{
//	AFAProcessingParam_t	*AFAPP_hw;
	unsigned long			*mt; // the array for the state vector 
	int						*mti;
	int						pStart;
	int						pEnd;

//	AFAPP_hw = AFAProcessGetParamBlockAddress();
	AFARandomGetStateVectorBlockAddress( &mt, &mti );
	AFASpectraPixelStartEndGet( &pStart, &pEnd );

	// no misunderstandings here ...
	memset( &AFAPP_hw, 0, sizeof( AFAProcessingParam_t ));

	AFAPP_hw.m_currentStep = AFAPP_sw.m_currentStep;
	AFAPP_hw.m_gridSize = AFAPP_sw.m_gridSize;
	AFAPP_hw.m_gridSizeSqr = AFAPP_sw.m_gridSizeSqr;
	AFAPP_hw.m_localSearchErrorVec = AFAPP_sw.m_localSearchErrorVec;
	AFAPP_hw.m_localSearchIndexVec = AFAPP_sw.m_localSearchIndexVec;
	AFAPP_hw.m_localSearchSpectraVec = AFAPP_sw.m_localSearchSpectraVec;
	AFAPP_hw.g_spectraDataInput = AFAPP_sw.g_spectraDataInput;
	AFAPP_hw.m_pSpectraIndexList = AFAPP_sw.m_pSpectraIndexList;
	AFAPP_hw.m_numSpectra = AFAPP_sw.m_numSpectra;
	memcpy( &AFAPP_hw.m_params, &AFAPP_sw.m_params, sizeof( AFAParameters ));
	AFAPP_hw.spectraDataWorkingSet = AFAPP_sw.spectraDataWorkingSet;

	// the random generator state vector
	memcpy( &AFAPP_hw.m_mt[ 0 ], mt, sizeof( AFAPP_hw.m_mt ));
	AFAPP_hw.m_mti = *mti;

	// spectra start/stop
	AFAPP_hw.m_pStart = pStart;
	AFAPP_hw.m_pEnd = pEnd;
}

void resetBM( BestMatch *bmu)
{
	bmu->error = FLT_MAX;
	bmu->index = 0;
}

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
	AFAPP_sw.m_params = *params;
	AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

	AFAPP_sw.m_currentStep = 0;
}




void calcFluxAndNormalizeInputDS()
{
	volatile AFASpectra *a;
	int i;
	m_flux = 0.0f;

	for ( i=0;i<AFAPP_sw.m_numSpectra;i++ )
	{
		a = &AFAPP_sw.g_spectraDataInput[ i ];
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
	for ( i=0;i<AFAPP_sw.m_numSpectra;i++ )
	{
		a = &AFAPP_sw.g_spectraDataInput[ i ];

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
	calcMinMaxSp( AFAPP_sw.g_spectraDataInput, &m_Min, &m_Max );
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
		sp = &AFAPP_sw.spectraDataWorkingSet[ i ];

		x = i%_gridSize;
		y = i/_gridSize;
		strength = ((float)(x*x+y*y))*0.25f/strengthScale;
		AFASpectraRandomize( &spec, _minPeak*strength, _maxPeak*strength );

		AFASpectraSet( sp, &spec);
	}
}

uint32_t AFACalcGridSize( uint32_t numSpectra )
{
	uint32_t gridSize = (unsigned int)(ceilf(sqrtf((float)numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
	return gridSize;
}

uint64_t AFACalcAllocSpaceForHelperStructures( uint32_t numSpectra )
{
	uint32_t gridSize = AFACalcGridSize( numSpectra );
	uint32_t gridSizeSqr = gridSize*gridSize;
	uint64_t neededBytes = 0;

	// for m_pNet / SOM
	neededBytes += gridSizeSqr*sizeof(AFASpectra);

	// for m_pSpectraIndexList
	neededBytes += gridSizeSqr*sizeof(int);

	// for m_localSearchSpectraVec
	neededBytes += gridSizeSqr*sizeof(AFASpectra*);

	// for m_localSearchIndexVec
	neededBytes += gridSizeSqr*sizeof(int);

	// for m_localSearchErrorVec
	neededBytes += gridSizeSqr*sizeof(float);

	return neededBytes;
}


bool_t AFAInitProcessing(
	volatile void *spectraArrayInput,
	volatile void *helperStucturesMem,
	uint32_t numSpectra,
	bool_t continueProcessing,
	AFAParameters *params
	)
{
	int i;
	unsigned int xp, yp;
	unsigned int inset, spectraIndex;
	volatile AFASpectra *a, *b;
	uint64_t memOffset;


	if ( helperStucturesMem == NULL || spectraArrayInput == NULL )
		return FALSE;


	AFAPP_sw.m_gridSize = AFACalcGridSize(numSpectra);
	AFAPP_sw.m_gridSizeSqr = AFAPP_sw.m_gridSize*AFAPP_sw.m_gridSize;
	AFAPP_sw.m_currentStep = 0;
	AFAPP_sw.m_numSpectra = numSpectra;
	AFAPP_sw.g_spectraDataInput = ( volatile AFASpectra * ) spectraArrayInput;

	memOffset = 0;
	AFAPP_sw.spectraDataWorkingSet = ( volatile AFASpectra * )  &((unsigned char*)helperStucturesMem)[memOffset];
	memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(AFASpectra);

	AFAPP_sw.m_pSpectraIndexList = ( volatile int * ) &((unsigned char*)helperStucturesMem)[memOffset];
	memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(int);

	AFAPP_sw.m_localSearchSpectraVec = (volatile AFASpectra**) &((unsigned char*)helperStucturesMem)[memOffset];
	memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(AFASpectra*);

	AFAPP_sw.m_localSearchIndexVec = (int*)  &((unsigned char*)helperStucturesMem)[memOffset];
	memOffset += AFAPP_sw.m_gridSizeSqr*sizeof(int);

	AFAPP_sw.m_localSearchErrorVec = (float*)  &((unsigned char*)helperStucturesMem)[memOffset];


	reset(params);

	AFASpectraSetOperationRange( AFAPP_sw.m_params.useBOSSWavelengthRange );
	calcFluxAndNormalizeInputDS();
	calcMinMaxInputDS();

	if ( !continueProcessing )
	{
		//
		// start new computation
		//

		// generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
		initNetwork( AFAPP_sw.m_gridSize, m_Min, m_Max*0.01f );


		// fill network with random spectra, this improves the convergence times.
		// it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
		AFARandomInitRandom( AFAPP_sw.m_params.randomSeed );

		for ( i=0;i<AFAPP_sw.m_gridSizeSqr;i++ )
		{
			xp = i % AFAPP_sw.m_gridSize;
			yp = i / AFAPP_sw.m_gridSize;
			inset = 0;
			if ( xp >= inset && xp <= (AFAPP_sw.m_gridSize-inset) && yp >= inset && yp <= (AFAPP_sw.m_gridSize-inset) )
			{
				a = &AFAPP_sw.spectraDataWorkingSet[ i ];

				spectraIndex = AFARandomIntRange( AFAPP_sw.m_numSpectra - 1 );
				b = &AFAPP_sw.g_spectraDataInput[ spectraIndex ];
a->m_JSCIndex = spectraIndex;
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
	return TRUE;
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
	return _cellX+_cellY*AFAPP_sw.m_gridSize;
}



void adaptNetwork( volatile AFASpectra *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	int x,y;
	float distY1, distY1Sqr, distYSqr;
	float distX1, distX1Sqr, distXSqr, distSqr;
	float hxy, lratehsx;
	unsigned int xpBestMatch = _bestMatchIndex % AFAPP_sw.m_gridSize;
	unsigned int ypBestMatch = _bestMatchIndex / AFAPP_sw.m_gridSize;
	float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	float fGridSizeSqr = (float)AFAPP_sw.m_gridSizeSqr;
	int gridSize = (int) AFAPP_sw.m_gridSize;
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
			hxy = ( float )exp(-sqrtf(distSqr)/sigmaSqr2);						// spike
			lratehsx = _lRate*hxy;

			if ( lratehsx > _adaptionThreshold )
			{
				spectraAdress = y*AFAPP_sw.m_gridSize+x;
				a = &AFAPP_sw.spectraDataWorkingSet[spectraAdress];
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


	while (j<AFAPP_sw.m_gridSizeSqr)
	{
		const int jInc = AFAMIN( AFA_COMPARE_BATCH, (AFAMIN(AFAPP_sw.m_gridSizeSqr, j+AFA_COMPARE_BATCH)-j));

		// calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
		a = &AFAPP_sw.spectraDataWorkingSet[j];
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

	xpBestMatchOld = _src->m_Index % AFAPP_sw.m_gridSize;
	ypBestMatchOld = _src->m_Index / AFAPP_sw.m_gridSize;

	// calc boundaries
	xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
	yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
	xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, AFAPP_sw.m_gridSize );
	yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, AFAPP_sw.m_gridSize );

	numSpectraToSearch = ( (xMax-xMin)*(yMax-yMin) );


	// read spectra from vfs
	c=0;
	for ( y=yMin;y<yMax;y++ )
	{
		for ( x=xMin;x<xMax;x++ )
		{
			spectraIndex = getNetworkIndex( x, y );
			AFAPP_sw.m_localSearchIndexVec[c] = spectraIndex;
			AFAPP_sw.m_localSearchSpectraVec[c] = &AFAPP_sw.spectraDataWorkingSet[spectraIndex];
			c++;
		}
	}

	// calculate errors/distances
	compareSpectra( _src, *AFAPP_sw.m_localSearchSpectraVec, c, &AFAPP_sw.m_localSearchErrorVec[0] );

	// find bmu from error vector
	for ( i=0;i<numSpectraToSearch;i++ )
	{
		spectraIndex = AFAPP_sw.m_localSearchIndexVec[i];
		err = AFAPP_sw.m_localSearchErrorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
		}
	}


	*outbm = bestMatch;
}

int AFAGetSpectraIndex( int xp, int yp )
{
	int adr = xp+yp*AFAPP_sw.m_gridSize;

	if ( adr < 0 || adr >= AFAPP_sw.m_gridSizeSqr )
		return -1;

	return AFAPP_sw.spectraDataWorkingSet[adr].m_Index;
}
