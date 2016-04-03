#include "AFANetworkSettings.h"
#include "AFADefines.h"


void
AFASetDefaultParameters(
    AFAParameters *params )
{
    params->numSteps = 100;
    params->randomSeed = 26;
    params->lRateBegin = 0.25f;
    params->lRateEnd = 0.01f;
    params->radiusBegin = 1.0f;
    params->radiusEnd = 0.125f;
    params->searchMode = AFANET_SETTINGS_SEARCHMODE_global;
    params->normaliziationType = SN_NONE;
    params->useBOSSWavelengthRange = FALSE;
}
