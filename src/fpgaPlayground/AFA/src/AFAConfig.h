#ifndef AFA_CONFIG_H_
#define AFA_CONFIG_H_

#define AFA_KB      ( 1024 )
#define AFA_MB      ( 1024 * 1024 )
#define AFA_GB      ( 1024 * 1024 * 1024 )

// ===============================================================
//
// 4 variants:
// ===========
//
// select one at a time
//
// 1) Runs in VisualStudio:
#define AFA_RUN_ON_VISUAL_STUDIO
// 2) Runs in Vivado_hls - CSimulation:
//#define AFA_RUN_ON_VIVADO_HLS_CSIM
// 3) Runs in Vivado_hls - CoSimulation:
//#define AFA_RUN_ON_VIVADO_HLS_COSIM
// 4) Runs in Xilinx SDK
//#define AFA_RUN_ON_XILINX_SDK
//
// Option: Runs in Xilinx SDK - with AFA on real HW:
//#define AFA_RUN_PROCESSHW_HW
// with option: AP interface (start/stop/ready/idle)
//#define AFA_RUN_PROCESSHW_HW_INTERFACE_AP
// with option: memory polling, AXI-Master only
//#define AFA_RUN_PROCESSHW_HW_INTERFACE_RAW

// ===============================================================

// this is an index into an array of type uint32_t starting at address 0x00000000
//#define PARAM_BLOCK_ADDRESS_INDEX	( 0x82000000 / sizeof( uint32_t ))
#define PARAM_BLOCK_ADDRESS_INDEX	( 0x00000000 / sizeof( uint32_t ))
#define AFA_HW_MEMORY_SIZE          ( 256 * AFA_MB )

#endif
