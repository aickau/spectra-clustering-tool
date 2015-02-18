############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project proj_axi_master
set_top AFAProcessFunction
add_files proj_axi_master/src/AFAProcessFunction.cpp
add_files include/AFAProcessing.h
add_files include/AFASpectra.h
add_files -tb AFANetworkSettings.cpp
add_files -tb include/AFAProcessFunction.h
add_files -tb AFAProcessing.cpp
add_files -tb AFARandom.cpp
add_files -tb AFASpectra.cpp
add_files -tb main.cpp
open_solution "solution1"
set_part {xc7z020clg484-2}
create_clock -period 75MHz -name default
source "./proj_axi_master/solution1/directives.tcl"
csim_design -argv {test abc123} -clean
csynth_design
cosim_design -argv {test abc123} -rtl vhdl -tool xsim
export_design -format ip_catalog
