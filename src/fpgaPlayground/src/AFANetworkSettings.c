#include "AFANetworkSettings.h"
#include "AFADefines.h"

#define AFA_TEST_PHILIPP_TEST


#ifdef AFA_TEST_PHILIPP_TEST
void
AFASetDefaultParameters(
    AFAParameters *params )
{
    params->numSteps = 200;
    params->randomSeed = 15;
    params->lRateBegin = 0.25f;
    params->lRateEnd = 0.01f;
    params->radiusBegin = 1.0f;
    params->radiusEnd = 0.125f;
    params->searchMode = AFANET_SETTINGS_SEARCHMODE_global;
    params->normaliziationType = SN_FLUX;
    params->useBOSSWavelengthRange = TRUE;
}
#endif


#ifdef AFA_TEST_SINE_TEST
void
AFASetDefaultParameters(
    AFAParameters *params )
{
    params->numSteps = 100;
    params->randomSeed = 26;
    params->lRateBegin = 0.25f;
    params->lRateEnd = 0.01f;
    params->radiusBegin = 1.0f;
    params->radiusEnd = 0.5f;
    params->searchMode = AFANET_SETTINGS_SEARCHMODE_global;
    params->normaliziationType = SN_NONE;
    params->useBOSSWavelengthRange = FALSE;
}
#endif

