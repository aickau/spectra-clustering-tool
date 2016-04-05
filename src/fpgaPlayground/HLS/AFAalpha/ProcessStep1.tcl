open_project AFAHls2015.4

set all_solution [list AFAProcessing_64Bit_250MHz-2 ]
set all_part     [list xc7vx690tffg1761-2           ]
set all_clocks   [list 4ns                          ]

#set all_solution [list AFAProcessing_64Bit_250MHz-2 AFAProcessing_64Bit_400MHz-2 ]
#set all_part     [list xc7vx690tffg1761-2           xc7vx690tffg1761-2           ]
#set all_clocks   [list 4ns                          2.5ns                        ]

#set all_solution [list AFAProcessing_64Bit_250MHz-1 AFAProcessing_64Bit_250MHz-2 AFAProcessing_64Bit_333MHz-1 AFAProcessing_64Bit_333MHz-2 AFAProcessing_64Bit_400MHz-1 AFAProcessing_64Bit_400MHz-2]
#set all_part     [list xc7vx690tffg1761-1           xc7vx690tffg1761-2           xc7vx690tffg1761-1           xc7vx690tffg1761-2           xc7vx690tffg1761-1           xc7vx690tffg1761-2          ]
#set all_clocks   [list 4ns                          4ns                          3.333ns                      3.333ns                      2.5ns                        2.5ns                       ]

# Hardware entry function
set_top AFAProcess_HW

# Files to build the hardware
add_files     src/AFADefines.h
add_files     src/AFANetworkSettingsHW.h
add_files     src/AFAProcessingHW.c
add_files     src/AFAProcessingHW.h
add_files     src/AFATypes.h

# Testbench files
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

foreach solution $all_solution part $all_part clock $all_clocks {
	open_solution -reset $solution
	set_part             $part
    create_clock -period $clock -name default

	config_interface -m_axi_addr64 -m_axi_offset off -register_io off

#	csim_design -clean
	csynth_design
#    cosim_design -rtl vhdl
#    export_design -format ip_catalog

#	report_timing_summary -return_string
#	report_utilization -file ReportJSC.txt -hierarchical -slr -pblocks -name JeskoTest
	export_design -format ip_catalog -evaluate vhdl -description $solution-$part-$clock -library Jesko -vendor SystemberatungSchwarzer -version 1.0
}

exit