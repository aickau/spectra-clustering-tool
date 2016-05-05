source _ProcessSourceFiles.tcl
## Hardware entry function
#set_top AFAProcess_HW
#
## Files to build the hardware
#add_files     $src_dir/AFADefines.h
#add_files     $src_dir/AFANetworkSettingsHW.h
#add_files     $src_dir/AFAProcessingHW.c
#add_files     $src_dir/AFAProcessingHW.h
#add_files     $src_dir/AFATypes.h
#
## Testbench files
#add_files -tb $src_dir/AFADefines.h
#add_files -tb $src_dir/AFANetworkSettings.c
#add_files -tb $src_dir/AFANetworkSettings.h
#add_files -tb $src_dir/AFANetworkSettingsCommon.h
#add_files -tb $src_dir/AFAProcessing.c
#add_files -tb $src_dir/AFAProcessing.h
#add_files -tb $src_dir/AFAProcessingHWWrapper.c
#add_files -tb $src_dir/AFAProcessingCommon.h
#add_files -tb $src_dir/AFARandom.c
#add_files -tb $src_dir/AFARandom.h
#add_files -tb $src_dir/AFARandomCommon.h
#add_files -tb $src_dir/AFASpectra.c
#add_files -tb $src_dir/AFASpectra.h
#add_files -tb $src_dir/AFASpectraCommon.h
#add_files -tb $src_dir/AFATypes.c
#add_files -tb $src_dir/AFATypes.h
#add_files -tb $src_dir/main.c

foreach solution $all_solution part $all_part clock $all_clocks axi_addr $all_axi_addr {
	puts "axi_addr=$axi_addr"
	open_solution -reset $solution
	set_part             $part
    create_clock -period $clock -name default
	config_interface -m_axi_offset off -register_io off
#	config_interface $axi_addr

#	csim_design -clean
	csynth_design
#    cosim_design -rtl vhdl
#    export_design -format ip_catalog

#	report_timing_summary -return_string
#	report_utilization -file ReportJSC.txt -hierarchical -slr -pblocks -name JeskoTest
#	export_design -format ip_catalog -evaluate vhdl -description $solution-$part-$clock -library Jesko -vendor SystemberatungSchwarzer -version 1.0
#	set_property core_revision 4711
#	set_property core_revision [expr [get_property core_revision [ipx::current_core]] + 1] [ipx::current_core]
	export_design -format ip_catalog -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $version
#	export_design -format ip_catalog -evaluate vhdl -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $version
}

exit