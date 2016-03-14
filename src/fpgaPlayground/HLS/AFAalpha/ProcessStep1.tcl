open_project AFAHls2015.4

set_top AFAProcess_HW
add_files     src/AFADefines.h
add_files     src/AFANetworkSettingsHW.h
add_files     src/AFAProcessingHW.c
add_files     src/AFAProcessingHW.h
add_files     src/AFARandomHW.c
add_files     src/AFARandomHW.h
add_files     src/AFASpectraHW.c
add_files     src/AFASpectraHW.h
add_files     src/AFATypes.h
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
add_files -tb src/AFATypes.h
add_files -tb src/main.c

open_solution "AFAProcessing_64Bit_250MHz"
set_part {xc7vx690tffg1761-2}
create_clock -period 2.5ns -name default
config_interface -m_axi_addr64 -m_axi_offset off -register_io off

#source "./AFAalpha/AFAProcessing_64Bit_250MHz/directives.tcl"
csim_design -clean
#csynth_design
#cosim_design -rtl vhdl
#export_design -format ip_catalog
