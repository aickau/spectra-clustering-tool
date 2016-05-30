################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2016.1
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
update_compile_order -fileset sources_1

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
create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:3.0 mig_7series_0
apply_board_connection -board_interface "ddr3_sdram_socket_j1_j3" -ip_intf "mig_7series_0/mig_ddr_interface" -diagram $design_name
endgroup

# important changes of the vc709 MIG7 V3.0 core:
# - C0: set "Data Width" to "128" Bit
# - C1: set "Clock Period" to "1250ps"
# - C1: set "Data Width" to "128" Bit
# - C1: set "Iput Clock Period" to "5000 ps (200 Mhz)"
# - System Signals Selection: "c1_sys_clk_p/n" to Bank Number "38" to Pin Number "H19/G18(CC_P/N)"
puts "Importing XML definition of MIG7: $proj_resources_dir/mig/$proj_name/mig_a.prj"
set_property -name {CONFIG.XML_INPUT_FILE} -value $proj_resources_dir/mig/$proj_name/mig_a.prj -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.RESET_BOARD_INTERFACE} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.MIG_DONT_TOUCH_PARAM} -value  {Custom} -objects [get_bd_cells mig_7series_0]
set_property -name {CONFIG.BOARD_MIG_PARAM} -value  {ddr3_sdram_socket_j1_j3} -objects [get_bd_cells mig_7series_0]
delete_bd_objs [get_bd_intf_nets sys_diff_clock_0_1]
delete_bd_objs [get_bd_intf_ports sys_diff_clock_0]
save_bd_design

## reset ######################################################################

apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins mig_7series_0/sys_rst]

## Microblaze #################################################################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.6 microblaze_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:microblaze -config {local_mem "4KB" ecc "None" cache "8KB" debug_module "Debug Only" axi_periph "Enabled" axi_intc "1" clk "/mig_7series_0/c0_ui_addn_clk_0 (100 MHz)" }  [get_bd_cells microblaze_0]

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S0_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "Auto" }  [get_bd_intf_pins mig_7series_0/S1_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset ( FPGA Reset ) " }  [get_bd_pins rst_mig_7series_0_100M/ext_reset_in]
endgroup

save_bd_design

# trick - make the range small - so we can move it without collisions everywhere
set_property range 1M [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c0_memaddr}]
set_property range 1M [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c0_memaddr}]
set_property range 1M [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c1_memaddr}]
set_property range 1M [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr}]
set_property offset 0x70000000 [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c0_memaddr}]
set_property offset 0x70000000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c0_memaddr}]
set_property offset 0x70100000 [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c1_memaddr}]
set_property offset 0x70100000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr}]

# set interrupt controller to a nice address
set_property offset 0x00100000 [get_bd_addr_segs {microblaze_0/Data/SEG_microblaze_0_axi_intc_Reg}]
save_bd_design

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

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_1
apply_board_connection -board_interface "push_buttons_5bits" -ip_intf "axi_gpio_1/GPIO" -diagram $design_name 
endgroup
apply_board_connection -board_interface "dip_switches_8bits" -ip_intf "axi_gpio_1/GPIO2" -diagram $design_name 

save_bd_design

## Peripheral - Connections ###################################################

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_uartlite_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_timer_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_gpio_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_gpio_1/S_AXI]
endgroup
save_bd_design

## Interrupt - Connections ####################################################

connect_bd_net [get_bd_pins axi_timer_0/interrupt] [get_bd_pins microblaze_0_xlconcat/In0]
#connect_bd_net [get_bd_pins axi_ethernetlite_0/ip2intc_irpt] [get_bd_pins microblaze_0_xlconcat/In1]

## Ethernet reference clock ###################################################

#create_bd_port -dir O -type clk eth_ref_clk
#connect_bd_net [get_bd_ports eth_ref_clk] [get_bd_pins clk_wiz_0/clk_out3]

## Insert custom IP ###########################################################

if { $enableCustomDesign == "yes" } {
	# add custom design from repo
	startgroup
		create_bd_cell -type ip -vlnv SystemberatungSchwarzer:AFAProcessingLib:AFAProcess_HW:0.1000 AFAProcess_HW_0
	endgroup

	# connect it
	startgroup
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins AFAProcess_HW_0/s_axi_INTERFACE_AXILITE_SLAVE]
		apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Slave "/mig_7series_0/S1_AXI" Clk "Auto" }  [get_bd_intf_pins AFAProcess_HW_0/m_axi_gmem]
		apply_bd_automation -rule xilinx.com:bd_rule:bram_cntlr -config {BRAM "New Blk_Mem_Gen" }  [get_bd_intf_pins AFAProcess_HW_0/param_PORTA]
	endgroup

	# enable 2nd port on BRAM for design
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
#	# IRQ concentrator extension from 2 to 3 ports
#	startgroup
#		set_property -dict [list CONFIG.NUM_PORTS {3}] [get_bd_cells microblaze_0_xlconcat]
#	endgroup

	# connect IRQ out from design to IRQ concentrator
	connect_bd_net [get_bd_pins AFAProcess_HW_0/interrupt] [get_bd_pins microblaze_0_xlconcat/In1]
	puts "jesko7"
	save_bd_design

	# set address of BRAM-module
	set_property offset 0x70300000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_axi_bram_ctrl_0_Mem0}]
	set_property offset 0x70300000 [get_bd_addr_segs {microblaze_0/Data/SEG_AFAProcess_HW_0_Reg}]
	puts "jesko8"
	save_bd_design

	# memory location of register set of HW block
	set_property offset 0x10000000 [get_bd_addr_segs {microblaze_0/Data/SEG_AFAProcess_HW_0_Reg}]
	
	# memory location (only temporary) of param-block (bram block)
	set_property offset 0x10010000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_bram_ctrl_0_Mem0}]
	set_property offset 0x10010000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_axi_bram_ctrl_0_Mem0}]
	set_property offset 0x10010000 [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_axi_bram_ctrl_0_Mem0}]
	puts "jesko9"
	save_bd_design
} else {
## Interrupt - Defined state on second input ##################################

startgroup
	create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0
endgroup
set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells xlconstant_0]
connect_bd_net [get_bd_pins xlconstant_0/dout] [get_bd_pins microblaze_0_xlconcat/In1]

}

## Address map settings #######################################################

puts "jesko0"
save_bd_design
set_property offset 0x00200000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_uartlite_0_Reg}]
set_property offset 0x00300000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_timer_0_Reg}]
set_property offset 0x01000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_gpio_0_Reg}]
set_property offset 0x01100000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_gpio_1_Reg}]
puts "jesko1"
save_bd_design

## set the memory blocks to its final positions

# exclude 2nd memory module from Microblaze and AFAProcess_HW access completly
exclude_bd_addr_seg [get_bd_addr_segs microblaze_0/Data/SEG_mig_7series_0_c1_memaddr]
exclude_bd_addr_seg [get_bd_addr_segs microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr]
exclude_bd_addr_seg [get_bd_addr_segs AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c1_memaddr]
#delete_bd_objs [get_bd_addr_segs -excluded microblaze_0/Data/SEG_mig_7series_0_c1_memaddr]
#delete_bd_objs [get_bd_addr_segs -excluded microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr]
#delete_bd_objs [get_bd_addr_segs -excluded AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c1_memaddr]
#exclude_bd_addr_seg [get_bd_addr_segs mig_7series_0/c1_memmap/c1_memaddr] -target_address_space [get_bd_addr_spaces microblaze_0/Data]
#exclude_bd_addr_seg [get_bd_addr_segs mig_7series_0/c1_memmap/c1_memaddr] -target_address_space [get_bd_addr_spaces microblaze_0/Instruction]
#exclude_bd_addr_seg [get_bd_addr_segs mig_7series_0/c1_memmap/c1_memaddr] -target_address_space [get_bd_addr_spaces AFAProcess_HW_0/Data_m_axi_gmem]
puts "jesko1a"
save_bd_design

# put 1st memory module in place
set_property offset 0x80000000 [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c0_memaddr}]
set_property offset 0x80000000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c0_memaddr}]
set_property range 1G [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c0_memaddr}]
set_property range 1G [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c0_memaddr}]
#set_property offset 0xc0000000 [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c1_memaddr}]
#set_property offset 0xc0000000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr}]
#set_property range 1G [get_bd_addr_segs {microblaze_0/Instruction/SEG_mig_7series_0_c1_memaddr}]
#set_property range 1G [get_bd_addr_segs {microblaze_0/Data/SEG_mig_7series_0_c1_memaddr}]

## Set the BRAM controller address 
set_property offset 0xC0000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_bram_ctrl_0_Mem0}]
set_property offset 0xC0000000 [get_bd_addr_segs {microblaze_0/Instruction/SEG_axi_bram_ctrl_0_Mem0}]
set_property offset 0xC0000000 [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_axi_bram_ctrl_0_Mem0}]

puts "jesko2"

## Set the Microblaze cache access regions (all regions that must be accessed by memory operations)

set_property offset 0x80000000 [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c0_memaddr}]
set_property range 1G [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c0_memaddr}]
#set_property offset 0xC0000000 [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c1_memaddr}]
#set_property range 1G [get_bd_addr_segs {AFAProcess_HW_0/Data_m_axi_gmem/SEG_mig_7series_0_c1_memaddr}]
puts "jesko3"
save_bd_design

## Microblaze - Configure 2G (Memory-)Addresses are accessible ################

startgroup
set_property -dict [list CONFIG.C_ICACHE_BASEADDR.VALUE_SRC USER CONFIG.C_ICACHE_HIGHADDR.VALUE_SRC USER CONFIG.C_DCACHE_BASEADDR.VALUE_SRC USER CONFIG.C_DCACHE_HIGHADDR.VALUE_SRC USER] [get_bd_cells microblaze_0]
set_property -dict [list CONFIG.C_ICACHE_BASEADDR {0x80000000} CONFIG.C_ICACHE_HIGHADDR {0xfFFFFFFF} CONFIG.C_DCACHE_BASEADDR {0x0000000080000000} CONFIG.C_DCACHE_HIGHADDR {0x00000000fFFFFFFF}] [get_bd_cells microblaze_0]
endgroup

#startgroup
#set_property -dict [list CONFIG.C_ICACHE_BASEADDR.VALUE_SRC USER CONFIG.C_ICACHE_HIGHADDR.VALUE_SRC USER CONFIG.C_DCACHE_BASEADDR.VALUE_SRC USER] [get_bd_cells microblaze_0]
#set_property -dict [list CONFIG.C_ICACHE_BASEADDR {0x80000000} CONFIG.C_DCACHE_BASEADDR {0x0000000080000000}] [get_bd_cells microblaze_0]
#set_property -dict [list CONFIG.C_ICACHE_HIGHADDR {0xFFFFFFFF}] [get_bd_cells microblaze_0]
#endgroup

## Beautify Layout ############################################################

regenerate_bd_layout -routing
regenerate_bd_layout

## Store design ###############################################################

save_bd_design

## Validate design ############################################################

validate_bd_design

return 0
