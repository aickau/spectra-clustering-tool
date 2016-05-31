source $prj_dir_scripts/_ProcessHLSSourceFiles.tcl

foreach solution $all_solution part $all_part clock $all_clocks axi_addr $all_axi_addr repo_dir_new $all_repo_dir {
	puts "axi_addr=$axi_addr"
	open_solution -reset $solution
	set_part             $part
    create_clock -period $clock -name default
	config_interface -m_axi_offset off -register_io off
#	config_interface $axi_addr

# clean build
#csim_design -clean
# compile only
#csim_design -clean -setup

#exit

	csynth_design
#cosim_design -rtl vhdl

	#pack and export design
	export_design -format ip_catalog -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $versionH.$versionL -display_name $display_name
	#export_design -format ip_catalog -evaluate vhdl -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $versionH.$versionL -display_name $display_name
	#export_design -format ip_catalog -evaluate verilog -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $versionH.$versionL -display_name $display_name

	#copy result to repo
	file mkdir $repo_dir_new
	file copy $prj_dir_hls/$proj_name/$solution/impl/ip/$vendor\_$libraryname\_$top_function\_$versionH\_$versionL\.zip $repo_dir_new
}
exit