set libraryname 	AFAProcess
set vendor      	SystemberatungSchwarzer
set version     	0.5
set proj_dir		AFAProcessHW
set display_name	AFAProcessHW
set src_dir     	../src
# ============================================

open_project ../hls/$proj_dir

set all_solution [list VC709_32Bit_250Mhz                   ]
set all_part     [list xc7vx690tffg1761-2                   ]
set all_clocks   [list 4ns                                  ]
set all_axi_addr [list "-m_axi_offset off -register_io off" ]
set all_repo_dir [list "HWRepo/$proj_dir/VC709"          ]

#set all_solution [list AFAProcessing_64Bit_250MHz-2 AFAProcessing_32Bit_100MHz-1 ]
#set all_part     [list xc7vx690tffg1761-2           xc7a35ticsg324-1L            ]
#set all_clocks   [list 4ns                          10ns                         ]
#set all_axi_addr [list "-m_axi_addr64 -m_axi_offset off -register_io off"              " "                          ]

#set all_solution [list AFAProcessing_64Bit_250MHz-2 AFAProcessing_64Bit_400MHz-2 ]
#set all_part     [list xc7vx690tffg1761-2           xc7vx690tffg1761-2           ]
#set all_clocks   [list 4ns                          2.5ns                        ]

#set all_solution [list AFAProcessing_64Bit_250MHz-1 AFAProcessing_64Bit_250MHz-2 AFAProcessing_64Bit_333MHz-1 AFAProcessing_64Bit_333MHz-2 AFAProcessing_64Bit_400MHz-1 AFAProcessing_64Bit_400MHz-2]
#set all_part     [list xc7vx690tffg1761-1           xc7vx690tffg1761-2           xc7vx690tffg1761-1           xc7vx690tffg1761-2           xc7vx690tffg1761-1           xc7vx690tffg1761-2          ]
#set all_clocks   [list 4ns                          4ns                          3.333ns                      3.333ns                      2.5ns                        2.5ns                       ]

source _ProcessCoreHLS.tcl
file copy ../hls