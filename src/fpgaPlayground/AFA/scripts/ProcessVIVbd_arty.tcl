################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2016.2
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

## sysclock ###################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.3 clk_wiz_0
apply_board_connection -board_interface "sys_clock" -ip_intf "clk_wiz_0/clock_CLK_IN1" -diagram $design_name
set_property -dict [list CONFIG.CLKOUT2_USED {true} CONFIG.CLKOUT3_USED {true} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {166.666666666} CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {200.000} CONFIG.CLKOUT3_REQUESTED_OUT_FREQ {25.000} CONFIG.RESET_TYPE {ACTIVE_LOW} CONFIG.MMCM_DIVCLK_DIVIDE {1} CONFIG.MMCM_CLKOUT0_DIVIDE_F {6.000} CONFIG.MMCM_CLKOUT1_DIVIDE {5} CONFIG.MMCM_CLKOUT2_DIVIDE {40} CONFIG.NUM_OUT_CLKS {3} CONFIG.RESET_PORT {resetn} CONFIG.CLKOUT1_JITTER {118.758} CONFIG.CLKOUT2_JITTER {114.829} CONFIG.CLKOUT2_PHASE_ERROR {98.575} CONFIG.CLKOUT3_JITTER {175.402} CONFIG.CLKOUT3_PHASE_ERROR {98.575}] [get_bd_cells clk_wiz_0]
endgroup

## DDR3 #######################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.0 mig_7series_0
apply_board_connection -board_interface "ddr3_sdram" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
endgroup
delete_bd_objs [get_bd_nets clk_ref_i_1] [get_bd_ports clk_ref_i]
delete_bd_objs [get_bd_nets sys_clk_i_1] [get_bd_ports sys_clk_i]
connect_bd_net [get_bd_pins clk_wiz_0/clk_out1] [get_bd_pins mig_7series_0/sys_clk_i]
connect_bd_net [get_bd_pins clk_wiz_0/clk_out2] [get_bd_pins mig_7series_0/clk_ref_i]

## reset ######################################################################

apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( System Reset ) " }  [get_bd_pins mig_7series_0/sys_rst]
connect_bd_net [get_bd_pins clk_wiz_0/resetn] [get_bd_pins mig_7series_0/sys_rst]

## Microblaze #################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.6 microblaze_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:microblaze -config {local_mem "4KB" ecc "None" cache "8KB" debug_module "Debug Only" axi_periph "Enabled" axi_intc "1" clk "/mig_7series_0/ui_clk (83 MHz)" }  [get_bd_cells microblaze_0]

## Peripheral - UART ##########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uartlite:2.0 axi_uartlite_0
apply_board_connection -board_interface "usb_uart" -ip_intf "axi_uartlite_0/UART" -diagram $design_name
endgroup
startgroup
set_property -dict [list CONFIG.C_BAUDRATE {115200}] [get_bd_cells axi_uartlite_0]
endgroup

## Peripheral - Ethernet ######################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_ethernetlite:3.0 axi_ethernetlite_0
apply_board_connection -board_interface "eth_mii" -ip_intf "axi_ethernetlite_0/MII" -diagram $design_name
apply_board_connection -board_interface "eth_mdio_mdc" -ip_intf "axi_ethernetlite_0/MDIO" -diagram $design_name
endgroup

## Peripheral - Timer #########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:2.0 axi_timer_0
endgroup

## Peripheral - LEDs ##########################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0
apply_board_connection -board_interface "led_4bits" -ip_intf "axi_gpio_0/GPIO" -diagram $design_name
endgroup
apply_board_connection -board_interface "rgb_led" -ip_intf "axi_gpio_0/GPIO2" -diagram $design_name

## Peripheral - Buttons and switches ##########################################

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

if { $CustomIPEnable == "yes" } {
	# store design up to this point
	save_bd_design

	# add custom design from repo
	startgroup
#		create_bd_cell -type ip -vlnv SystemberatungSchwarzer:AFAProcessingLib:AFAProcess_HW:0.1000 AFAProcess_HW_0
		create_bd_cell -type ip -vlnv $CustomIPNameFull $CustomIPName
	endgroup

	# connect it
	startgroup
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins $CustomIPName/s_axi_INTERFACE_AXILITE_SLAVE]
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Slave "/mig_7series_0/S_AXI" Clk "Auto" }  [get_bd_intf_pins $CustomIPName/m_axi_gmem]
		apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "New Blk_Mem_Gen" }  [get_bd_intf_pins $CustomIPName/param_PORTA]
	endgroup

	# add BRAM as needed by design
	startgroup
		set_property -dict [list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Enable_B {Use_ENB_Pin} CONFIG.Use_RSTB_Pin {true} CONFIG.Port_B_Clock {100} CONFIG.Port_B_Write_Rate {50} CONFIG.Port_B_Enable_Rate {100}] [get_bd_cells $CustomIPName\_bram]
	endgroup
	# Adding AXI-BRAM-Controller
	startgroup
		create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_bram_ctrl_0
	endgroup
	# Reducing AXI-BRAM-Controller to single port
	set_property -dict [list CONFIG.SINGLE_PORT_BRAM {1}] [get_bd_cells axi_bram_ctrl_0]

	# Connecting AXI-BRAM-Controller
	apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins axi_bram_ctrl_0/S_AXI]
	apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config "BRAM /$CustomIPName\_bram"  [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA]

	# IRQ concentrator extension from 2 to 3 ports
	startgroup
		set_property -dict [list CONFIG.NUM_PORTS {3}] [get_bd_cells microblaze_0_xlconcat]
	endgroup
	# connect IRQ out from design to IRQ concentrator
	connect_bd_net [get_bd_pins $CustomIPName/interrupt] [get_bd_pins microblaze_0_xlconcat/In2]
	
	# set address of module
	set_property offset 0x20000000 [get_bd_addr_segs "microblaze_0/Data/SEG_$CustomIPName\_Reg"]

	# store design up to this point
	save_bd_design
}

## Beautify Layout ############################################################

regenerate_bd_layout -routing
regenerate_bd_layout

## Validate design ############################################################

validate_bd_design

## Store design ###############################################################
save_bd_design

puts "Block design constructed and ready to use ..."
return 0
