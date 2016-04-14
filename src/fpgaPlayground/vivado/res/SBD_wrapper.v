//Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2015.4 (win64) Build 1412921 Wed Nov 18 09:43:45 MST 2015
//Date        : Sun Apr 10 02:06:49 2016
//Host        : Cetus running 64-bit major release  (build 9200)
//Command     : generate_target SBD_wrapper.bd
//Design      : SBD_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module SBD_wrapper
   (ddr3_sdram_socket_j1_addr,
    ddr3_sdram_socket_j1_ba,
    ddr3_sdram_socket_j1_cas_n,
    ddr3_sdram_socket_j1_ck_n,
    ddr3_sdram_socket_j1_ck_p,
    ddr3_sdram_socket_j1_cke,
    ddr3_sdram_socket_j1_cs_n,
    ddr3_sdram_socket_j1_dm,
    ddr3_sdram_socket_j1_dq,
    ddr3_sdram_socket_j1_dqs_n,
    ddr3_sdram_socket_j1_dqs_p,
    ddr3_sdram_socket_j1_odt,
    ddr3_sdram_socket_j1_ras_n,
    ddr3_sdram_socket_j1_reset_n,
    ddr3_sdram_socket_j1_we_n,
    led_8bits_tri_o,
    reset,
    rs232_uart_rxd,
    rs232_uart_txd,
    sys_diff_clock_clk_n,
    sys_diff_clock_clk_p);
  output [15:0]ddr3_sdram_socket_j1_addr;
  output [2:0]ddr3_sdram_socket_j1_ba;
  output ddr3_sdram_socket_j1_cas_n;
  output [0:0]ddr3_sdram_socket_j1_ck_n;
  output [0:0]ddr3_sdram_socket_j1_ck_p;
  output [0:0]ddr3_sdram_socket_j1_cke;
  output [0:0]ddr3_sdram_socket_j1_cs_n;
  output [7:0]ddr3_sdram_socket_j1_dm;
  inout [63:0]ddr3_sdram_socket_j1_dq;
  inout [7:0]ddr3_sdram_socket_j1_dqs_n;
  inout [7:0]ddr3_sdram_socket_j1_dqs_p;
  output [0:0]ddr3_sdram_socket_j1_odt;
  output ddr3_sdram_socket_j1_ras_n;
  output ddr3_sdram_socket_j1_reset_n;
  output ddr3_sdram_socket_j1_we_n;
  output [7:0]led_8bits_tri_o;
  input reset;
  input rs232_uart_rxd;
  output rs232_uart_txd;
  input sys_diff_clock_clk_n;
  input sys_diff_clock_clk_p;

  wire [15:0]ddr3_sdram_socket_j1_addr;
  wire [2:0]ddr3_sdram_socket_j1_ba;
  wire ddr3_sdram_socket_j1_cas_n;
  wire [0:0]ddr3_sdram_socket_j1_ck_n;
  wire [0:0]ddr3_sdram_socket_j1_ck_p;
  wire [0:0]ddr3_sdram_socket_j1_cke;
  wire [0:0]ddr3_sdram_socket_j1_cs_n;
  wire [7:0]ddr3_sdram_socket_j1_dm;
  wire [63:0]ddr3_sdram_socket_j1_dq;
  wire [7:0]ddr3_sdram_socket_j1_dqs_n;
  wire [7:0]ddr3_sdram_socket_j1_dqs_p;
  wire [0:0]ddr3_sdram_socket_j1_odt;
  wire ddr3_sdram_socket_j1_ras_n;
  wire ddr3_sdram_socket_j1_reset_n;
  wire ddr3_sdram_socket_j1_we_n;
  wire [7:0]led_8bits_tri_o;
  wire reset;
  wire rs232_uart_rxd;
  wire rs232_uart_txd;
  wire sys_diff_clock_clk_n;
  wire sys_diff_clock_clk_p;

  SBD SBD_i
       (.ddr3_sdram_socket_j1_addr(ddr3_sdram_socket_j1_addr),
        .ddr3_sdram_socket_j1_ba(ddr3_sdram_socket_j1_ba),
        .ddr3_sdram_socket_j1_cas_n(ddr3_sdram_socket_j1_cas_n),
        .ddr3_sdram_socket_j1_ck_n(ddr3_sdram_socket_j1_ck_n),
        .ddr3_sdram_socket_j1_ck_p(ddr3_sdram_socket_j1_ck_p),
        .ddr3_sdram_socket_j1_cke(ddr3_sdram_socket_j1_cke),
        .ddr3_sdram_socket_j1_cs_n(ddr3_sdram_socket_j1_cs_n),
        .ddr3_sdram_socket_j1_dm(ddr3_sdram_socket_j1_dm),
        .ddr3_sdram_socket_j1_dq(ddr3_sdram_socket_j1_dq),
        .ddr3_sdram_socket_j1_dqs_n(ddr3_sdram_socket_j1_dqs_n),
        .ddr3_sdram_socket_j1_dqs_p(ddr3_sdram_socket_j1_dqs_p),
        .ddr3_sdram_socket_j1_odt(ddr3_sdram_socket_j1_odt),
        .ddr3_sdram_socket_j1_ras_n(ddr3_sdram_socket_j1_ras_n),
        .ddr3_sdram_socket_j1_reset_n(ddr3_sdram_socket_j1_reset_n),
        .ddr3_sdram_socket_j1_we_n(ddr3_sdram_socket_j1_we_n),
        .led_8bits_tri_o(led_8bits_tri_o),
        .reset(reset),
        .rs232_uart_rxd(rs232_uart_rxd),
        .rs232_uart_txd(rs232_uart_txd),
        .sys_diff_clock_clk_n(sys_diff_clock_clk_n),
        .sys_diff_clock_clk_p(sys_diff_clock_clk_p));
endmodule
