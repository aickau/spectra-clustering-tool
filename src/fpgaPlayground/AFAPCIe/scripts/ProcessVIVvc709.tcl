##################################################
#
# Jesko Schwarzer, 2016/04-05
# email: hls@schwarzers.de
# mobile: +49 163 8442071
#
# free for personal use, may the force be with you
#
##################################################

# AFA - Create my project
# =======================

# =============================================
# call this file with
# vivado -mode batch -source ProcessVIVvc709.tcl
# =============================================

# user modifyable area:
# ---------------------

## == Provide important information ============================

# ==> "platform_name" can be: arty, vc709

# Set the directory path for the new project
set proj_name_long          pciePIO				;# directory
set proj_name_short         axi_pcie_pio		;# prefix
set platform_name           vc709
set bd_design_name          axi_pcie_mig
set design_dir              "d:/AFAPCIe/viv"
set repo_dir                "d:/AFAPCIe/repoHW"
set resource_files          "res"
set scripts_vivado_version  2016.3

set design_version_VIV_H    1
set design_version_VIV_L    0

# do we want to generate the framework with our custom element or not ?
set CustomIPEnable          no
set CustomIPName            AFAProcessHW
set CustomIPNameFull        SystemberatungSchwarzer:AFAProcessingLib:AFAProcess_HW:0.3008
# do not modify below this line ...
## =============================================================

## == Prepare directory variables ==============================
set cur_dir                 [pwd]
set prj_dir_repo            "[file normalize "$repo_dir/$platform_name"]"
set proj_resources_dir		"[file normalize "$cur_dir/$resource_files/$scripts_vivado_version"]"

set target_board            $platform_name

##-----------------------------------------------------------
## Archive existing design if it already exists
##-----------------------------------------------------------
#puts "NOTE: Archive existing $design_name_full design if it exists"
#set format_date [clock format [clock seconds] -format %Y%m%d_%H%m]
#set date_suffix _$format_date
#puts "Date: $format_date"
#if { [file exists "$design_dir/$design_name_full"] == 1 } { 
#  puts "Moving existing $design_name_full to time-stamped suffix $design_name_full$date_suffix"
#  file rename "$design_dir/$design_name_full" "$design_dir/$design_name_full$date_suffix"
#} else {
#  file mkdir "$design_dir"
#}

#-----------------------------------------------------------
# Create project
#-----------------------------------------------------------
puts "Creating project for $proj_name_long ..."
if { $target_board == "arty" } {
	set target_part xc7a35ticsg324-1L
	set board_property digilentinc.com:arty:part0:1.1
} elseif { $target_board == "vc709" } {
	set target_part xc7vx690tffg1761-2
	set board_property xilinx.com:vc709:part0:1.7
} else {
	puts "ERROR! Selected board '$target_board' is not supported."
	exit
}

create_project $proj_name_short "$design_dir/$proj_name_long" -part $target_part
set_property board $board_property [current_project]

#-----------------------------------------------------------
# Add HDL IP repositories
#-----------------------------------------------------------
#set_property ip_repo_paths "$prj_dir_repo" [current_fileset]
set_property ip_repo_paths $prj_dir_repo [current_project]
update_ip_catalog -rebuild

#-----------------------------------------------------------
# Create BD source
#-----------------------------------------------------------
puts "Creating block diagram..."
source "./ProcessVIVbd_$target_board.tcl"
save_bd_design

#-----------------------------------------------------------
# Create wrapper code
#-----------------------------------------------------------

puts $design_dir/$proj_name_long
make_wrapper -files [get_files $design_dir/$proj_name_long/$proj_name_short\.srcs/sources_1/bd/$bd_design_name/$bd_design_name\.bd] -top
add_files -norecurse $design_dir/$proj_name_long/$proj_name_short\.srcs/sources_1/bd/$bd_design_name/hdl/$bd_design_name\_wrapper.v
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

#-----------------------------------------------------------
# Create constraints file
#-----------------------------------------------------------

source "./ProcessVIVconstraints_$target_board.tcl"

##-----------------------------------------------------------
## Synthesize design
##-----------------------------------------------------------
#launch_runs synth_1 -jobs 4
#wait_on_run [current_run]
#open_run synth_1 -name synth_1
#
##-----------------------------------------------------------
## Setup debug core
##-----------------------------------------------------------
#source "./ProcessVIVdebug_$target_board.tcl"
#
exit
#-----------------------------------------------------------
# Generate bitstream
#-----------------------------------------------------------
launch_runs impl_1 -to_step write_bitstream -jobs 6
wait_on_run [current_run]

#-----------------------------------------------------------
# Export to SDK
#-----------------------------------------------------------
file mkdir $design_dir/$proj_name_long/$proj_name_short\.sdk
file copy -force $design_dir/$proj_name_long/$proj_name_short\.runs/impl_1/design_1_wrapper.sysdef $design_dir/$proj_name_long/$proj_name_short\.sdk/$bd_design_name\_wrapper.hdf

#-----------------------------------------------------------
# Start SDK
#-----------------------------------------------------------
launch_sdk -workspace $design_dir/$proj_name_long/$proj_name_short\.sdk -hwspec $design_dir/$proj_name_long/$proj_name_short\.sdk/$bd_design_name\_wrapper.hdf
