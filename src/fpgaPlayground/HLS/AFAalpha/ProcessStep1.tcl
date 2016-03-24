############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project AFAHls2015.4
set_top AFAProcess_HW
add_files src/AFADefines.h
add_files src/AFANetworkSettingsHW.h
add_files src/AFAProcessingHW.c
add_files src/AFAProcessingHW.h
add_files src/AFARandomHW.c
add_files src/AFARandomHW.h
add_files src/AFASpectraHW.c
add_files src/AFASpectraHW.h
add_files src/AFATypes.h
add_files -tb src/AFADefines.h
add_files -tb src/AFANetworkSettings.c
add_files -tb src/AFANetworkSettings.h
add_files -tb src/AFANetworkSettingsCommon.h
add_files -tb src/AFAProcessing.c
add_files -tb src/AFAProcessing.h
add_files -tb src/AFAProcessingCommon.h
add_files -tb src/AFARandom.c
add_files -tb src/AFARandom.h
add_files -tb src/AFARandomCommon.h
add_files -tb src/AFASpectra.c
add_files -tb src/AFASpectra.h
add_files -tb src/AFASpectraCommon.h
add_files -tb src/AFATypes.c
add_files -tb src/AFATypes.h
add_files -tb src/main.c
open_solution "AFAProcessing_64Bit_250MHz"
set_part {xc7vx690tffg1761-2}
create_clock -period 4ns -name default
config_interface -m_axi_addr64 -m_axi_offset off -register_io off
#source "./AFAHls2015.4/AFAProcessing_64Bit_250MHz/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -format ip_catalog
