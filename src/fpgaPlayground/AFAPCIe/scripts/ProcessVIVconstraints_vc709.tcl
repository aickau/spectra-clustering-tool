#set constraintCode1 {set_property -dict { PACKAGE_PIN G18    IOSTANDARD LVCMOS33 } [get_ports { eth_ref_clk }]; #         Sch=eth_ref_clk}


## #################################################
## constraints for VC709 with Vivado 2016.3 and PCIe
## setting the clock and control LEDs
## #################################################

set constraintCode10  {create_clock -period 10.000 -name CLK_IN_D_clk_p -waveform {0.000 5.000} -add [get_ports CLK_IN_D_clk_p]}
set constraintCode11  {set_property PACKAGE_PIN AB8     [get_ports CLK_IN_D_clk_p]}
set constraintCode12  {set_property PACKAGE_PIN AR35    [get_ports user_link_up]}
set constraintCode13  {set_property IOSTANDARD LVCMOS18 [get_ports user_link_up]}

set constraintCode20  {set_property PACKAGE_PIN AM39    [get_ports init_calib_complete]}
set constraintCode21  {set_property IOSTANDARD LVCMOS18 [get_ports init_calib_complete]}
set constraintCode22  {set_property PACKAGE_PIN AR37    [get_ports mmcm_locked]}
set constraintCode23  {set_property IOSTANDARD LVCMOS18 [get_ports mmcm_locked]}

set constraintCode30  {set_property PACKAGE_PIN AN39    [get_ports c1_init_calib_complete]}
set constraintCode31  {set_property IOSTANDARD LVCMOS18 [get_ports c1_init_calib_complete]}
set constraintCode32  {set_property PACKAGE_PIN AT37    [get_ports c1_mmcm_locked]}
set constraintCode33  {set_property IOSTANDARD LVCMOS18 [get_ports c1_mmcm_locked]}

# create dirs
file mkdir $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1
file mkdir $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new

# create file
set fileID [ open $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc w ]

# put the contents in
puts $fileID $constraintCode10
puts $fileID $constraintCode11
puts $fileID $constraintCode12
puts $fileID $constraintCode13
puts $fileID $constraintCode20
puts $fileID $constraintCode21
puts $fileID $constraintCode22
puts $fileID $constraintCode23
puts $fileID $constraintCode30
puts $fileID $constraintCode31
puts $fileID $constraintCode32
puts $fileID $constraintCode33

# close file
close $fileID

# add it to constrints pool
add_files -fileset constrs_1                    $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc

# say "it's not used during synthesis"
set_property used_in_synthesis false [get_files $design_dir/$proj_name_long/$proj_name_short\.srcs/constrs_1/new/axi_pcie_mig_constraints.xdc]
