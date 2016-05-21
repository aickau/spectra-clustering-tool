# AFA - Create my project
# =======================

# user modifyable area:
# ---------------------

# target_board can be: arty, vc709, zedboard, zc702
set target_board arty

set enableCustomDesign yes

# Set the directory path for the new project
set design_dir "/prj/AFA/viv"
set design_version v1_0
set proj_name "SD"
set bd_design_name "design_1"

# Set the reference directory for source file relative paths (by default the value is script directory path)
set repoHW "/prj/AFA/viv/repoHW"

# experimental -----
set proj_resources_dir "res"
#set proj_board "xilinx.com:vc709:part0:1.7"
#set proj_device "xc7vx690tffg1761-2"

# ===============================================================================
# do not modify below this line ...
set design_name_full "$proj_name\_$design_version"

#-----------------------------------------------------------
# Archive existing design if it already exists
#-----------------------------------------------------------
puts "NOTE: Archive existing $design_name_full design if it exists"
set format_date [clock format [clock seconds] -format %Y%m%d_%H%m]
set date_suffix _$format_date
puts "Date: $format_date"
if { [file exists "$design_dir/$design_name_full"] == 1 } { 
  puts "Moving existing $design_name_full to time-stamped suffix $design_name_full$date_suffix"
  file rename "$design_dir/$design_name_full" "$design_dir/$design_name_full$date_suffix"
} else {
  file mkdir "$design_dir"
}

#-----------------------------------------------------------
# Create project
#-----------------------------------------------------------
puts "Creating project for $design_name_full..."
if { $target_board == "arty" } {
	set target_part xc7a35ticsg324-1L
	set board_property digilentinc.com:arty:part0:1.1
} elseif { $target_board == "vc709" } {
	set target_part xc7vx690tffg1761-2
	set board_property xilinx.com:vc709:part0:1.7
} elseif { $target_board == "zedboard" } {
	set target_part xc7z020clg484-1
	set board_property em.avnet.com:zed:part0:1.3
} elseif { $target_board == "zc702" } {
	set target_part xc7z020clg484-1
	set board_property xilinx.com:zc702:part0:1.2
} else {
	puts "ERROR! Selected board '$target_board' is not supported."
	exit
}
create_project $design_name_full "$design_dir/$design_name_full" -part $target_part
set_property board $board_property [current_project]

#-----------------------------------------------------------
# Add HDL IP repositories
#-----------------------------------------------------------
#set_property ip_repo_paths "$repoHW" [current_fileset]
set_property ip_repo_paths $repoHW [current_project]
update_ip_catalog -rebuild

#-----------------------------------------------------------
# Create BD source
#-----------------------------------------------------------
puts "Creating block diagram..."
source "./ProcessVIVbd_$target_board.tcl"

#-----------------------------------------------------------
# Create wrapper code
#-----------------------------------------------------------

puts $design_dir/$design_name_full
make_wrapper -files [get_files $design_dir/$design_name_full/$design_name_full\.srcs/sources_1/bd/$bd_design_name/$bd_design_name\.bd] -top
add_files -norecurse $design_dir/$design_name_full/$design_name_full\.srcs/sources_1/bd/$bd_design_name/hdl/$bd_design_name\_wrapper.v
update_compile_order -fileset sources_1
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

#-----------------------------------------------------------
# Create constraints file
#-----------------------------------------------------------

set constraintCode {set_property -dict { PACKAGE_PIN G18    IOSTANDARD LVCMOS33 } [get_ports { eth_ref_clk }]; #         Sch=eth_ref_clk}
file mkdir $design_dir/$design_name_full/$design_name_full\.srcs/constrs_1
file mkdir $design_dir/$design_name_full/$design_name_full\.srcs/constrs_1/new
set fileID [ open $design_dir/$design_name_full/$design_name_full\.srcs/constrs_1/new/eth_ref_clk.xdc w ]
puts $fileID $constraintCode
close $fileID
add_files -fileset constrs_1 $design_dir/$design_name_full/$design_name_full\.srcs/constrs_1/new/eth_ref_clk.xdc

#-----------------------------------------------------------
# Generate bitstream
#-----------------------------------------------------------
launch_runs impl_1 -to_step write_bitstream -jobs 6
wait_on_run [current_run]

#-----------------------------------------------------------
# Export to SDK
#-----------------------------------------------------------
file mkdir $design_dir/$design_name_full/$design_name_full\.sdk
file copy -force $design_dir/$design_name_full/$design_name_full\.runs/impl_1/design_1_wrapper.sysdef $design_dir/$design_name_full/$design_name_full\.sdk/$bd_design_name\_wrapper.hdf

#-----------------------------------------------------------
# Start SDK
#-----------------------------------------------------------
launch_sdk -workspace $design_dir/$design_name_full/$design_name_full\.sdk -hwspec $design_dir/$design_name_full/$design_name_full\.sdk/$bd_design_name\_wrapper.hdf
