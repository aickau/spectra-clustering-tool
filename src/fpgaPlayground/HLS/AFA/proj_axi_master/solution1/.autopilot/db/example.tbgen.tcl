set moduleName example
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set C_modelName example
set C_modelType { void 0 }
set C_modelArgList { 
	{ a int 32 regular {axi_master 2}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "a", "interface" : "axi_master", "bitwidth" : 32 ,"direction" : "READWRITE" ,"bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "a","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 49,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 51
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_a_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_a_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_AWLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_a_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_a_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_a_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_a_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_a_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_WDATA sc_out sc_lv 32 signal 0 } 
	{ m_axi_a_WSTRB sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_a_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_ARLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_a_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_a_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_a_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_a_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_a_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_a_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_a_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_RDATA sc_in sc_lv 32 signal 0 } 
	{ m_axi_a_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_a_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_a_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_a_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_a_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_a_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_a_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_a_BUSER sc_in sc_lv 1 signal 0 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_a_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "AWVALID" }} , 
 	{ "name": "m_axi_a_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "AWREADY" }} , 
 	{ "name": "m_axi_a_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "AWADDR" }} , 
 	{ "name": "m_axi_a_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "AWID" }} , 
 	{ "name": "m_axi_a_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "a", "role": "AWLEN" }} , 
 	{ "name": "m_axi_a_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "a", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_a_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "AWBURST" }} , 
 	{ "name": "m_axi_a_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_a_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_a_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "a", "role": "AWPROT" }} , 
 	{ "name": "m_axi_a_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "AWQOS" }} , 
 	{ "name": "m_axi_a_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "AWREGION" }} , 
 	{ "name": "m_axi_a_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "AWUSER" }} , 
 	{ "name": "m_axi_a_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "WVALID" }} , 
 	{ "name": "m_axi_a_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "WREADY" }} , 
 	{ "name": "m_axi_a_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "WDATA" }} , 
 	{ "name": "m_axi_a_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "WSTRB" }} , 
 	{ "name": "m_axi_a_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "WLAST" }} , 
 	{ "name": "m_axi_a_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "WID" }} , 
 	{ "name": "m_axi_a_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "WUSER" }} , 
 	{ "name": "m_axi_a_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ARVALID" }} , 
 	{ "name": "m_axi_a_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ARREADY" }} , 
 	{ "name": "m_axi_a_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "ARADDR" }} , 
 	{ "name": "m_axi_a_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ARID" }} , 
 	{ "name": "m_axi_a_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "a", "role": "ARLEN" }} , 
 	{ "name": "m_axi_a_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "a", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_a_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "ARBURST" }} , 
 	{ "name": "m_axi_a_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_a_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_a_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "a", "role": "ARPROT" }} , 
 	{ "name": "m_axi_a_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "ARQOS" }} , 
 	{ "name": "m_axi_a_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "a", "role": "ARREGION" }} , 
 	{ "name": "m_axi_a_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ARUSER" }} , 
 	{ "name": "m_axi_a_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "RVALID" }} , 
 	{ "name": "m_axi_a_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "RREADY" }} , 
 	{ "name": "m_axi_a_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "RDATA" }} , 
 	{ "name": "m_axi_a_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "RLAST" }} , 
 	{ "name": "m_axi_a_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "RID" }} , 
 	{ "name": "m_axi_a_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "RUSER" }} , 
 	{ "name": "m_axi_a_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "RRESP" }} , 
 	{ "name": "m_axi_a_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "BVALID" }} , 
 	{ "name": "m_axi_a_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "BREADY" }} , 
 	{ "name": "m_axi_a_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "a", "role": "BRESP" }} , 
 	{ "name": "m_axi_a_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "BID" }} , 
 	{ "name": "m_axi_a_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "BUSER" }}  ]}
set Spec2ImplPortList { 
	a { m_axi {  { m_axi_a_AWVALID VALID 1 1 }  { m_axi_a_AWREADY READY 0 1 }  { m_axi_a_AWADDR ADDR 1 32 }  { m_axi_a_AWID ID 1 1 }  { m_axi_a_AWLEN LEN 1 8 }  { m_axi_a_AWSIZE SIZE 1 3 }  { m_axi_a_AWBURST BURST 1 2 }  { m_axi_a_AWLOCK LOCK 1 2 }  { m_axi_a_AWCACHE CACHE 1 4 }  { m_axi_a_AWPROT PROT 1 3 }  { m_axi_a_AWQOS QOS 1 4 }  { m_axi_a_AWREGION REGION 1 4 }  { m_axi_a_AWUSER USER 1 1 }  { m_axi_a_WVALID VALID 1 1 }  { m_axi_a_WREADY READY 0 1 }  { m_axi_a_WDATA DATA 1 32 }  { m_axi_a_WSTRB STRB 1 4 }  { m_axi_a_WLAST LAST 1 1 }  { m_axi_a_WID ID 1 1 }  { m_axi_a_WUSER USER 1 1 }  { m_axi_a_ARVALID VALID 1 1 }  { m_axi_a_ARREADY READY 0 1 }  { m_axi_a_ARADDR ADDR 1 32 }  { m_axi_a_ARID ID 1 1 }  { m_axi_a_ARLEN LEN 1 8 }  { m_axi_a_ARSIZE SIZE 1 3 }  { m_axi_a_ARBURST BURST 1 2 }  { m_axi_a_ARLOCK LOCK 1 2 }  { m_axi_a_ARCACHE CACHE 1 4 }  { m_axi_a_ARPROT PROT 1 3 }  { m_axi_a_ARQOS QOS 1 4 }  { m_axi_a_ARREGION REGION 1 4 }  { m_axi_a_ARUSER USER 1 1 }  { m_axi_a_RVALID VALID 0 1 }  { m_axi_a_RREADY READY 1 1 }  { m_axi_a_RDATA DATA 0 32 }  { m_axi_a_RLAST LAST 0 1 }  { m_axi_a_RID ID 0 1 }  { m_axi_a_RUSER USER 0 1 }  { m_axi_a_RRESP RESP 0 2 }  { m_axi_a_BVALID VALID 0 1 }  { m_axi_a_BREADY READY 1 1 }  { m_axi_a_BRESP RESP 0 2 }  { m_axi_a_BID ID 0 1 }  { m_axi_a_BUSER USER 0 1 } } }
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
	{ a 5 }
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
	{ a 5 }
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
