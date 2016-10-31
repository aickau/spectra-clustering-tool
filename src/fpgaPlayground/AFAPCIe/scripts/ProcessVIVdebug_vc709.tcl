create_debug_core u_ila_0 ila
set_property C_DATA_DEPTH 1024 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
create_debug_core u_ila_1 ila
set_property C_DATA_DEPTH 1024 [get_debug_cores u_ila_1]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_1]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_1]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_1]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_1]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_1]
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_1]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_1]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list axi_pcie_mig_i/axi_pcie3_0/inst/pcie3_ip_i/inst/gt_top_i/pipe_wrapper_i/pipe_clock_int.pipe_clock_i/CLK_RXUSRCLK ]]
set_property port_width 1 [get_debug_ports u_ila_1/clk]
connect_debug_port u_ila_1/clk [get_nets [list axi_pcie_mig_i/axi_pcie3_0/inst/pcie3_ip_i/inst/gt_top_i/pipe_wrapper_i/pipe_clock_int.pipe_clock_i/CLK_USERCLK2 ]]
set_property port_width 1 [get_debug_ports u_ila_0/probe0]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list axi_pcie_mig_i/axi_pcie3_0/inst/pcie3_ip_i/inst/store_ltssm ]]
set_property port_width 32 [get_debug_ports u_ila_1/probe0]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe0]
connect_debug_port u_ila_1/probe0 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARADDR[31]} ]]
create_debug_port u_ila_1 probe
set_property port_width 8 [get_debug_ports u_ila_1/probe1]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe1]
connect_debug_port u_ila_1/probe1 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARLEN[7]} ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe2]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe2]
connect_debug_port u_ila_1/probe2 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARPROT[1]} ]]
create_debug_port u_ila_1 probe
set_property port_width 3 [get_debug_ports u_ila_1/probe3]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe3]
connect_debug_port u_ila_1/probe3 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARSIZE[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARSIZE[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARSIZE[2]} ]]
create_debug_port u_ila_1 probe
set_property port_width 32 [get_debug_ports u_ila_1/probe4]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe4]
connect_debug_port u_ila_1/probe4 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWADDR[31]} ]]
create_debug_port u_ila_1 probe
set_property port_width 8 [get_debug_ports u_ila_1/probe5]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe5]
connect_debug_port u_ila_1/probe5 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWLEN[7]} ]]
create_debug_port u_ila_1 probe
set_property port_width 3 [get_debug_ports u_ila_1/probe6]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe6]
connect_debug_port u_ila_1/probe6 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWPROT[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWPROT[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWPROT[2]} ]]
create_debug_port u_ila_1 probe
set_property port_width 3 [get_debug_ports u_ila_1/probe7]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe7]
connect_debug_port u_ila_1/probe7 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWSIZE[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWSIZE[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWSIZE[2]} ]]
create_debug_port u_ila_1 probe
set_property port_width 3 [get_debug_ports u_ila_1/probe8]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe8]
connect_debug_port u_ila_1/probe8 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_BID[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_BID[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_BID[2]} ]]
create_debug_port u_ila_1 probe
set_property port_width 2 [get_debug_ports u_ila_1/probe9]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe9]
connect_debug_port u_ila_1/probe9 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_BRESP[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_BRESP[1]} ]]
create_debug_port u_ila_1 probe
set_property port_width 256 [get_debug_ports u_ila_1/probe10]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe10]
connect_debug_port u_ila_1/probe10 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[31]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[32]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[33]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[34]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[35]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[36]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[37]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[38]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[39]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[40]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[41]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[42]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[43]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[44]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[45]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[46]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[47]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[48]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[49]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[50]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[51]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[52]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[53]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[54]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[55]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[56]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[57]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[58]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[59]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[60]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[61]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[62]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[63]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[64]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[65]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[66]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[67]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[68]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[69]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[70]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[71]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[72]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[73]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[74]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[75]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[76]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[77]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[78]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[79]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[80]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[81]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[82]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[83]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[84]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[85]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[86]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[87]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[88]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[89]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[90]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[91]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[92]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[93]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[94]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[95]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[96]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[97]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[98]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[99]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[100]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[101]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[102]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[103]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[104]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[105]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[106]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[107]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[108]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[109]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[110]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[111]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[112]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[113]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[114]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[115]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[116]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[117]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[118]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[119]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[120]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[121]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[122]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[123]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[124]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[125]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[126]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[127]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[128]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[129]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[130]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[131]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[132]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[133]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[134]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[135]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[136]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[137]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[138]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[139]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[140]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[141]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[142]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[143]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[144]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[145]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[146]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[147]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[148]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[149]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[150]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[151]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[152]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[153]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[154]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[155]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[156]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[157]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[158]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[159]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[160]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[161]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[162]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[163]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[164]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[165]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[166]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[167]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[168]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[169]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[170]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[171]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[172]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[173]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[174]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[175]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[176]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[177]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RDATA[178]} {axi_pcie_mig_...
create_debug_port u_ila_1 probe
set_property port_width 3 [get_debug_ports u_ila_1/probe11]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe11]
connect_debug_port u_ila_1/probe11 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RID[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RID[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RID[2]} ]]
create_debug_port u_ila_1 probe
set_property port_width 2 [get_debug_ports u_ila_1/probe12]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe12]
connect_debug_port u_ila_1/probe12 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RRESP[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RRESP[1]} ]]
create_debug_port u_ila_1 probe
set_property port_width 32 [get_debug_ports u_ila_1/probe13]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe13]
connect_debug_port u_ila_1/probe13 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_RUSER[31]} ]]
create_debug_port u_ila_1 probe
set_property port_width 256 [get_debug_ports u_ila_1/probe14]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe14]
connect_debug_port u_ila_1/probe14 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[31]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[32]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[33]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[34]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[35]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[36]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[37]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[38]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[39]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[40]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[41]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[42]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[43]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[44]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[45]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[46]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[47]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[48]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[49]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[50]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[51]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[52]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[53]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[54]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[55]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[56]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[57]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[58]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[59]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[60]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[61]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[62]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[63]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[64]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[65]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[66]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[67]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[68]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[69]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[70]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[71]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[72]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[73]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[74]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[75]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[76]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[77]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[78]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[79]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[80]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[81]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[82]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[83]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[84]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[85]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[86]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[87]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[88]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[89]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[90]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[91]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[92]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[93]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[94]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[95]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[96]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[97]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[98]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[99]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[100]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[101]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[102]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[103]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[104]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[105]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[106]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[107]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[108]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[109]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[110]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[111]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[112]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[113]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[114]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[115]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[116]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[117]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[118]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[119]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[120]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[121]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[122]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[123]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[124]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[125]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[126]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[127]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[128]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[129]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[130]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[131]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[132]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[133]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[134]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[135]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[136]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[137]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[138]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[139]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[140]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[141]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[142]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[143]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[144]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[145]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[146]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[147]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[148]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[149]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[150]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[151]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[152]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[153]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[154]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[155]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[156]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[157]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[158]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[159]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[160]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[161]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[162]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[163]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[164]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[165]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[166]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[167]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[168]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[169]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[170]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[171]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[172]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[173]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[174]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[175]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[176]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[177]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WDATA[178]} {axi_pcie_mig_...
create_debug_port u_ila_1 probe
set_property port_width 32 [get_debug_ports u_ila_1/probe15]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe15]
connect_debug_port u_ila_1/probe15 [get_nets [list {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[0]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[1]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[2]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[3]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[4]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[5]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[6]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[7]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[8]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[9]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[10]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[11]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[12]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[13]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[14]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[15]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[16]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[17]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[18]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[19]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[20]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[21]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[22]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[23]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[24]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[25]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[26]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[27]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[28]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[29]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[30]} {axi_pcie_mig_i/axi_pcie3_0_M_AXI_WSTRB[31]} ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe16]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe16]
connect_debug_port u_ila_1/probe16 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARREADY ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe17]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe17]
connect_debug_port u_ila_1/probe17 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_ARVALID ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe18]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe18]
connect_debug_port u_ila_1/probe18 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWREADY ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe19]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe19]
connect_debug_port u_ila_1/probe19 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_AWVALID ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe20]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe20]
connect_debug_port u_ila_1/probe20 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_BREADY ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe21]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe21]
connect_debug_port u_ila_1/probe21 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_BVALID ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe22]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe22]
connect_debug_port u_ila_1/probe22 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_RLAST ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe23]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe23]
connect_debug_port u_ila_1/probe23 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_RREADY ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe24]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe24]
connect_debug_port u_ila_1/probe24 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_RVALID ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe25]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe25]
connect_debug_port u_ila_1/probe25 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_WLAST ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe26]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe26]
connect_debug_port u_ila_1/probe26 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_WREADY ]]
create_debug_port u_ila_1 probe
set_property port_width 1 [get_debug_ports u_ila_1/probe27]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_1/probe27]
connect_debug_port u_ila_1/probe27 [get_nets [list axi_pcie_mig_i/axi_pcie3_0_M_AXI_WVALID ]]
