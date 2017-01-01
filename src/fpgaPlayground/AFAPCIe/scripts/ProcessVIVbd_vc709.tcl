################################################################
# Check if script is running in correct Vivado version.
################################################################
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   puts "ERROR: This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."

   return 1
}

################################################################
# START
################################################################

# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}

set design_name $bd_design_name
create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

puts "INFO: design_name is \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}

## PCIe #######################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_pcie3:3.0 axi_pcie3_0
endgroup
startgroup
#set_property -dict [list CONFIG.pl_link_cap_max_link_width {X8} CONFIG.pl_link_cap_max_link_speed {8.0_GT/s} CONFIG.axi_addr_width {64} CONFIG.pf0_bar0_scale {Gigabytes} CONFIG.pf0_bar0_64bit {true} CONFIG.pf0_bar2_enabled {true} CONFIG.pf0_bar2_size {1} CONFIG.pf0_bar2_scale {Gigabytes} CONFIG.pf0_bar2_64bit {true} CONFIG.pf0_bar4_enabled {true} CONFIG.pf0_bar4_size {16} CONFIG.pf0_bar4_scale {Megabytes} CONFIG.pf0_bar4_64bit {true} CONFIG.pciebar2axibar_2 {0x0000000100000000} CONFIG.pciebar2axibar_4 {0x0000000F00000000} CONFIG.axi_data_width {256_bit} CONFIG.plltype {QPLL1} CONFIG.axisten_freq {250} CONFIG.pf0_device_id {7038} CONFIG.pf0_bar0_size {1} CONFIG.pf0_msix_cap_table_bir {BAR_1:0} CONFIG.pf0_msix_cap_pba_bir {BAR_1:0}] [get_bd_cells axi_pcie3_0]
set_property -dict [list CONFIG.pl_link_cap_max_link_width {X8} CONFIG.pl_link_cap_max_link_speed {8.0_GT/s} CONFIG.en_axi_slave_if {false} CONFIG.pf0_bar0_size {128} CONFIG.pf0_bar0_scale {Megabytes} CONFIG.pf0_bar1_enabled {true} CONFIG.pf0_bar1_size {128} CONFIG.pf0_bar1_scale {Megabytes} CONFIG.pciebar2axibar_1 {0x0000000080000000} CONFIG.PCIE_BOARD_INTERFACE {pci_express_x8} CONFIG.axi_data_width {256_bit} CONFIG.plltype {QPLL1} CONFIG.axisten_freq {250} CONFIG.pf0_device_id {7038}] [get_bd_cells axi_pcie3_0]

#force AXI interconnect to use 64-bit address width
set_property -dict [list CONFIG.axi_addr_width {64}] [get_bd_cells axi_pcie3_0]
endgroup

## DDR3 & Clock ###############################################################

#  startgroup
#  create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.0 mig_7series_0
#  apply_board_connection -board_interface "ddr3_sdram_socket_j1_j3" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
#  #create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.0 mig_7series_0
#  #apply_board_connection -board_interface "ddr3_sdram_socket_j1" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
#  endgroup
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.0 mig_7series_0
apply_board_connection -board_interface "ddr3_sdram_socket_j1_j3" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
endgroup
set_property -name {CONFIG.XML_INPUT_FILE} -value $proj_resources_dir/mig/vc709_2ports/mig_a.prj -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.RESET_BOARD_INTERFACE} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.MIG_DONT_TOUCH_PARAM} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.BOARD_MIG_PARAM} -value  {ddr3_sdram_socket_j1_j3} -objects [get_bd_cells mig_7series_0]
delete_bd_objs [get_bd_nets c1_sys_clk_p_1] [get_bd_nets c1_sys_clk_n_1] [get_bd_intf_nets sys_diff_clock_0_1]
delete_bd_objs [get_bd_intf_ports sys_diff_clock_0]
delete_bd_objs [get_bd_ports c1_sys_clk_p]
delete_bd_objs [get_bd_ports c1_sys_clk_n]
delete_bd_objs [get_bd_nets c0_sys_clk_n_1] [get_bd_ports c0_sys_clk_n]
delete_bd_objs [get_bd_nets c0_sys_clk_p_1] [get_bd_ports c0_sys_clk_p]
set_property -dict [list CONFIG.FREQ_HZ {200000000}] [get_bd_intf_ports sys_diff_clock]

save_bd_design

#connection automation
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "pci_express_x8 ( PCI Express ) " }  [get_bd_intf_pins axi_pcie3_0/pcie_7x_mgt]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "pcie_perstn ( PCI Express ) " }  [get_bd_pins axi_pcie3_0/sys_rst_n]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/axi_pcie3_0/M_AXI" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S0_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/axi_pcie3_0/M_AXI" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S1_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins mig_7series_0/sys_rst]
endgroup

save_bd_design

#refclock for pcie with utility buffer and constant interrupt
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.1 util_ds_buf_0
endgroup
#set_property location {1 147 -589} [get_bd_cells util_ds_buf_0]
startgroup
set_property -dict [list CONFIG.C_BUF_TYPE {IBUFDSGTE} CONFIG.DIFF_CLK_IN_BOARD_INTERFACE {pcie_refclk}] [get_bd_cells util_ds_buf_0]
endgroup
connect_bd_net [get_bd_pins util_ds_buf_0/IBUF_OUT] [get_bd_pins axi_pcie3_0/refclk]
startgroup
create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 CLK_IN_D
connect_bd_intf_net [get_bd_intf_pins util_ds_buf_0/CLK_IN_D] [get_bd_intf_ports CLK_IN_D]
endgroup

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0
endgroup
set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells xlconstant_0]
connect_bd_net [get_bd_pins xlconstant_0/dout] [get_bd_pins axi_pcie3_0/intx_msi_request]

save_bd_design
#  delete_bd_objs [get_bd_nets sys_clk_p_1] [get_bd_ports sys_clk_p]
#  delete_bd_objs [get_bd_nets sys_clk_n_1] [get_bd_ports sys_clk_n]
#  set_property -dict [list CONFIG.FREQ_HZ {200000000}] [get_bd_intf_ports sys_diff_clock]
#  
#set_property -name {CONFIG.XML_INPUT_FILE} -value  {mig_b.prj} -objects [get_bd_cells mig_7series_0]
#set_property -name {CONFIG.RESET_BOARD_INTERFACE} -value  {reset} -objects [get_bd_cells mig_7series_0]
#set_property -name {CONFIG.MIG_DONT_TOUCH_PARAM} -value  {Custom} -objects [get_bd_cells mig_7series_0]
#set_property -name {CONFIG.BOARD_MIG_PARAM} -value  {ddr3_sdram_socket_j1} -objects [get_bd_cells mig_7series_0]

startgroup
create_bd_port -dir O user_link_up
connect_bd_net [get_bd_pins /axi_pcie3_0/user_link_up] [get_bd_ports user_link_up]
endgroup
# MIG configured with 1 port:
# port c0
#  startgroup
#  create_bd_port -dir O init_calib_complete
#  connect_bd_net [get_bd_pins /mig_7series_0/init_calib_complete] [get_bd_ports init_calib_complete]
#  endgroup
#
#  startgroup
#  create_bd_port -dir O mmcm_locked
#  connect_bd_net [get_bd_pins /mig_7series_0/mmcm_locked] [get_bd_ports mmcm_locked]
#  endgroup

# MIG configured with 2 ports:
# port c0
startgroup
create_bd_port -dir O init_calib_complete
connect_bd_net [get_bd_pins /mig_7series_0/c0_init_calib_complete] [get_bd_ports init_calib_complete]
endgroup
startgroup
create_bd_port -dir O c1_init_calib_complete
connect_bd_net [get_bd_pins /mig_7series_0/c1_init_calib_complete] [get_bd_ports c1_init_calib_complete]
endgroup
startgroup
create_bd_port -dir O mmcm_locked
connect_bd_net [get_bd_pins /mig_7series_0/c0_mmcm_locked] [get_bd_ports mmcm_locked]
endgroup
startgroup
create_bd_port -dir O c1_mmcm_locked
connect_bd_net [get_bd_pins /mig_7series_0/c1_mmcm_locked] [get_bd_ports c1_mmcm_locked]
endgroup

## Beautify Layout ############################################################

regenerate_bd_layout -routing
regenerate_bd_layout

## Store design ###############################################################

save_bd_design

## Validate design ############################################################

validate_bd_design
return 0





















startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.0 mig_7series_0
apply_board_connection -board_interface "ddr3_sdram_socket_j1" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
endgroup

# important changes of the vc709 MIG7 V4.0 core:
# - C0: set "Data Width" to "256" Bit
puts "Importing XML definition of MIG7: $proj_resources_dir/mig/$platform_name/mig_a.prj"
set_property -name {CONFIG.XML_INPUT_FILE} -value $proj_resources_dir/mig/$platform_name/mig_a.prj -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.RESET_BOARD_INTERFACE} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.MIG_DONT_TOUCH_PARAM} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.BOARD_MIG_PARAM} -value  {ddr3_sdram_socket_j1} -objects [get_bd_cells mig_7series_0]

set_property -dict [list CONFIG.FREQ_HZ {200000000}] [get_bd_intf_ports sys_diff_clock]

save_bd_design

## sysclock ###################################################################

set_property -dict [list CONFIG.FREQ_HZ {200000000}] [get_bd_intf_ports sys_diff_clock]

## Connection automation ######################################################

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "pci_express_x8 ( PCI Express ) " }  [get_bd_intf_pins axi_pcie3_0/pcie_7x_mgt]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "pcie_perstn ( PCI Express ) " }  [get_bd_pins axi_pcie3_0/sys_rst_n]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/axi_pcie3_0/M_AXI" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins mig_7series_0/sys_rst]
endgroup

save_bd_design

## Additional connections for PCIe ############################################

## clock connect

connect_bd_net [get_bd_pins axi_pcie3_0/axi_aclk] [get_bd_pins axi_pcie3_0/axi_ctl_aclk]

## clock buffer

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.1 util_ds_buf_0
endgroup
set_property -dict [list CONFIG.C_BUF_TYPE {IBUFDSGTE}] [get_bd_cells util_ds_buf_0]
connect_bd_net [get_bd_pins util_ds_buf_0/IBUF_OUT] [get_bd_pins axi_pcie3_0/refclk]
startgroup
create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 CLK_IN_D
connect_bd_intf_net [get_bd_intf_pins util_ds_buf_0/CLK_IN_D] [get_bd_intf_ports CLK_IN_D]
endgroup

## constant

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0
endgroup
set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells xlconstant_0]
connect_bd_net [get_bd_pins xlconstant_0/dout] [get_bd_pins axi_pcie3_0/intx_msi_request]
save_bd_design

## debug info #################################################################

set_property HDL_ATTRIBUTE.MARK_DEBUG true [get_bd_intf_nets {axi_pcie3_0_M_AXI }]

## debug LEDs

startgroup
create_bd_port -dir O user_link_up
connect_bd_net [get_bd_pins /axi_pcie3_0/user_link_up] [get_bd_ports user_link_up]
endgroup
startgroup
create_bd_port -dir O init_calib_complete
connect_bd_net [get_bd_pins /mig_7series_0/init_calib_complete] [get_bd_ports init_calib_complete]
endgroup
startgroup
create_bd_port -dir O mmcm_locked
connect_bd_net [get_bd_pins /mig_7series_0/mmcm_locked] [get_bd_ports mmcm_locked]
endgroup

## Beautify Layout ############################################################

regenerate_bd_layout -routing
regenerate_bd_layout

## Store design ###############################################################

save_bd_design

## Validate design ############################################################

validate_bd_design

return 0
