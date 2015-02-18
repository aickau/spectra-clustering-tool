set moduleName AFAProcessFunction
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set C_modelName AFAProcessFunction
set C_modelType { void 0 }
set C_modelArgList { 
	{ p int 32 regular {axi_master 2}  }
	{ srcNet int 8 unused {axi_master 0}  }
	{ dstNet int 8 unused {axi_master 0}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "p", "interface" : "axi_master", "bitwidth" : 32 ,"direction" : "READWRITE" ,"bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "p","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 63,"step" : 1}]}]}]} , 
 	{ "Name" : "srcNet", "interface" : "axi_master", "bitwidth" : 8 ,"direction" : "READONLY" ,"bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "srcNet","cData": "void","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 255,"step" : 1}]}]}]} , 
 	{ "Name" : "dstNet", "interface" : "axi_master", "bitwidth" : 8 ,"direction" : "READONLY" ,"bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "dstNet","cData": "void","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 255,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 141
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_p_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_p_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_AWLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_p_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_p_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_p_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_p_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_p_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_WDATA sc_out sc_lv 32 signal 0 } 
	{ m_axi_p_WSTRB sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_p_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_ARLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_p_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_p_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_p_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_p_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_p_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_p_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_p_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_RDATA sc_in sc_lv 32 signal 0 } 
	{ m_axi_p_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_p_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_p_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_p_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_p_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_p_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_p_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_p_BUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_srcNet_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_srcNet_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_AWLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_srcNet_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_srcNet_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_srcNet_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_srcNet_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_srcNet_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_WDATA sc_out sc_lv 32 signal 1 } 
	{ m_axi_srcNet_WSTRB sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_srcNet_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_ARLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_srcNet_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_srcNet_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_srcNet_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_srcNet_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_srcNet_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_srcNet_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_srcNet_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_RDATA sc_in sc_lv 32 signal 1 } 
	{ m_axi_srcNet_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_srcNet_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_srcNet_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_srcNet_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_srcNet_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_srcNet_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_srcNet_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_srcNet_BUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_dstNet_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_dstNet_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_AWLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_dstNet_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_dstNet_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_dstNet_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_dstNet_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_dstNet_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_WDATA sc_out sc_lv 32 signal 2 } 
	{ m_axi_dstNet_WSTRB sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_dstNet_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_ARLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_dstNet_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_dstNet_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_dstNet_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_dstNet_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_dstNet_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_dstNet_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_dstNet_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_RDATA sc_in sc_lv 32 signal 2 } 
	{ m_axi_dstNet_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_dstNet_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_dstNet_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_dstNet_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_dstNet_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_dstNet_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_dstNet_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_dstNet_BUSER sc_in sc_lv 1 signal 2 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_p_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "AWVALID" }} , 
 	{ "name": "m_axi_p_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "AWREADY" }} , 
 	{ "name": "m_axi_p_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "p", "role": "AWADDR" }} , 
 	{ "name": "m_axi_p_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "AWID" }} , 
 	{ "name": "m_axi_p_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p", "role": "AWLEN" }} , 
 	{ "name": "m_axi_p_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "p", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_p_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "AWBURST" }} , 
 	{ "name": "m_axi_p_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_p_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_p_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "p", "role": "AWPROT" }} , 
 	{ "name": "m_axi_p_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "AWQOS" }} , 
 	{ "name": "m_axi_p_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "AWREGION" }} , 
 	{ "name": "m_axi_p_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "AWUSER" }} , 
 	{ "name": "m_axi_p_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "WVALID" }} , 
 	{ "name": "m_axi_p_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "WREADY" }} , 
 	{ "name": "m_axi_p_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "p", "role": "WDATA" }} , 
 	{ "name": "m_axi_p_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "WSTRB" }} , 
 	{ "name": "m_axi_p_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "WLAST" }} , 
 	{ "name": "m_axi_p_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "WID" }} , 
 	{ "name": "m_axi_p_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "WUSER" }} , 
 	{ "name": "m_axi_p_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "ARVALID" }} , 
 	{ "name": "m_axi_p_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "ARREADY" }} , 
 	{ "name": "m_axi_p_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "p", "role": "ARADDR" }} , 
 	{ "name": "m_axi_p_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "ARID" }} , 
 	{ "name": "m_axi_p_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p", "role": "ARLEN" }} , 
 	{ "name": "m_axi_p_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "p", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_p_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "ARBURST" }} , 
 	{ "name": "m_axi_p_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_p_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_p_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "p", "role": "ARPROT" }} , 
 	{ "name": "m_axi_p_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "ARQOS" }} , 
 	{ "name": "m_axi_p_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "p", "role": "ARREGION" }} , 
 	{ "name": "m_axi_p_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "ARUSER" }} , 
 	{ "name": "m_axi_p_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "RVALID" }} , 
 	{ "name": "m_axi_p_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "RREADY" }} , 
 	{ "name": "m_axi_p_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "p", "role": "RDATA" }} , 
 	{ "name": "m_axi_p_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "RLAST" }} , 
 	{ "name": "m_axi_p_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "RID" }} , 
 	{ "name": "m_axi_p_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "RUSER" }} , 
 	{ "name": "m_axi_p_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "RRESP" }} , 
 	{ "name": "m_axi_p_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "BVALID" }} , 
 	{ "name": "m_axi_p_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "BREADY" }} , 
 	{ "name": "m_axi_p_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "p", "role": "BRESP" }} , 
 	{ "name": "m_axi_p_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "BID" }} , 
 	{ "name": "m_axi_p_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "p", "role": "BUSER" }} , 
 	{ "name": "m_axi_srcNet_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "AWVALID" }} , 
 	{ "name": "m_axi_srcNet_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "AWREADY" }} , 
 	{ "name": "m_axi_srcNet_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "srcNet", "role": "AWADDR" }} , 
 	{ "name": "m_axi_srcNet_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "AWID" }} , 
 	{ "name": "m_axi_srcNet_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "srcNet", "role": "AWLEN" }} , 
 	{ "name": "m_axi_srcNet_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "srcNet", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_srcNet_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "AWBURST" }} , 
 	{ "name": "m_axi_srcNet_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_srcNet_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_srcNet_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "srcNet", "role": "AWPROT" }} , 
 	{ "name": "m_axi_srcNet_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "AWQOS" }} , 
 	{ "name": "m_axi_srcNet_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "AWREGION" }} , 
 	{ "name": "m_axi_srcNet_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "AWUSER" }} , 
 	{ "name": "m_axi_srcNet_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "WVALID" }} , 
 	{ "name": "m_axi_srcNet_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "WREADY" }} , 
 	{ "name": "m_axi_srcNet_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "srcNet", "role": "WDATA" }} , 
 	{ "name": "m_axi_srcNet_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "WSTRB" }} , 
 	{ "name": "m_axi_srcNet_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "WLAST" }} , 
 	{ "name": "m_axi_srcNet_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "WID" }} , 
 	{ "name": "m_axi_srcNet_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "WUSER" }} , 
 	{ "name": "m_axi_srcNet_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "ARVALID" }} , 
 	{ "name": "m_axi_srcNet_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "ARREADY" }} , 
 	{ "name": "m_axi_srcNet_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "srcNet", "role": "ARADDR" }} , 
 	{ "name": "m_axi_srcNet_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "ARID" }} , 
 	{ "name": "m_axi_srcNet_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "srcNet", "role": "ARLEN" }} , 
 	{ "name": "m_axi_srcNet_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "srcNet", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_srcNet_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "ARBURST" }} , 
 	{ "name": "m_axi_srcNet_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_srcNet_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_srcNet_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "srcNet", "role": "ARPROT" }} , 
 	{ "name": "m_axi_srcNet_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "ARQOS" }} , 
 	{ "name": "m_axi_srcNet_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "srcNet", "role": "ARREGION" }} , 
 	{ "name": "m_axi_srcNet_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "ARUSER" }} , 
 	{ "name": "m_axi_srcNet_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "RVALID" }} , 
 	{ "name": "m_axi_srcNet_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "RREADY" }} , 
 	{ "name": "m_axi_srcNet_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "srcNet", "role": "RDATA" }} , 
 	{ "name": "m_axi_srcNet_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "RLAST" }} , 
 	{ "name": "m_axi_srcNet_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "RID" }} , 
 	{ "name": "m_axi_srcNet_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "RUSER" }} , 
 	{ "name": "m_axi_srcNet_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "RRESP" }} , 
 	{ "name": "m_axi_srcNet_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "BVALID" }} , 
 	{ "name": "m_axi_srcNet_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "BREADY" }} , 
 	{ "name": "m_axi_srcNet_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "srcNet", "role": "BRESP" }} , 
 	{ "name": "m_axi_srcNet_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "BID" }} , 
 	{ "name": "m_axi_srcNet_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "srcNet", "role": "BUSER" }} , 
 	{ "name": "m_axi_dstNet_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "AWVALID" }} , 
 	{ "name": "m_axi_dstNet_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "AWREADY" }} , 
 	{ "name": "m_axi_dstNet_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dstNet", "role": "AWADDR" }} , 
 	{ "name": "m_axi_dstNet_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "AWID" }} , 
 	{ "name": "m_axi_dstNet_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "dstNet", "role": "AWLEN" }} , 
 	{ "name": "m_axi_dstNet_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dstNet", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_dstNet_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "AWBURST" }} , 
 	{ "name": "m_axi_dstNet_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_dstNet_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_dstNet_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dstNet", "role": "AWPROT" }} , 
 	{ "name": "m_axi_dstNet_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "AWQOS" }} , 
 	{ "name": "m_axi_dstNet_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "AWREGION" }} , 
 	{ "name": "m_axi_dstNet_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "AWUSER" }} , 
 	{ "name": "m_axi_dstNet_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "WVALID" }} , 
 	{ "name": "m_axi_dstNet_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "WREADY" }} , 
 	{ "name": "m_axi_dstNet_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dstNet", "role": "WDATA" }} , 
 	{ "name": "m_axi_dstNet_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "WSTRB" }} , 
 	{ "name": "m_axi_dstNet_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "WLAST" }} , 
 	{ "name": "m_axi_dstNet_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "WID" }} , 
 	{ "name": "m_axi_dstNet_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "WUSER" }} , 
 	{ "name": "m_axi_dstNet_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "ARVALID" }} , 
 	{ "name": "m_axi_dstNet_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "ARREADY" }} , 
 	{ "name": "m_axi_dstNet_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dstNet", "role": "ARADDR" }} , 
 	{ "name": "m_axi_dstNet_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "ARID" }} , 
 	{ "name": "m_axi_dstNet_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "dstNet", "role": "ARLEN" }} , 
 	{ "name": "m_axi_dstNet_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dstNet", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_dstNet_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "ARBURST" }} , 
 	{ "name": "m_axi_dstNet_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_dstNet_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_dstNet_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dstNet", "role": "ARPROT" }} , 
 	{ "name": "m_axi_dstNet_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "ARQOS" }} , 
 	{ "name": "m_axi_dstNet_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dstNet", "role": "ARREGION" }} , 
 	{ "name": "m_axi_dstNet_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "ARUSER" }} , 
 	{ "name": "m_axi_dstNet_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "RVALID" }} , 
 	{ "name": "m_axi_dstNet_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "RREADY" }} , 
 	{ "name": "m_axi_dstNet_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dstNet", "role": "RDATA" }} , 
 	{ "name": "m_axi_dstNet_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "RLAST" }} , 
 	{ "name": "m_axi_dstNet_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "RID" }} , 
 	{ "name": "m_axi_dstNet_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "RUSER" }} , 
 	{ "name": "m_axi_dstNet_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "RRESP" }} , 
 	{ "name": "m_axi_dstNet_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "BVALID" }} , 
 	{ "name": "m_axi_dstNet_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "BREADY" }} , 
 	{ "name": "m_axi_dstNet_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dstNet", "role": "BRESP" }} , 
 	{ "name": "m_axi_dstNet_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "BID" }} , 
 	{ "name": "m_axi_dstNet_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dstNet", "role": "BUSER" }}  ]}
set Spec2ImplPortList { 
	p { m_axi {  { m_axi_p_AWVALID VALID 1 1 }  { m_axi_p_AWREADY READY 0 1 }  { m_axi_p_AWADDR ADDR 1 32 }  { m_axi_p_AWID ID 1 1 }  { m_axi_p_AWLEN LEN 1 8 }  { m_axi_p_AWSIZE SIZE 1 3 }  { m_axi_p_AWBURST BURST 1 2 }  { m_axi_p_AWLOCK LOCK 1 2 }  { m_axi_p_AWCACHE CACHE 1 4 }  { m_axi_p_AWPROT PROT 1 3 }  { m_axi_p_AWQOS QOS 1 4 }  { m_axi_p_AWREGION REGION 1 4 }  { m_axi_p_AWUSER USER 1 1 }  { m_axi_p_WVALID VALID 1 1 }  { m_axi_p_WREADY READY 0 1 }  { m_axi_p_WDATA DATA 1 32 }  { m_axi_p_WSTRB STRB 1 4 }  { m_axi_p_WLAST LAST 1 1 }  { m_axi_p_WID ID 1 1 }  { m_axi_p_WUSER USER 1 1 }  { m_axi_p_ARVALID VALID 1 1 }  { m_axi_p_ARREADY READY 0 1 }  { m_axi_p_ARADDR ADDR 1 32 }  { m_axi_p_ARID ID 1 1 }  { m_axi_p_ARLEN LEN 1 8 }  { m_axi_p_ARSIZE SIZE 1 3 }  { m_axi_p_ARBURST BURST 1 2 }  { m_axi_p_ARLOCK LOCK 1 2 }  { m_axi_p_ARCACHE CACHE 1 4 }  { m_axi_p_ARPROT PROT 1 3 }  { m_axi_p_ARQOS QOS 1 4 }  { m_axi_p_ARREGION REGION 1 4 }  { m_axi_p_ARUSER USER 1 1 }  { m_axi_p_RVALID VALID 0 1 }  { m_axi_p_RREADY READY 1 1 }  { m_axi_p_RDATA DATA 0 32 }  { m_axi_p_RLAST LAST 0 1 }  { m_axi_p_RID ID 0 1 }  { m_axi_p_RUSER USER 0 1 }  { m_axi_p_RRESP RESP 0 2 }  { m_axi_p_BVALID VALID 0 1 }  { m_axi_p_BREADY READY 1 1 }  { m_axi_p_BRESP RESP 0 2 }  { m_axi_p_BID ID 0 1 }  { m_axi_p_BUSER USER 0 1 } } }
	srcNet { m_axi {  { m_axi_srcNet_AWVALID VALID 1 1 }  { m_axi_srcNet_AWREADY READY 0 1 }  { m_axi_srcNet_AWADDR ADDR 1 32 }  { m_axi_srcNet_AWID ID 1 1 }  { m_axi_srcNet_AWLEN LEN 1 8 }  { m_axi_srcNet_AWSIZE SIZE 1 3 }  { m_axi_srcNet_AWBURST BURST 1 2 }  { m_axi_srcNet_AWLOCK LOCK 1 2 }  { m_axi_srcNet_AWCACHE CACHE 1 4 }  { m_axi_srcNet_AWPROT PROT 1 3 }  { m_axi_srcNet_AWQOS QOS 1 4 }  { m_axi_srcNet_AWREGION REGION 1 4 }  { m_axi_srcNet_AWUSER USER 1 1 }  { m_axi_srcNet_WVALID VALID 1 1 }  { m_axi_srcNet_WREADY READY 0 1 }  { m_axi_srcNet_WDATA DATA 1 32 }  { m_axi_srcNet_WSTRB STRB 1 4 }  { m_axi_srcNet_WLAST LAST 1 1 }  { m_axi_srcNet_WID ID 1 1 }  { m_axi_srcNet_WUSER USER 1 1 }  { m_axi_srcNet_ARVALID VALID 1 1 }  { m_axi_srcNet_ARREADY READY 0 1 }  { m_axi_srcNet_ARADDR ADDR 1 32 }  { m_axi_srcNet_ARID ID 1 1 }  { m_axi_srcNet_ARLEN LEN 1 8 }  { m_axi_srcNet_ARSIZE SIZE 1 3 }  { m_axi_srcNet_ARBURST BURST 1 2 }  { m_axi_srcNet_ARLOCK LOCK 1 2 }  { m_axi_srcNet_ARCACHE CACHE 1 4 }  { m_axi_srcNet_ARPROT PROT 1 3 }  { m_axi_srcNet_ARQOS QOS 1 4 }  { m_axi_srcNet_ARREGION REGION 1 4 }  { m_axi_srcNet_ARUSER USER 1 1 }  { m_axi_srcNet_RVALID VALID 0 1 }  { m_axi_srcNet_RREADY READY 1 1 }  { m_axi_srcNet_RDATA DATA 0 32 }  { m_axi_srcNet_RLAST LAST 0 1 }  { m_axi_srcNet_RID ID 0 1 }  { m_axi_srcNet_RUSER USER 0 1 }  { m_axi_srcNet_RRESP RESP 0 2 }  { m_axi_srcNet_BVALID VALID 0 1 }  { m_axi_srcNet_BREADY READY 1 1 }  { m_axi_srcNet_BRESP RESP 0 2 }  { m_axi_srcNet_BID ID 0 1 }  { m_axi_srcNet_BUSER USER 0 1 } } }
	dstNet { m_axi {  { m_axi_dstNet_AWVALID VALID 1 1 }  { m_axi_dstNet_AWREADY READY 0 1 }  { m_axi_dstNet_AWADDR ADDR 1 32 }  { m_axi_dstNet_AWID ID 1 1 }  { m_axi_dstNet_AWLEN LEN 1 8 }  { m_axi_dstNet_AWSIZE SIZE 1 3 }  { m_axi_dstNet_AWBURST BURST 1 2 }  { m_axi_dstNet_AWLOCK LOCK 1 2 }  { m_axi_dstNet_AWCACHE CACHE 1 4 }  { m_axi_dstNet_AWPROT PROT 1 3 }  { m_axi_dstNet_AWQOS QOS 1 4 }  { m_axi_dstNet_AWREGION REGION 1 4 }  { m_axi_dstNet_AWUSER USER 1 1 }  { m_axi_dstNet_WVALID VALID 1 1 }  { m_axi_dstNet_WREADY READY 0 1 }  { m_axi_dstNet_WDATA DATA 1 32 }  { m_axi_dstNet_WSTRB STRB 1 4 }  { m_axi_dstNet_WLAST LAST 1 1 }  { m_axi_dstNet_WID ID 1 1 }  { m_axi_dstNet_WUSER USER 1 1 }  { m_axi_dstNet_ARVALID VALID 1 1 }  { m_axi_dstNet_ARREADY READY 0 1 }  { m_axi_dstNet_ARADDR ADDR 1 32 }  { m_axi_dstNet_ARID ID 1 1 }  { m_axi_dstNet_ARLEN LEN 1 8 }  { m_axi_dstNet_ARSIZE SIZE 1 3 }  { m_axi_dstNet_ARBURST BURST 1 2 }  { m_axi_dstNet_ARLOCK LOCK 1 2 }  { m_axi_dstNet_ARCACHE CACHE 1 4 }  { m_axi_dstNet_ARPROT PROT 1 3 }  { m_axi_dstNet_ARQOS QOS 1 4 }  { m_axi_dstNet_ARREGION REGION 1 4 }  { m_axi_dstNet_ARUSER USER 1 1 }  { m_axi_dstNet_RVALID VALID 0 1 }  { m_axi_dstNet_RREADY READY 1 1 }  { m_axi_dstNet_RDATA DATA 0 32 }  { m_axi_dstNet_RLAST LAST 0 1 }  { m_axi_dstNet_RID ID 0 1 }  { m_axi_dstNet_RUSER USER 0 1 }  { m_axi_dstNet_RRESP RESP 0 2 }  { m_axi_dstNet_BVALID VALID 0 1 }  { m_axi_dstNet_BREADY READY 1 1 }  { m_axi_dstNet_BRESP RESP 0 2 }  { m_axi_dstNet_BID ID 0 1 }  { m_axi_dstNet_BUSER USER 0 1 } } }
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
	{ p 5 }
	{ srcNet 1 }
	{ dstNet 1 }
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
	{ p 5 }
	{ srcNet 1 }
	{ dstNet 1 }
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
