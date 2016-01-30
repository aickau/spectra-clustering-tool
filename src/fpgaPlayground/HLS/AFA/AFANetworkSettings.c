#include "include/AFANetworkSettings.h"


void afaSetDefaultParameters( AFAParameters *params )
{
	params->numSteps = 100;
	params->randomSeed = 26;
	params->lRateBegin = 0.25f;
	params->lRateEnd = 0.01f;
	params->radiusBegin = 1.0f;
	params->radiusEnd = 0.5f;
	params->searchMode = AFANET_SETTINGS_SEARCHMODE_global;
	params->normaliziationType = SN_FLUX;
	params->useBOSSWavelengthRange = false;
}
