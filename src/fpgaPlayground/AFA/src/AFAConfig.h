#ifndef AFA_CONFIG_H_
#define AFA_CONFIG_H_

// ===============================================================
//
// 4 variants:
// ===========
//
// select one at a time
//
// 1) Runs in VisualStudio:
//#define AFA_RUN_ON_VISUAL_STUDIO
// 2) Runs in Vivado_hls - CSimulation:
//#define AFA_RUN_ON_VIVADO_HLS_CSIM
// 3) Runs in Vivado_hls - CoSimulation:
//#define AFA_RUN_ON_VIVADO_HLS_COSIM
// 4) Runs in Xilinx SDK
#define AFA_RUN_ON_XILINX_SDK
//
// Option: Runs in Xilinx SDK - with AFA on real HW:
#define AFA_RUN_PROCESSHW_HW
// ===============================================================

#endif
