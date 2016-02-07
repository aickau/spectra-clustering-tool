#include "include/AFATypes.h"
#include "include/AFADefines.h"

#include "include/AFASpectraHW.h"
#include "include/AFANetworkSettingsHW.h"
#include "include/AFAProcessingHW.h"
#include "include/AFARandomHW.h"

#include <float.h>
#include <math.h>

AFAProcessingParam_t	AFAPP_HW;

void resetBM_HW( BestMatch *bmu)
{
	bmu->error = FLT_MAX;
	bmu->index = 0;
}

void adaptNetwork_HW( volatile AFASpectra *_srcSpectrum, int _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate )
{
	int x,y;
	float distY1, distY1Sqr, distYSqr;
	float distX1, distX1Sqr, distXSqr, distSqr;
	float hxy, lratehsx;
	unsigned int xpBestMatch = _bestMatchIndex % AFAPP_HW.m_gridSize;
	unsigned int ypBestMatch = _bestMatchIndex / AFAPP_HW.m_gridSize;
	float sigmaSqr2 = _sigmaSqr*(1.f/EULER);
	float fGridSizeSqr = (float)AFAPP_HW.m_gridSizeSqr;
	int gridSize = (int) AFAPP_HW.m_gridSize;
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
				spectraAdress = y*AFAPP_HW.m_gridSize+x;
				a = &AFAPP_HW.m_pNet[spectraAdress];
				AFASpectraAdapt_HW( a, _srcSpectrum, lratehsx );
			}
		}
	}
}

void compareSpectra_HW(volatile AFASpectra *_a, volatile AFASpectra *_pB, int _nCount, float *_pOutErrors )
{
	//assert( _pB != NULL );
	//assert( _pOutErrors != NULL );
	int i;
	int numElements = (int)_nCount;

	// hint this can run in parallel
	for (i=0;i<numElements;i++)
	{
		if ( AFASpectraIsEmpty_HW( &_pB[i] ) )
		{
			_pOutErrors[i] = AFASpectraCompare_HW( _a, &_pB[i] );
		}
		else
		{
			_pOutErrors[i] = FLT_MAX;
		}
	}
}

void searchBestMatchComplete_HW( volatile AFASpectra *_src, BestMatch *outbm )
{
	// see for the same outcome only slightly easier to understand..
	// searchBestMatchCompleteNonBatchMode()
	int j=0;
	int k;
	float err[AFA_COMPARE_BATCH_HW];
	BestMatch bestMatch;
	volatile AFASpectra *a;
	resetBM_HW(&bestMatch);


	while (j<AFAPP_HW.m_gridSizeSqr)
	{
		const int jInc = AFAMIN( AFA_COMPARE_BATCH_HW, (AFAMIN(AFAPP_HW.m_gridSizeSqr, j+AFA_COMPARE_BATCH_HW)-j));

		// calc euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
		a = &AFAPP_HW.m_pNet[j];
		compareSpectra_HW( _src, a, jInc, err );

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

int getNetworkIndex_HW( int _cellX, int _cellY )
{
	return _cellX+_cellY*AFAPP_HW.m_gridSize;
}

// set BMU in the map and source spectrum
// _networkSpectrum artificial spectrum in the map
// _networkIndex network index [0..gridsizesqr-1]
// _bestMatchSpectrum source/input spectrum
// _bestMatchIndex index to input spectrum [0..numspectra-1]
void setBestMatch_HW( volatile AFASpectra *_networkSpectrum, unsigned int _networkIndex, volatile AFASpectra *_bestMatchSpectrum, unsigned int _bestMatchIndex )
{
	//assert( _networkSpectrum.isEmpty() );
	// set best matching related info.
	_networkSpectrum->m_SpecObjID	= _bestMatchSpectrum->m_SpecObjID;
	_networkSpectrum->m_Index		= _bestMatchIndex;
	_networkSpectrum->m_version		= _bestMatchSpectrum->m_version;

	// remember best match position to NW for faster search
	_bestMatchSpectrum->m_Index = _networkIndex;

}

void searchBestMatchLocal_HW( volatile AFASpectra *_src, const int _searchRadius, BestMatch *outbm )
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
	resetBM_HW(&bestMatch);


	if ( _src->m_Index < 0 )
	{
		// no old position, due to continue computation, we have to use complete search!
		searchBestMatchComplete_HW( _src, outbm );
		return;
	}

	xpBestMatchOld = _src->m_Index % AFAPP_HW.m_gridSize;
	ypBestMatchOld = _src->m_Index / AFAPP_HW.m_gridSize;

	// calc boundaries
	xMin = AFAMAX( xpBestMatchOld-_searchRadius, 0 );
	yMin = AFAMAX( ypBestMatchOld-_searchRadius, 0 );
	xMax = AFAMIN( xpBestMatchOld+_searchRadius+1, AFAPP_HW.m_gridSize );
	yMax = AFAMIN( ypBestMatchOld+_searchRadius+1, AFAPP_HW.m_gridSize );

	numSpectraToSearch = ( (xMax-xMin)*(yMax-yMin) );


	// read spectra from vfs
	c=0;
	for ( y=yMin;y<yMax;y++ )
	{
		for ( x=xMin;x<xMax;x++ )
		{
			spectraIndex = getNetworkIndex_HW( x, y );
			AFAPP_HW.m_localSearchIndexVec[c] = spectraIndex;
			AFAPP_HW.m_localSearchSpectraVec[c] = &AFAPP_HW.m_pNet[spectraIndex];
			c++;
		}
	}

	// calculate errors/distances
	compareSpectra_HW( _src, *AFAPP_HW.m_localSearchSpectraVec, c, &AFAPP_HW.m_localSearchErrorVec[0] );

	// find bmu from error vector
	for ( i=0;i<numSpectraToSearch;i++ )
	{
		spectraIndex = AFAPP_HW.m_localSearchIndexVec[i];
		err = AFAPP_HW.m_localSearchErrorVec[i];

		if (err < bestMatch.error )
		{
			bestMatch.error = err;
			bestMatch.index = spectraIndex;
		}
	}


	*outbm = bestMatch;
}

bool_t AFAProcess_HW()
{
	BestMatch bmu;
	volatile AFASpectra *bmuSpectrum=NULL;
	volatile AFASpectra *currentSourceSpectrum;
	volatile AFASpectra *a;
	float lPercent = (float)(AFAPP_HW.m_currentStep)/(float)(AFAPP_HW.m_params.numSteps);
	float lRate = ( float ) ( AFAPP_HW.m_params.lRateBegin*pow(AFAPP_HW.m_params.lRateEnd/AFAPP_HW.m_params.lRateBegin,lPercent));
	float adaptionThreshold = AFAPP_HW.m_params.lRateEnd*0.01f;
	float sigma = ( float ) ( AFAPP_HW.m_params.radiusBegin*pow(AFAPP_HW.m_params.radiusEnd/AFAPP_HW.m_params.radiusBegin,lPercent));
	float sigmaSqr = sigma*sigma;
	double avgDist = 0.0;
	bool_t bFullSearch = TRUE;
	unsigned int searchRadius = 1;
	int i,j;
	int spectraIndex=0;
	int ind0, ind1, tmp;

	if ( AFAPP_HW.m_currentStep > AFAPP_HW.m_params.numSteps )
	{
		//Clustering finished (success).
		return TRUE;
	}


	// determine search strategy for BMUs for the current learning step
	if ( AFAPP_HW.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_localfast )
	{
		// always use a constant search radius, never do a global search
		bFullSearch = (AFAPP_HW.m_currentStep<1);
		searchRadius = 2;
	}
	else if ( AFAPP_HW.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_local )
	{
		// global search for the first 5 steps, decreasing search radius for increasing number of learning steps
		bFullSearch = (AFAPP_HW.m_currentStep<5);
		searchRadius = (unsigned int)(((1.f-lPercent)*0.5f*(float)(AFAPP_HW.m_gridSize)))+2;
	}
	else // SOFMNET_SETTINGS_SEARCHMODE_global
	{
		// always use global search, never go local.
		// slow but guarantees optimal results in every case
		bFullSearch = TRUE;
	}


	// select random spectra from spectra dataset

	// store all indicies in a list
	for ( i=0;i<AFAPP_HW.m_numSpectra;i++)
	{
		AFAPP_HW.m_pSpectraIndexList[i] = i;
	}

	// shake well
	for ( i=0;i<AFAPP_HW.m_numSpectra*2;i++)
	{
		ind0 = AFARandomIntRange_HW(AFAPP_HW.m_numSpectra-1);
		ind1 = AFARandomIntRange_HW(AFAPP_HW.m_numSpectra-1);

		// switch indices
		tmp = AFAPP_HW.m_pSpectraIndexList[ind0];
		AFAPP_HW.m_pSpectraIndexList[ind0] = AFAPP_HW.m_pSpectraIndexList[ind1];
		AFAPP_HW.m_pSpectraIndexList[ind1] = tmp;
	}

	// clear names
	for ( i=0;i<AFAPP_HW.m_gridSizeSqr;i++)
	{
		a = &AFAPP_HW.m_pNet[i];
		a->m_SpecObjID = 0;
		a->m_Index = -1;
	}


	// for each training spectra..
	for ( j=0;j<AFAPP_HW.m_numSpectra;j++ )
	{
		// initialize best match batch
		resetBM_HW(&bmu);

		spectraIndex = AFAPP_HW.m_pSpectraIndexList[j];
		currentSourceSpectrum = &AFAPP_HW.m_pSourceSpectra[spectraIndex];


		// retrieve best match neuron for a source spectra
		if (bFullSearch)
		{
			searchBestMatchComplete_HW( currentSourceSpectrum, &bmu );
		}
		else
		{
			searchBestMatchLocal_HW( currentSourceSpectrum, searchRadius, &bmu );
		}

		// mark best match neuron
		bmuSpectrum = &AFAPP_HW.m_pNet[bmu.index];
		setBestMatch_HW( bmuSpectrum, bmu.index, currentSourceSpectrum, spectraIndex );

		// adapt neighborhood
		// hint: this takes long.
		adaptNetwork_HW( currentSourceSpectrum, bmu.index, adaptionThreshold, sigmaSqr, lRate );
	}


	AFAPP_HW.m_currentStep++;

	// clustering not yet finished, need another learning step
	return FALSE;
}

