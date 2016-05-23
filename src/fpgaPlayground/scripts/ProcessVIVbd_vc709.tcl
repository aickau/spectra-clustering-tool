################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2015.4
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

## DDR3 & Clock ###############################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:2.4 mig_7series_0
apply_board_connection -board_interface "ddr3_sdram_socket_j1_j3" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram "design_1" 
endgroup
delete_bd_objs [get_bd_intf_nets sys_diff_clock_0_1] [get_bd_intf_ports sys_diff_clock_0]
delete_bd_objs [get_bd_intf_nets sys_diff_clock_1]
delete_bd_objs [get_bd_intf_ports sys_diff_clock]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.2 clk_wiz_0
apply_board_connection -board_interface "sys_diff_clock" -ip_intf "clk_wiz_0/CLK_IN1_D" -diagram "design_1" 
endgroup
delete_bd_objs [get_bd_intf_nets sys_diff_clock_1] [get_bd_cells clk_wiz_0]
connect_bd_intf_net [get_bd_intf_ports sys_diff_clock] [get_bd_intf_pins mig_7series_0/C0_SYS_CLK]

## reset ######################################################################

apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins mig_7series_0/sys_rst]

## Microblaze #################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.5 microblaze_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:microblaze -config {local_mem "4KB" ecc "None" cache "8KB" debug_module "Debug Only" axi_periph "Enabled" axi_intc "1" clk "/mig_7series_0/c0_ui_addn_clk_0 (100 MHz)" }  [get_bd_cells microblaze_0]

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S0_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S1_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins rst_mig_7series_0_100M/ext_reset_in]
endgroup

## Peripheral - UART ##########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uartlite:2.0 axi_uartlite_0
apply_board_connection -board_interface "rs232_uart" -ip_intf "axi_uartlite_0/UART" -diagram $design_name
endgroup
startgroup
set_property -dict [list CONFIG.C_BAUDRATE {115200}] [get_bd_cells axi_uartlite_0]
endgroup

## Peripheral - Ethernet ######################################################

#startgroup
#create_bd_cell -type ip -vlnv xilinx.com:ip:axi_ethernetlite:3.0 axi_ethernetlite_0
#apply_board_connection -board_interface "eth_mii" -ip_intf "axi_ethernetlite_0/MII" -diagram $design_name
#apply_board_connection -board_interface "eth_mdio_mdc" -ip_intf "axi_ethernetlite_0/MDIO" -diagram $design_name
#endgroup

## Peripheral - Timer #########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_0
endgroup

## Peripheral - LEDs ##########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0
apply_board_connection -board_interface "led_8bits" -ip_intf "axi_gpio_0/GPIO" -diagram $design_name
endgroup

## Peripheral - Buttons and switches ##########################################



regenerate_bd_layout -routing
regenerate_bd_layout
save_bd_design
validate_bd_design
exit

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_1
apply_board_connection -board_interface "push_buttons_4bits" -ip_intf "axi_gpio_1/GPIO" -diagram $design_name
endgroup
apply_board_connection -board_interface "dip_switches_4bits" -ip_intf "axi_gpio_1/GPIO2" -diagram $design_name

## Peripheral - Connections ###################################################

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_uartlite_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_ethernetlite_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_timer_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_gpio_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_gpio_1/S_AXI]
endgroup

## Interrupt - Connections ####################################################

connect_bd_net [get_bd_pins axi_timer_0/interrupt] [get_bd_pins microblaze_0_xlconcat/In0]
connect_bd_net [get_bd_pins axi_ethernetlite_0/ip2intc_irpt] [get_bd_pins microblaze_0_xlconcat/In1]

## Ethernet reference clock ###################################################

create_bd_port -dir O -type clk eth_ref_clk
connect_bd_net [get_bd_ports eth_ref_clk] [get_bd_pins clk_wiz_0/clk_out3]

## Address map settings #######################################################

set_property offset 0x10000000 [get_bd_addr_segs {microblaze_0/Data/SEG_microblaze_0_axi_intc_Reg}]
set_property offset 0x11000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_timer_0_Reg}]
set_property offset 0x12000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_uartlite_0_Reg}]
set_property offset 0x13000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_ethernetlite_0_Reg}]
set_property offset 0x14000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_gpio_0_Reg}]
set_property offset 0x14100000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_gpio_1_Reg}]

## Insert custom IP ###########################################################

if { $enableCustomDesign == "yes" } {
	# add custom design from repo
	startgroup
		create_bd_cell -type ip -vlnv SystemberatungSchwarzer:AFAProcessingLib:AFAProcess_HW:0.611 AFAProcess_HW_0
	endgroup
	# connect it
	startgroup
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins AFAProcess_HW_0/s_axi_INTERFACE_AXILITE_SLAVE]
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Slave "/mig_7series_0/S_AXI" Clk "Auto" }  [get_bd_intf_pins AFAProcess_HW_0/m_axi_gmem]
		apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "New Blk_Mem_Gen" }  [get_bd_intf_pins AFAProcess_HW_0/param_PORTA]
	endgroup
	# add BRAM as needed by design
	startgroup
		set_property -dict [list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Enable_B {Use_ENB_Pin} CONFIG.Use_RSTB_Pin {true} CONFIG.Port_B_Clock {100} CONFIG.Port_B_Write_Rate {50} CONFIG.Port_B_Enable_Rate {100}] [get_bd_cells AFAProcess_HW_0_bram]
	endgroup
	# Adding AXI-BRAM-Controller
	startgroup
		create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_0
	endgroup
	# Reducing AXI-BRAM-Controller to single port
	set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_0]
	# Connecting AXI-BRAM-Controller
	startgroup
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins axi_bram_ctrl_0/S_AXI]
		apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "/AFAProcess_HW_0_bram" }  [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA]
	endgroup
	# IRQ concentrator extension from 2 to 3 ports
	startgroup
		set_property -dict [list CONFIG.NUM_PORTS {3}] [get_bd_cells microblaze_0_xlconcat]
	endgroup
	# connect IRQ out from design to IRQ concentrator
	connect_bd_net [get_bd_pins AFAProcess_HW_0/interrupt] [get_bd_pins microblaze_0_xlconcat/In2]
	# set address of BRAM
	set_property offset 0xC0000000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_axi_bram_ctrl_0_Mem0}]
	# set address of module
	set_property offset 0x20000000 [get_bd_addr_segs {microblaze_0/Data/SEG_AFAProcess_HW_0_Reg}]

## 100Mhz
#startgroup
#set_property -dict [list CONFIG.CLKOUT4_USED {true} CONFIG.MMCM_DIVCLK_DIVIDE {1} CONFIG.MMCM_CLKOUT3_DIVIDE {10} CONFIG.NUM_OUT_CLKS {4} CONFIG.CLKOUT4_JITTER {130.958} CONFIG.CLKOUT4_PHASE_ERROR {98.575}] [get_bd_cells clk_wiz_0]
#endgroup
#disconnect_bd_net /microblaze_0_Clk [get_bd_pins mig_7series_0/ui_clk]
#connect_bd_net [get_bd_pins clk_wiz_0/clk_out4] [get_bd_pins axi_mem_intercon/M01_ACLK]
#
#regenerate_bd_layout -routing
#regenerate_bd_layout
#save_bd_design
#validate_bd_design
#exit

}

## Beautify Layout ############################################################

regenerate_bd_layout -routing
regenerate_bd_layout

## Store design ###############################################################

save_bd_design

## Validate design ############################################################

validate_bd_design

return 0
