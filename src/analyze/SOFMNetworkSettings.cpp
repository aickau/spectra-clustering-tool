#include "SOFMNetworkSettings.h"


//Parameters(100,1,0.5,0.1,0.5,1)
//Parameters(100,26,1.0,0.001,0.5,1)
//Parameters(100,26,0.15,0.05,10,1)
//Parameters(100,26,1.0,0.001,0.5,2)
//Parameters(100,1,0.15,0.05,10,1)
//Parameters(100,26,0.25,0.01,1,0.5) << best!
SOFMParameters SOFMParameters::defaultParameters(100,26,0.25f,0.01f,1.f,0.5f); // default parameters


SOFMParameters::SOFMParameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd )
	:numSteps(_numSteps)
	,randomSeed(_randomSeed)
	,lRateBegin(_lRateBegin)
	,lRateEnd(_lRateEnd)
	,radiusBegin(_radiusBegin)
	,radiusEnd(_radiusEnd)
	,exportSubPage(false)
	,waitForUser(false)
	,fullExport(true)
	,imageBoderSize(6)
	,iconSize(64)
	,sstrSearchMode(SOFMNET_SETTINGS_SEARCHMODE_global)
	,normaliziationType(Spectra::SN_FLUX)
	,useBOSSWavelengthRange(false)
{
}

