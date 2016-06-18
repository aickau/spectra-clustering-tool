source $prj_dir_scripts/_ProcessHLSSourceFiles.tcl

foreach solution $all_solution part $all_part clock $all_clocks axi_addr $all_axi_addr repo_dir_new $all_repo_dir {
	puts "axi_addr=$axi_addr"
	open_solution -reset $solution
	set_part             $part
    create_clock -period $clock -name default
	config_interface -m_axi_offset off -register_io off
#	config_interface $axi_addr

	## == clean build
	#csim_design -clean

	## == compile only
	#csim_design -clean -setup

	## == synthesize design
	csynth_design
	
	## == cosim design
	#cosim_design -rtl vhdl

	#pack and export design
	export_design -format ip_catalog -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $design_version_HLS_H.$design_version_HLS_L -display_name $display_name
	
	## == copy result to repo
	puts =======================================
	puts "* Copying IP ($vendor\_$libraryname\_$top_function\_$design_version_HLS_H\_$design_version_HLS_L\.zip) to repository ($repo_dir_new)"
	
	file mkdir $repo_dir_new
	file copy $prj_dir_hls/$proj_name/$solution/impl/ip/$vendor\_$libraryname\_$top_function\_$design_version_HLS_H\_$design_version_HLS_L\.zip $repo_dir_new/$vendor\_$libraryname\_$top_function\_$design_version_HLS_H\_$design_version_HLS_L-$clock\.zip
	puts "* please unpack for usage ($vendor\_$libraryname\_$top_function\_$design_version_HLS_H\_$design_version_HLS_L-$clock\.zip)"
}
exit

	#export_design -format ip_catalog -evaluate vhdl -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $design_version_HLS_H.$design_version_HLS_L -display_name $display_name
	#export_design -format ip_catalog -evaluate verilog -description $solution-$part-$clock -library $libraryname -vendor $vendor -version $design_version_HLS_H.$design_version_HLS_L -display_name $display_name

