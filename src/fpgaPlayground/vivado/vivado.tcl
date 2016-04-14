# AFA - Create my project
# =======================

# user modifyable area:
# ---------------------

# Set the reference directory for source file relative paths (by default the value is script directory path)
set origin_dir "."
variable script_file
set script_file "vivado.tcl"
# Set the directory path for the new project
set proj_dir "./SimpleDesign"
set proj_name "SD"
set proj_board "xilinx.com:vc709:part0:1.7"
set proj_device "xc7vx690tffg1761-2"
set proj_resources_dir "res"
variable proj_sources_name
set proj_sources_name sources_1
set proj_constraints_name constrs_1
set proj_simulation_name sim_1
set proj_synthesis_name synth_1
set proj_implementation_name impl_1

# files essential to build the system
set resource_block_design_filename      "SBD.bd"
set resource_design_wrapper_filename    "SBD_wrapper.v"
set resource_board_description_filename "board.prj"

# ===============================================================================
# do not modify below this line ...

set resource_block_design_file      "$origin_dir/$proj_resources_dir/$resource_block_design_filename"
set resource_design_wrapper_file    "$origin_dir/$proj_resources_dir/$resource_design_wrapper_filename"
set resource_board_description_file "$origin_dir/$proj_resources_dir/$resource_board_description_filename"


# Use origin directory path location variable, if specified in the tcl shell
if { [info exists ::origin_dir_loc] } {
  set origin_dir $::origin_dir_loc
}

# Create project
create_project -force -part $proj_device $proj_name $proj_dir

# Set project properties
set obj [get_projects $proj_name]
set_property "board_part" $proj_board $obj
set_property "compxlib.activehdl_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/activehdl" $obj
set_property "compxlib.funcsim" "1" $obj
set_property "compxlib.ies_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/ies" $obj
set_property "compxlib.modelsim_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/modelsim" $obj
set_property "compxlib.overwrite_libs" "0" $obj
set_property "compxlib.questa_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/questa" $obj
set_property "compxlib.riviera_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/riviera" $obj
set_property "compxlib.timesim" "1" $obj
set_property "compxlib.vcs_compiled_library_dir" "$proj_dir/$proj_name.cache/compile_simlib/vcs" $obj
set_property "corecontainer.enable" "0" $obj
set_property "default_lib" "xil_defaultlib" $obj
set_property "enable_optional_runs_sta" "0" $obj
set_property "ip_cache_permissions" "" $obj
set_property "ip_output_repo" "" $obj
set_property "managed_ip" "0" $obj
set_property "sim.ip.auto_export_scripts" "1" $obj
set_property "simulator_language" "Mixed" $obj
set_property "source_mgmt_mode" "All" $obj
#set_property "target_language" "Verilog" $obj
set_property "target_language" "VHDL" $obj
set_property "target_simulator" "XSim" $obj

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets -quiet $proj_sources_name] ""]} {
  create_fileset -srcset $proj_sources_name
}

# =======================================================================================================
# =============================================   Sources   =============================================
# =======================================================================================================

# Set 'sources_1' fileset object
set obj [get_filesets $proj_sources_name]
set files [list \
 "[file normalize "$resource_block_design_file"]"\
 "[file normalize "$resource_design_wrapper_file"]"\
 "[file normalize "$resource_board_description_file"]"\
]
add_files -norecurse -fileset $obj $files

# Set 'sources_1' fileset file properties for remote files

# set properties for BlockDesign ===========================================================

set file "$resource_block_design_file"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets $proj_sources_name] [list "*$file"]]
if { ![get_property "is_locked" $file_obj] } {
  set_property "generate_synth_checkpoint" "0" $file_obj
}
set_property "is_enabled" "1" $file_obj
set_property "is_global_include" "0" $file_obj
if { ![get_property "is_locked" $file_obj] } {
  set_property "is_locked" "0" $file_obj
}
set_property "library" "xil_defaultlib" $file_obj
set_property "path_mode" "RelativeFirst" $file_obj
if { ![get_property "is_locked" $file_obj] } {
  set_property "synth_checkpoint_mode" "None" $file_obj
}
set_property "used_in" "synthesis implementation simulation" $file_obj
set_property "used_in_implementation" "1" $file_obj
set_property "used_in_simulation" "1" $file_obj
set_property "used_in_synthesis" "1" $file_obj

# set properties for DesignWrapper =========================================================
set file "$resource_design_wrapper_file"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets $proj_sources_name] [list "*$file"]]
set_property "file_type" "Verilog" $file_obj
set_property "is_enabled" "1" $file_obj
set_property "is_global_include" "0" $file_obj
set_property "library" "xil_defaultlib" $file_obj
set_property "path_mode" "RelativeFirst" $file_obj
set_property "used_in" "synthesis implementation simulation" $file_obj
set_property "used_in_implementation" "1" $file_obj
set_property "used_in_simulation" "1" $file_obj
set_property "used_in_synthesis" "1" $file_obj

# set properties for BoardDescription ======================================================
set file "$resource_board_description_file"
set file [file normalize $file]
set file_obj [get_files -of_objects [get_filesets $proj_sources_name] [list "*$file"]]
set_property "is_enabled" "1" $file_obj
set_property "is_global_include" "0" $file_obj
set_property "library" "xil_defaultlib" $file_obj
set_property "path_mode" "RelativeFirst" $file_obj
set_property "scoped_to_cells" "" $file_obj
set_property "scoped_to_ref" "" $file_obj
set_property "used_in" "synthesis" $file_obj
set_property "used_in_synthesis" "1" $file_obj

# Set 'sources_1' fileset file properties for local files
# None

# Set 'sources_1' fileset properties
set obj [get_filesets $proj_sources_name]
set_property "design_mode" "RTL" $obj
set_property "edif_extra_search_paths" "" $obj
set_property "elab_link_dcps" "1" $obj
set_property "elab_load_timing_constraints" "1" $obj
set_property "generic" "" $obj
set_property "include_dirs" "" $obj
set_property "lib_map_file" "" $obj
set_property "loop_count" "1000" $obj
set_property "name" "$proj_sources_name" $obj
set_property "top" "SBD_wrapper" $obj
set_property "verilog_define" "" $obj
set_property "verilog_uppercase" "0" $obj

# =======================================================================================================
# ============================================= Constraints =============================================
# =======================================================================================================

# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets -quiet $proj_constraints_name] ""]} {
  create_fileset -constrset $proj_constraints_name
}

# Set '$proj_constraints_name' fileset object
set obj [get_filesets $proj_constraints_name]

# Empty (no sources present)

# Set '$proj_constraints_name' fileset properties
set obj [get_filesets $proj_constraints_name]
set_property "name" "$proj_constraints_name" $obj
set_property "target_constrs_file" "" $obj

# =======================================================================================================
# ============================================= Simulation  =============================================
# =======================================================================================================

# Create 'sim_1' fileset (if not found)
if {[string equal [get_filesets -quiet $proj_simulation_name] ""]} {
  create_fileset -simset $proj_simulation_name
}

# Set '$proj_simulation_name' fileset object
set obj [get_filesets $proj_simulation_name]
# Empty (no sources present)

# Set '$proj_simulation_name' fileset properties
set obj [get_filesets $proj_simulation_name]
set_property "generic" "" $obj
set_property "include_dirs" "" $obj
set_property "name" "$proj_simulation_name" $obj
set_property "nl.cell" "" $obj
set_property "nl.incl_unisim_models" "0" $obj
set_property "nl.process_corner" "slow" $obj
set_property "nl.rename_top" "" $obj
set_property "nl.sdf_anno" "1" $obj
set_property "nl.write_all_overrides" "0" $obj
set_property "runtime" "1000ns" $obj
set_property "source_set" "$proj_sources_name" $obj
set_property "top" "SBD_wrapper" $obj
set_property "unit_under_test" "" $obj
set_property "verilog_define" "" $obj
set_property "verilog_uppercase" "0" $obj
set_property "xelab.debug_level" "typical" $obj
set_property "xelab.dll" "0" $obj
set_property "xelab.load_glbl" "1" $obj
set_property "xelab.more_options" "" $obj
set_property "xelab.mt_level" "auto" $obj
set_property "xelab.nosort" "1" $obj
set_property "xelab.rangecheck" "0" $obj
set_property "xelab.relax" "1" $obj
set_property "xelab.sdf_delay" "sdfmax" $obj
set_property "xelab.snapshot" "" $obj
set_property "xelab.unifast" "" $obj
set_property "xsim.compile.xvhdl.more_options" "" $obj
set_property "xsim.compile.xvhdl.nosort" "1" $obj
set_property "xsim.compile.xvhdl.relax" "1" $obj
set_property "xsim.compile.xvlog.more_options" "" $obj
set_property "xsim.compile.xvlog.nosort" "1" $obj
set_property "xsim.compile.xvlog.relax" "1" $obj
set_property "xsim.elaborate.debug_level" "typical" $obj
set_property "xsim.elaborate.load_glbl" "1" $obj
set_property "xsim.elaborate.mt_level" "auto" $obj
set_property "xsim.elaborate.rangecheck" "0" $obj
set_property "xsim.elaborate.relax" "1" $obj
set_property "xsim.elaborate.sdf_delay" "sdfmax" $obj
set_property "xsim.elaborate.snapshot" "" $obj
set_property "xsim.elaborate.xelab.more_options" "" $obj
set_property "xsim.more_options" "" $obj
set_property "xsim.saif" "" $obj
set_property "xsim.simulate.runtime" "1000ns" $obj
set_property "xsim.simulate.saif" "" $obj
set_property "xsim.simulate.saif_all_signals" "0" $obj
set_property "xsim.simulate.uut" "" $obj
set_property "xsim.simulate.wdb" "" $obj
set_property "xsim.simulate.xsim.more_options" "" $obj
set_property "xsim.tclbatch" "" $obj
set_property "xsim.view" "" $obj
set_property "xsim.wdb" "" $obj

# =======================================================================================================
# =============================================  Synthesis  =============================================
# =======================================================================================================

# Create 'synth_1' run (if not found)
if {[string equal [get_runs -quiet $proj_synthesis_name] ""]} {
  puts "if: $proj_synthesis_name"
  create_run -name $proj_synthesis_name -part $proj_device -flow {Vivado Synthesis 2015} -strategy "Vivado Synthesis Defaults" -constrset $proj_constraints_name
} else {
  puts "else"
  set_property strategy "Vivado Synthesis Defaults" [get_runs $proj_synthesis_name]
  set_property flow "Vivado Synthesis 2015" [get_runs $proj_synthesis_name]

}
set obj [get_runs $proj_synthesis_name]

set_property "constrset" "$proj_constraints_name" $obj
set_property "description" "JSC-Script: Vivado Synthesis Defaults" $obj
set_property "flow" "Vivado Synthesis 2015" $obj
set_property "name" "$proj_synthesis_name" $obj
set_property "needs_refresh" "0" $obj
set_property "srcset" "$proj_sources_name" $obj
set_property "strategy" "Vivado Synthesis Defaults" $obj
set_property "incremental_checkpoint" "" $obj
set_property "include_in_archive" "1" $obj
set_property "steps.synth_design.tcl.pre" "" $obj
set_property "steps.synth_design.tcl.post" "" $obj
set_property "steps.synth_design.args.flatten_hierarchy" "rebuilt" $obj
set_property "steps.synth_design.args.gated_clock_conversion" "off" $obj
set_property "steps.synth_design.args.bufg" "12" $obj
set_property "steps.synth_design.args.fanout_limit" "10000" $obj
set_property "steps.synth_design.args.directive" "Default" $obj
set_property "steps.synth_design.args.fsm_extraction" "auto" $obj
set_property "steps.synth_design.args.keep_equivalent_registers" "0" $obj
set_property "steps.synth_design.args.resource_sharing" "auto" $obj
set_property "steps.synth_design.args.control_set_opt_threshold" "auto" $obj
set_property "steps.synth_design.args.no_lc" "0" $obj
set_property "steps.synth_design.args.shreg_min_size" "3" $obj
set_property "steps.synth_design.args.max_bram" "-1" $obj
set_property "steps.synth_design.args.max_dsp" "-1" $obj
set_property "steps.synth_design.args.cascade_dsp" "auto" $obj
set_property -name {steps.synth_design.args.more options} -value {} -objects $obj

# =======================================================================================================
# ============================================= Implementation ==========================================
# =======================================================================================================

# Create 'impl_1' run (if not found)
if {[string equal [get_runs -quiet $proj_implementation_name] ""]} {
  create_run -name $proj_implementation_name -part $proj_device -flow {Vivado Implementation 2015} -strategy "Vivado Implementation Defaults" -constrset $proj_constraints_name -parent_run $proj_synthesis_name
} else {
  set_property strategy "Vivado Implementation Defaults" [get_runs $proj_implementation_name]
  set_property flow "Vivado Implementation 2015" [get_runs $proj_implementation_name]
}
set obj [get_runs $proj_implementation_name]

set_property "constrset" "$proj_constraints_name" $obj
set_property "description" "JSC-Script: Vivado Implementation Defaults" $obj
set_property "flow" "Vivado Implementation 2015" $obj
set_property "name" "$proj_implementation_name" $obj
set_property "needs_refresh" "0" $obj
set_property "srcset" "$proj_sources_name" $obj
set_property "strategy" "Vivado Implementation Defaults" $obj
set_property "incremental_checkpoint" "" $obj
set_property "include_in_archive" "1" $obj
set_property "steps.opt_design.is_enabled" "1" $obj
set_property "steps.opt_design.tcl.pre" "" $obj
set_property "steps.opt_design.tcl.post" "" $obj
set_property "steps.opt_design.args.verbose" "0" $obj
set_property "steps.opt_design.args.directive" "Default" $obj
set_property -name {steps.opt_design.args.more options} -value {} -objects $obj
set_property "steps.power_opt_design.is_enabled" "0" $obj
set_property "steps.power_opt_design.tcl.pre" "" $obj
set_property "steps.power_opt_design.tcl.post" "" $obj
set_property -name {steps.power_opt_design.args.more options} -value {} -objects $obj
set_property "steps.place_design.tcl.pre" "" $obj
set_property "steps.place_design.tcl.post" "" $obj
set_property "steps.place_design.args.directive" "Default" $obj
set_property -name {steps.place_design.args.more options} -value {} -objects $obj
set_property "steps.post_place_power_opt_design.is_enabled" "0" $obj
set_property "steps.post_place_power_opt_design.tcl.pre" "" $obj
set_property "steps.post_place_power_opt_design.tcl.post" "" $obj
set_property -name {steps.post_place_power_opt_design.args.more options} -value {} -objects $obj
set_property "steps.phys_opt_design.is_enabled" "0" $obj
set_property "steps.phys_opt_design.tcl.pre" "" $obj
set_property "steps.phys_opt_design.tcl.post" "" $obj
set_property "steps.phys_opt_design.args.directive" "Default" $obj
set_property -name {steps.phys_opt_design.args.more options} -value {} -objects $obj
set_property "steps.route_design.tcl.pre" "" $obj
set_property "steps.route_design.tcl.post" "" $obj
set_property "steps.route_design.args.directive" "Default" $obj
set_property -name {steps.route_design.args.more options} -value {} -objects $obj
set_property "steps.post_route_phys_opt_design.is_enabled" "0" $obj
set_property "steps.post_route_phys_opt_design.tcl.pre" "" $obj
set_property "steps.post_route_phys_opt_design.tcl.post" "" $obj
set_property "steps.post_route_phys_opt_design.args.directive" "Default" $obj
set_property -name {steps.post_route_phys_opt_design.args.more options} -value {} -objects $obj
set_property "steps.write_bitstream.tcl.pre" "" $obj
set_property "steps.write_bitstream.tcl.post" "" $obj
set_property "steps.write_bitstream.args.raw_bitfile" "0" $obj
set_property "steps.write_bitstream.args.mask_file" "0" $obj
set_property "steps.write_bitstream.args.no_binary_bitfile" "0" $obj
set_property "steps.write_bitstream.args.bin_file" "0" $obj
set_property "steps.write_bitstream.args.readback_file" "0" $obj
set_property "steps.write_bitstream.args.logic_location_file" "0" $obj
set_property "steps.write_bitstream.args.verbose" "0" $obj
set_property -name {steps.write_bitstream.args.more options} -value {} -objects $obj

# =======================================================================================================
# ============================================= Runs           ==========================================
# =======================================================================================================

# set the current synth run
current_run -synthesis [get_runs $proj_synthesis_name]

# set the current impl run
current_run -implementation [get_runs $proj_implementation_name]

puts "INFO: Project created: '$proj_name' in '$proj_dir' with device $proj_device"

launch_runs $proj_implementation_name -jobs 2
wait_on_run $proj_implementation_name
launch_runs $proj_implementation_name -to_step write_bitstream -jobs 2
wait_on_run $proj_implementation_name


# =======================================================================================================
# ============================================= GUI            ==========================================
# =======================================================================================================
puts "INFO: Project compiled: '$proj_name' in '$proj_dir' with device $proj_device"

start_gui
open_bd_design {"$resource_block_design_file"}

#exit 0
