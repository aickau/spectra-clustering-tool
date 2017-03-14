#include "AFAConfig.h"
#include "AFATypesHW.h"
#include "AFADefines.h"

#include "AFANetworkSettingsHW.h"
#include "AFAProcessingHW.h"
#include "AFASpectraCommon.h"

#include <float.h>
#include <math.h>
#include <string.h>

#define AFA_STORE_STATUS( success, processing, idle )	                        \
	do													                        \
	{													                        \
		baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STATUS ] =	\
			  (( success    & 0x01 ) << 2 )				                     	\
			| (( processing & 0x01 ) << 1 )				                     	\
			| (( idle       & 0x01 ) << 0 );			                     	\
	} while ( 0 )

void
searchBestMatchComplete_HW(
    volatile uint32_t *baseAddr, 		// default starting address in memory
    uint64_t spectraDataInputHW_IndexToMem,
    uint64_t spectraDataWorkingSetHW_IndexToMem,
	AFAReadBackData_t *readBackData,
    BestMatch *outbm,
    uint32_t m_gridSizeSqr,
    uint32_t spectraIndex )
{
    // see for the same outcome only slightly easier to understand..
    // searchBestMatchCompleteNonBatchMode()
    uint32_t j = 0;
    uint32_t k;
    uint32_t i;
    float32_t err[ AFA_COMPARE_BATCH_HW ];
    BestMatch bestMatch;
    uint32_t bufferSource[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW ];
    uint32_t bufferWork[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW ];

    //resetBM_HW(&bestMatch);
    bestMatch.error = FLT_MAX;
    bestMatch.index = 0;

	readBackData->stats.memAccess_searchBestMatchComplete_HW += AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t );
	memcpy(( void * ) bufferSource, ( void * )&baseAddr[ spectraDataInputHW_IndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 ], AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t ));

    while ( j < m_gridSizeSqr )
    {
        const uint32_t jInc = AFAMIN( AFA_COMPARE_BATCH_HW, ( AFAMIN( m_gridSizeSqr, j + AFA_COMPARE_BATCH_HW ) - j ));

        // calculate euclidean distances for spectrum _src and a batch of network spectra starting at m_pNet[j] .. m_pNet[j+jInc-1]
        // hint this can run in parallel
        for ( i = 0; i < jInc; i++ )
        {
            float32_t error = 0.0f;

            uint32_t objIDLow  = baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ];
            uint32_t objIDHigh = baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ];

            if ( objIDLow == 0 && objIDHigh == 0 )
            {
				readBackData->stats.memAccess_searchBestMatchComplete_HW += AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t );
				memcpy(( void * ) bufferWork, ( void * )&baseAddr[ spectraDataWorkingSetHW_IndexToMem + ( j + i ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 ], AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t ));

                for ( k = AFA_SPECTRA_INDEX_AMPLITUDE; k < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; k++ )
                {
#pragma HLS PIPELINE
                    uint32_t tmpVal1 = bufferSource[ k ];
                    uint32_t tmpVal2 = bufferWork[ k ];
                    float32_t tmpVal1Float = *(( float32_t * ) &tmpVal1 );
                    float32_t tmpVal2Float = *(( float32_t * ) &tmpVal2 );
                    float32_t d = ( tmpVal2Float - tmpVal1Float );
                    error += d * d;
                }
                err[ i ] = error;
            }
            else
            {
                err[ i ] = FLT_MAX;
            }
        }

        // find smallest error (i.e. winning neuron/ network spectrum) in batch
        for ( k = 0; k < jInc; k++ )
        {
            if ( err[ k ] < bestMatch.error )
            {
                bestMatch.error = err[ k ];
                bestMatch.index = j + k;
            }
        }

        j += jInc;
    }

    *outbm = bestMatch;
}

void
adaptNetwork_HW(
    volatile uint32_t *baseAddr, 		// default starting address in memory
    uint64_t currentSourceSpectrumIndex,
    uint64_t spectraDataWorkingSetIndex,
	AFAReadBackData_t *readBackData,
    sint32_t bestMatchIndex,
    float32_t adaptionThreshold,
    float32_t sigmaSqr,
    float32_t lRate,
    uint32_t gridSize,
    uint32_t gridSizeSqr )
{
    const uint32_t xpBestMatch = bestMatchIndex % gridSize;
    const uint32_t ypBestMatch = bestMatchIndex / gridSize;
    const float32_t sigmaSqr2 = sigmaSqr * ( 1.f / AFA_CONST_EULER );
    const float32_t fGridSizeSqr = ( float32_t )gridSizeSqr;
    uint32_t x,y,i;
    uint32_t bufferSource[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW ];
    uint32_t bufferWork[ AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW ];

	readBackData->stats.memAccess_adaptNetwork_HW_read += AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t );
	memcpy(( void * ) bufferSource, ( void * )&baseAddr[ currentSourceSpectrumIndex ], AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t ));
	
    // hint: this should be parallelized
    // adjust weights of the whole network
    for ( y = 0; y < gridSize; y++ )
    {
        const float32_t distY1 = ( float32_t )y - ( float32_t )ypBestMatch;
        const float32_t distY1Sqr = distY1 * distY1;
        const float32_t distYSqr = distY1Sqr;

        for ( x = 0; x < gridSize; x++ )
        {
        	const float32_t distX1 = ( float32_t )x - ( float32_t )xpBestMatch;
        	const float32_t distX1Sqr = distX1 * distX1;
        	const float32_t distXSqr = distX1Sqr;//MIN(distX1Sqr, distX2Sqr);

            // JSC 2016-03-27: may not totally correct as fGridSizeSqr is the square of side length
            // whereas ( distXSqr + distYSqr ) may be twice squared side length, ==> [0..2] not [0..1]
        	const float32_t distSqr = ( distXSqr + distYSqr ) / fGridSizeSqr;                 // normalize squared distance with gridsize

            // calculate neighborhood function
        	const float32_t hxy = expf( -sqrtf( distSqr ) / sigmaSqr2 );              // spike
        	const float32_t lratehsx = lRate * hxy;

            if ( lratehsx > adaptionThreshold )
            {
                const uint64_t spectraAdress = ( y * gridSize + x ) * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
#if 1   // optimization trials
// #if 0   // 2nd opt
                // uint32_t xxx = AFA_SPECTRA_INDEX_AMPLITUDE;
                // uint32_t yyy = AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW;
                // an_inner_loop: for ( i = AFA_SPECTRA_INDEX_AMPLITUDE; i < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; i++ )

                // {
// #pragma HLS PIPELINE
                    // const uint32_t  aTmp = baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ];
                    // const uint32_t cTmp = baseAddr[ currentSourceSpectrumIndex + i ];
                    // const float32_t aTmpFloat = *(( float32_t * ) &aTmp );
                    // const float32_t cTmpFloat = *(( float32_t * ) &cTmp );
                    // const float32_t bTmpFloat = aTmpFloat + lratehsx * ( cTmpFloat - aTmpFloat );
                    // const uint32_t bTmp = *(( uint32_t * ) &bTmpFloat );

                    // baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ] = bTmp;
                // }
// #else   // 1st opt
                // memcpy(( void * ) bufferA, ( void * )&baseAddr[ spectraDataWorkingSetIndex + spectraAdress ], AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( uint32_t ));
                // memcpy(( void * ) bufferB, ( void * )&baseAddr[ currentSourceSpectrumIndex ],                 AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( uint32_t ));
                // for ( i = AFA_SPECTRA_INDEX_AMPLITUDE; i < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; i++ )
                // {
// #pragma HLS UNROLL
// //#pragma HLS PIPELINE II=2
                    // const uint32_t aTmp = bufferA[ i ];
                    // const uint32_t cTmp = bufferB[ i ];
                    // const float32_t aTmpFloat = *(( float32_t * ) &aTmp );
                    // const float32_t cTmpFloat = *(( float32_t * ) &cTmp );
                    // const float32_t bTmpFloat = aTmpFloat + lratehsx * ( cTmpFloat - aTmpFloat );
                    // const uint32_t bTmp = *(( uint32_t * ) &bTmpFloat );

                    // bufferA[ i ] = bTmp;
                // }
                // memcpy(( void * )&baseAddr[ spectraDataWorkingSetIndex + spectraAdress ], ( void * ) bufferA, AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( uint32_t ));
// #endif  // 1st opt

				readBackData->stats.memAccess_adaptNetwork_HW_read += AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t );
				memcpy(( void * ) bufferWork, ( void * )&baseAddr[ spectraDataWorkingSetIndex + spectraAdress ], AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t ));

                for ( i = AFA_SPECTRA_INDEX_AMPLITUDE; i < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; i++ )
                {
#pragma HLS PIPELINE
                    const uint32_t aTmp = bufferWork[ i ];
                    const uint32_t cTmp = bufferSource[ i ];
                    const float32_t aTmpFloat = *(( float32_t * ) &aTmp );
                    const float32_t cTmpFloat = *(( float32_t * ) &cTmp );
                    const float32_t bTmpFloat = aTmpFloat + lratehsx * ( cTmpFloat - aTmpFloat );
                    const uint32_t bTmp = *(( uint32_t * ) &bTmpFloat );

                    bufferWork[ i ] = bTmp;
                }
				
				readBackData->stats.memAccess_adaptNetwork_HW_write += AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t );
				memcpy(( void * )&baseAddr[ spectraDataWorkingSetIndex + spectraAdress ], ( void * ) bufferWork, AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW * sizeof( float32_t ));
				
#else
// unoptimized
                for ( i = AFA_SPECTRA_INDEX_AMPLITUDE; i < AFA_SPECTRA_INDEX_AMPLITUDE + AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; i++ )
                {
                    const uint32_t aTmp = baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ];
                    const uint32_t cTmp = baseAddr[ currentSourceSpectrumIndex + i ];
                    const float32_t aTmpFloat = *(( float32_t * ) &aTmp );
                    const float32_t cTmpFloat = *(( float32_t * ) &cTmp );
                    const float32_t bTmpFloat = aTmpFloat + lratehsx * ( cTmpFloat - aTmpFloat );
                    const uint32_t bTmp = *(( uint32_t * ) &bTmpFloat );

                    baseAddr[ spectraDataWorkingSetIndex + spectraAdress + i ] = bTmp;
                }
#endif
            }
        }
    }
}

void
AFAProcess_HW(
    volatile uint32_t *baseAddr, 		// default starting address in memory
	volatile uint1_t *interrupt
    )
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE ap_none      Port=interrupt
#pragma HLS INTERFACE m_axi        port=baseAddr              depth=32

    // define the offsets to access external memory through arrays
    uint64_t spectraDataInputHWIndexToMem;
    uint64_t spectraDataWorkingSetHWIndexToMem;
    uint64_t spectraIndexListIndexToMem;
    uint64_t readBackDataIndexToMem;

	uint64_t bmuSpectrumIndex;
    uint32_t idx;
	uint32_t ii;
    sint32_t spectraIndex = 0;

#ifdef LOCAL_SEARCH_ENABLED
    const sint32_t AFAConstantM1 = -1;
    const float32_t AFAConstantM1F = -1.0f;
    const uint32_t AFAConstant0  = 0;
#endif

    BestMatch bmu;
	AFAReadBackData_t readBackData;

	// block ram
    volatile uint32_t param[ AFA_PARAM_BLOCK_WORK_SIZE_IN_WORDS32 ];

	static bool_t statusSuccess = 0;
	static bool_t statusIdle = 0;
	static bool_t statusProcessing = 0;

#if 0
	uint32_t i = 0;
	for (;;)
	{
		uint32_t ii = ( i << 16 ) / 4;
		baseAddr[ ii ] = ( 0x1234 << 16 ) | i;
		i += 0x0010;
		i &= 0x0000ffff;
	}
#endif
#if 0
	// this clears at module start and will not disturb processing
	uint32_t clearStart = AFA_PARAM_BLOCK_ADDRESS_INDEX;
	uint32_t clearSize = ( 256 * 1024 * 1024 - ( AFA_PARAM_BLOCK_ADDRESS_INDEX * sizeof( uint32_t ) - 0x80000000 )) >> 2;
	while ( clearSize-- )
	{
		baseAddr[ clearStart++ ] = 0x7642e5e1;
	}
#endif

	// at module start: stop engine after start
	*interrupt = 0;
	baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STARTSTOP ] = 0xfefefefe;
	baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STATUS ]    = 0xffeedd00;
	param[ 0 ] = 0xff0042ee;	// magic number

	// set the status' at module start
	statusSuccess = 0;
	statusProcessing = 0;
	statusIdle = 1;

#ifdef __SYNTHESIS__
    // no endless loop in PC-Simulation
    for (;;)
#endif // #ifdef __SYNTHESIS__
    {
		// ==================================================================
		// The following section is provided to allow a memory only control
		// of the accelerator:
		// At a defined address "PARAM_BLOCK_ADDRESS_INDEX / 4" there is
		// a memory block located whose first entry is the on/off switch.
		// if this memory location contains 0xd00fd00f
		// the block starts running.
		//
		// The status ports show the actual state of the block.
		// Normally idle, but if started, idle is off and processing on, until
		// processing ends. Then processing is off and idle is back to on.
		// Success is set on operation result.
		//
		// Simple and hopefully useful.
		// ==================================================================
		
		AFA_STORE_STATUS( statusSuccess, statusProcessing, statusIdle ); // idle

#ifdef __SYNTHESIS__
		// enable the memory fence in the end
		baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW ] = 0xffffffff;
		
		do
		{
			if ( 0xd00fd00f == baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STARTSTOP ])
			{
				baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_STARTSTOP ] = 0xeeffeeff;	// prevent restart
				statusProcessing = 1;
			}
			AFA_STORE_STATUS( statusSuccess, statusProcessing, statusIdle ); // processing
			baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + 64 ]++;
		} while ( 0 == statusProcessing );
#else
        statusProcessing = 1;
#endif // #ifdef __SYNTHESIS__

        statusIdle = 0;
        statusSuccess = 0;	// we find eventually success at the end ...

        AFA_STORE_STATUS(statusSuccess, statusProcessing, statusIdle); // non idle
		
		// ==================================================================
		// End of start control block
		// ==================================================================

		// get data from param block
		// =========================
		memcpy(( void * ) &param[ AFA_PARAM_INDICES_COPY_START ], ( const void * )&baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_INDICES_COPY_START ], AFA_PARAM_BLOCK_WORK_SIZE_IN_BYTES - AFA_PARAM_INDICES_COPY_START * sizeof( uint32_t ));

		bool_t bFullSearch = param[ AFA_PARAM_INDICES_FULL_SEARCH ];
		uint32_t adaptionThreshold_temp = param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD ];
		float32_t adaptionThreshold = *(( float32_t * )&adaptionThreshold_temp );
		uint32_t sigmaSqr_temp = param[ AFA_PARAM_INDICES_SIGMA_SQR ];
		float32_t sigmaSqr = *(( float32_t * )&sigmaSqr_temp );
		uint32_t lRate_temp = param[ AFA_PARAM_INDICES_LRATE ];
		float32_t lRate = *(( float32_t * )&lRate_temp );
		uint32_t m_gridSize = param[ AFA_PARAM_INDICES_GRID_SIZE ];
		uint32_t m_gridSizeSqr = param[ AFA_PARAM_INDICES_GRID_SIZE_SQR ];
		uint32_t m_numSpectra = param[ AFA_PARAM_INDICES_NUM_SPECTRA ];
#ifdef LOCAL_SEARCH_ENABLED
		const uint32_t spectraCacheSize = AFAMIN( AFA_SPECTRA_CACHE_NUMSPECTRA, ( AFAMIN( m_gridSizeSqr, AFA_COMPARE_BATCH_HW )));
		const uint32_t searchRadius = param[ AFA_PARAM_INDICES_SEARCH_RADIUS ];
#endif

		// get offsets to different memory locations as 2 32-bit words combining them to 64bit offsets
		spectraDataInputHWIndexToMem      = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_LOW   ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_HIGH   ] << 32 ));
		spectraDataWorkingSetHWIndexToMem = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_LOW      ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_HIGH      ] << 32 ));
		spectraIndexListIndexToMem        = ( param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_LOW ] | (( uint64_t ) param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_HIGH ] << 32 ));
		readBackDataIndexToMem            = ( param[ AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_LOW          ] | (( uint64_t ) param[ AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_HIGH          ] << 32 ));
		
		// these offsets are used as array indices into an uint32_t array. So divide them up by the byte width
		spectraDataInputHWIndexToMem      /= sizeof( uint32_t );
		spectraDataWorkingSetHWIndexToMem /= sizeof( uint32_t );
		spectraIndexListIndexToMem        /= sizeof( uint32_t );
		readBackDataIndexToMem            /= sizeof( uint32_t );

		// readBackData receive - fill structure
		readBackData.stats.memAccess_AFAProcess_HW              = baseAddr[ readBackDataIndexToMem + 0 ] | (( uint64_t ) baseAddr[ readBackDataIndexToMem + 1 ] << 32 );
		readBackData.stats.memAccess_adaptNetwork_HW_read       = baseAddr[ readBackDataIndexToMem + 2 ] | (( uint64_t ) baseAddr[ readBackDataIndexToMem + 3 ] << 32 );
		readBackData.stats.memAccess_adaptNetwork_HW_write      = baseAddr[ readBackDataIndexToMem + 4 ] | (( uint64_t ) baseAddr[ readBackDataIndexToMem + 5 ] << 32 );
		readBackData.stats.memAccess_searchBestMatchComplete_HW = baseAddr[ readBackDataIndexToMem + 6 ] | (( uint64_t ) baseAddr[ readBackDataIndexToMem + 7 ] << 32 );
		
		// for each training spectra..
		for ( idx = 0; idx < m_numSpectra; idx++ )
		{
			// initialize best match batch
			bmu.error = FLT_MAX;
			bmu.index = 0;

			spectraIndex = baseAddr[ spectraIndexListIndexToMem + idx ];

			// retrieve best match neuron for a source spectra
			if ( bFullSearch )
			{
				searchBestMatchComplete_HW(
					baseAddr, 		// default starting address in memory
					spectraDataInputHWIndexToMem,
					spectraDataWorkingSetHWIndexToMem,
					&readBackData,
					&bmu,
					m_gridSizeSqr,
					spectraIndex );
			}
			else
			{
#ifdef LOCAL_SEARCH_ENABLED
				searchBestMatchLocal_HW( currentSourceSpectrum, searchRadius, &bmu );
#endif
			}

			// mark best match neuron

			// set BMU in the map and source spectrum
			// _networkSpectrum artificial spectrum in the map
			// _networkIndex network index [0..gridsizesqr-1]
			// _bestMatchSpectrum source/input spectrum
			// _bestMatchIndex index to input spectrum [0..numspectra-1]
			// set best matching related info.

			bmuSpectrumIndex = spectraDataWorkingSetHWIndexToMem + bmu.index * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
			baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ] = baseAddr[ spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW ];
			baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = baseAddr[ spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ];
			baseAddr[ bmuSpectrumIndex + AFA_SPECTRA_INDEX_INDEX ] = spectraIndex;

			// remember best match position to NW for faster search
			baseAddr[ spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 + AFA_SPECTRA_INDEX_INDEX ] = bmu.index;

			readBackData.stats.memAccess_AFAProcess_HW += ( 2 /* read */ + 4 /* write */ ) * sizeof( uint32_t );

			// adapt neighborhood
			// hint: this takes long.
			adaptNetwork_HW(
				baseAddr, 		// default starting address in memory
				spectraDataInputHWIndexToMem + spectraIndex * AFA_SPECTRA_INDEX_SIZE_IN_UINT32, // this is the "currentSourceSpectrumIndex"
				spectraDataWorkingSetHWIndexToMem, // this is the spectraDataWorkingSetHWIndexToMem
				&readBackData,
				bmu.index,
				adaptionThreshold,
				sigmaSqr,
				lRate,
				m_gridSize,
				m_gridSizeSqr );
		}

		// readBackData receive - fill structure
		baseAddr[ readBackDataIndexToMem + 0 ] = ( readBackData.stats.memAccess_AFAProcess_HW              >>  0 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 1 ] = ( readBackData.stats.memAccess_AFAProcess_HW              >> 32 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 2 ] = ( readBackData.stats.memAccess_adaptNetwork_HW_read       >>  0 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 3 ] = ( readBackData.stats.memAccess_adaptNetwork_HW_read       >> 32 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 4 ] = ( readBackData.stats.memAccess_adaptNetwork_HW_write      >>  0 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 5 ] = ( readBackData.stats.memAccess_adaptNetwork_HW_write      >> 32 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 6 ] = ( readBackData.stats.memAccess_searchBestMatchComplete_HW >>  0 ) & 0x00000000ffffffff;
		baseAddr[ readBackDataIndexToMem + 7 ] = ( readBackData.stats.memAccess_searchBestMatchComplete_HW >> 32 ) & 0x00000000ffffffff;

		// process return value
		param[ AFA_PARAM_INDICES_RETURN_VALUE ] = 0x12345678;
		
		// copy back data from param block
		// ===============================
#if 0
		for ( ii = 0; ii < AFA_PARAM_BLOCK_WORK_SIZE_IN_WORDS32; ii++ )
		{
#pragma HLS PIPELINE II=1
			baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW + ii ] = param[ ii ];
		}
#else
		memcpy(( void * ) &baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW ], ( const void * )&param[ 0 ], AFA_PARAM_BLOCK_WORK_SIZE_IN_BYTES );
#endif

#ifdef __SYNTHESIS__
		// memory fence - start =============================================================================================================

		baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW + AFA_PARAM_INDICES_STARTSTOP ] = 0xfefefefe;
		do
		{
			if ( 0xd00fd00f == baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW + AFA_PARAM_INDICES_STARTSTOP ])
			{
				baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW + AFA_PARAM_INDICES_STARTSTOP ] = 0xeeffeeff;	// prevent restart
				statusProcessing = 0;
			}
			AFA_STORE_STATUS( statusSuccess, statusProcessing, statusIdle ); // processing
			baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW + AFA_PARAM_INDICES_STATUS ]++;
		} while ( 1 == statusProcessing );

		// memory fence - end ===============================================================================================================
#endif // #ifdef __SYNTHESIS__

		// set the status
		statusProcessing = 0;
		statusIdle       = 1;
		statusSuccess    = 1;    // clustering not yet finished, need another learning step
		AFA_STORE_STATUS( statusSuccess, statusProcessing, statusIdle ); // finish, idle again

		*interrupt = 1;
		*interrupt = 0;
	}	// loop forever
	//	return ( statusSuccess << 2 ) | ( statusProcessing << 1 ) | ( statusIdle << 0 );
}
