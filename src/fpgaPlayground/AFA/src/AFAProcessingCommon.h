#ifndef AFA_PROCESSING_COMMON_H__
#define AFA_PROCESSING_COMMON_H__

#include "AFATypes.h"
#include "AFAConfig.h"
#include "AFASpectraCommon.h"

enum
{
	AFA_PARAM_BLOCK_SIZE_IN_BYTES       	= 256,							// size in bytes of the parameter block
	AFA_PARAM_BLOCK_WORK_SIZE_IN_BYTES     	= AFA_PARAM_BLOCK_SIZE_IN_BYTES / 2,	// size in bytes of working area within the parameter block
	AFA_PARAM_BLOCK_WORK_SIZE_IN_WORDS32	= AFA_PARAM_BLOCK_WORK_SIZE_IN_BYTES / 4,
	AFA_PARAM_BLOCK_ADDRESS_INDEX       	= PARAM_BLOCK_ADDRESS_INDEX,	// index (address) of the parameter block
	AFA_PARAM_BLOCK_ADDRESS_INDEX_SHADOW 	= PARAM_BLOCK_ADDRESS_INDEX + AFA_PARAM_BLOCK_WORK_SIZE_IN_WORDS32,	// index (address) of the parameter block
    AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS    	= 1 << 8,       // power of 2 needed: 256

    AFA_WORKING_DATA_NAME_LENGTH        	= 32,

    AFA_PARAM_INDICES_STARTSTOP         	= 0,
    AFA_PARAM_INDICES_STATUS,
	AFA_PARAM_INDICES_RESERVED,
    AFA_PARAM_INDICES_SEARCH_RADIUS,
    AFA_PARAM_INDICES_FULL_SEARCH,
    AFA_PARAM_INDICES_ADAPTION_THRESHOLD,
    AFA_PARAM_INDICES_SIGMA_SQR,
    AFA_PARAM_INDICES_LRATE,
    AFA_PARAM_INDICES_GRID_SIZE,
    AFA_PARAM_INDICES_GRID_SIZE_SQR,
    AFA_PARAM_INDICES_NUM_SPECTRA,
    AFA_PARAM_INDICES_RNG_MTI,
    AFA_PARAM_INDICES_PIXEL_START,
    AFA_PARAM_INDICES_PIXEL_END,
    AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_LOW,
    AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_HIGH,
    AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_LOW,
    AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_HIGH,
    AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_LOW,
    AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_HIGH,
    AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_LOW,
    AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_HIGH,
    AFA_PARAM_INDICES_LED1_OUTPUT,
    AFA_PARAM_INDICES_DBG_COUNTER,
};

typedef struct
{
    char        name[ AFA_WORKING_DATA_NAME_LENGTH ];   // name of data
    uint64_t    size;                                   // really needed size
    uint64_t    sizeAllocated;                          // size which can be larger because of alignment
    uint64_t    offsetToBaseAddress;                    // offset to base address of memory block
    void        *address;                               // absolute physical address in memory
} AFAProcessingWorkData_t;



typedef struct
{
    // training data
    AFASpectra_SW           *spectraDataInput;
    uint32_t                *spectraDataInputHW;

    // code book spectra
    AFASpectra_SW           *spectraDataWorkingSet;
    uint32_t                *spectraDataWorkingSetHW;

    // determine processing order. must be randomized every learning step
    // contains m_gridSize * m_gridSize  elements
    sint32_t                *m_pSpectraIndexList;


    AFASpectra_SW           **m_localSearchSpectraVec;
    sint32_t                *m_localSearchIndexVec;
    float32_t               *m_localSearchErrorVec;


    // number of source spectra
    uint32_t                m_numSpectra;

    // current learning step
    uint32_t                currentStep;
    AFAParameters           m_params;

    // grid size in cells of the map
    uint32_t                m_gridSize;
    // squared grid size, number of neurons
    uint32_t                m_gridSizeSqr;

    // memory needs
    AFAProcessingWorkData_t workData[ 16 ];
    uint32_t                workDataNumRecords;
    uint64_t                memoryBlockSizeAllocated;
    void                    *memoryBlockBaseAddressAllocated;
    uint64_t                memoryBlockSizeNeeded;
    void                    *memoryBlockBaseAddressAligned;
} AFAProcessingParamSW_t;

typedef struct
{
    // number of source spectra
    uint32_t        m_numSpectra;

    AFAParameters   m_params;

    // grid size in cells of the map
    uint32_t        m_gridSize;

    // squared grid size, number of neurons
    uint32_t        m_gridSizeSqr;

} AFAProcessingParamHW_t;

typedef struct	// packing is essential here: see readout in AFAProcess_HW
{
	uint64_t	memAccess_AFAProcess_HW;
	uint64_t	memAccess_adaptNetwork_HW_read;
	uint64_t	memAccess_adaptNetwork_HW_write;
	uint64_t	memAccess_searchBestMatchComplete_HW;
} AFAStatistics_t;

typedef struct	// packing is essential here: see readout in AFAProcess_HW
{
	AFAStatistics_t	stats;
} AFAReadBackData_t;

// one learning step
// returns true if learning is finished and maximum number of learning steps are reached.

bool_t
AFAProcess_HWWrapper(
	uint32_t param[ 256 ],
	volatile uint32_t *baseAddr 		// default starting address in memory
    );
	
void
AFAProcess_HW(
    volatile uint32_t *baseAddr, 		// default starting address in memory
	volatile uint1_t *interrupt
    );

#endif
