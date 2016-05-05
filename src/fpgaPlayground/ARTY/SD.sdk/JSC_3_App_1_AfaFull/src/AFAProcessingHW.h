#ifndef AFA_PROCESSING_HW_H__
#define AFA_PROCESSING_HW_H__

#include "AFAProcessingCommon.h"

// use any large number you like
#define AFA_COMPARE_BATCH_HW 			( 256 )	//140000

// spectra cache size has direct impact on used HW resources on FPGA
#define AFA_SPECTRA_CACHE_NUMSPECTRA	( 16 ) /* COMPARE_BATCH_HW 140000 */

#endif
