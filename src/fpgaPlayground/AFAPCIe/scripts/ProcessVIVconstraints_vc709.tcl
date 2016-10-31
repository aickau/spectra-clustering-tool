#set constraintCode1 {set_property -dict { PACKAGE_PIN G18    IOSTANDARD LVCMOS33 } [get_ports { eth_ref_clk }]; #         Sch=eth_ref_clk}


## #################################################
## constraints for VC709 with Vivado 2016.3 and PCIe
## setting the clock and control LEDs
## #################################################
set constraintCode1  {create_clock -period 10.000 -name CLK_IN_D_clk_p -waveform {0.000 5.000} [get_ports CLK_IN_D_clk_p]]; #sets the PCIe reference clock}
set constraintCode2  {set_property PACKAGE_PIN AM39    [get_ports init_calib_complete]}
set constraintCode3  {set_property IOSTANDARD LVCMOS18 [get_ports init_calib_complete]}
set constraintCode4  {set_property PACKAGE_PIN AN39    [get_ports c1_init_calib_complete]}
set constraintCode5  {set_property IOSTANDARD LVCMOS18 [get_ports c1_init_calib_complete]}
set constraintCode6  {set_property PACKAGE_PIN AR37    [get_ports mmcm_locked]}
set constraintCode7  {set_property IOSTANDARD LVCMOS18 [get_ports mmcm_locked]}
set constraintCode8  {set_property PACKAGE_PIN AT37    [get_ports c1_mmcm_locked]}
set constraintCode9  {set_property IOSTANDARD LVCMOS18 [get_ports c1_mmcm_locked]}
set constraintCode10 {set_property PACKAGE_PIN AR35    [get_ports user_link_up]}
set constraintCode11 {set_property IOSTANDARD LVCMOS18 [get_ports user_link_up]}

# create dirs
file mkdir $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1
file mkdir $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new

# create file
set fileID [ open $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc w ]

# put the contents in
puts $fileID $constraintCode1
puts $fileID $constraintCode2
puts $fileID $constraintCode3
puts $fileID $constraintCode4
puts $fileID $constraintCode5
puts $fileID $constraintCode6
puts $fileID $constraintCode7
puts $fileID $constraintCode8
puts $fileID $constraintCode9
puts $fileID $constraintCode10
puts $fileID $constraintCode11

# close file
close $fileID

# add it to constrints pool
add_files -fileset constrs_1                    $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc

# say "it's not used during synthesis"
set_property used_in_synthesis false [get_files $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc]
