#ifndef AFA_PROCESSING_COMMON_H__
#define AFA_PROCESSING_COMMON_H__

#include "AFATypes.h"

enum
{
    AFA_MEMORY_ALIGNMENT_HUGE_BLOCKS    = 1 << 8,   // power of 2 needed: 256

    AFA_WORKING_DATA_NAME_LENGTH        = 32,

    AFA_WRAPPER_WORK_MODE_HW            = 0,
    AFA_WRAPPER_WORK_MODE_SW            = 1,

    AFA_PARAM_INDICES_RESERVED          = 0,
    AFA_PARAM_INDICES_SIZE,
    AFA_PARAM_INDICES_INPUT_HW_ADDR_LOW,
    AFA_PARAM_INDICES_INPUT_HW_ADDR_HIGH,
    AFA_PARAM_INDICES_OUTPUT_HW_ADDR_LOW,
    AFA_PARAM_INDICES_OUTPUT_HW_ADDR_HIGH,
    AFA_PARAM_INDICES_LED1_OUTPUT
};

typedef struct
{
    char        name[ AFA_WORKING_DATA_NAME_LENGTH ];   // name of data
    uint64_t    size;                                   // really needed size
    uint64_t    sizeAllocated;                          // size which can be larger because of alignment
    uint64_t    offsetToBaseAddress;                    // offset to base address of memory block
    void        *address;                               // absolute physical address in memory
} AFAProcessingWorkData_t;

bool_t
AFAProcess_HWWrapper(
    volatile uint32_t param[ 256 ],              // whole block ram used
    volatile uint32_t *baseAddr,
    uint32_t workMode
    );

uint32_t
AFAProcess_HW(
    volatile uint32_t param[ 256 ],              // whole block ram used
    volatile uint32_t *baseAddr
    );

#endif
