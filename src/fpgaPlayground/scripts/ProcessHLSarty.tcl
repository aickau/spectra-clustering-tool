## == Provide important informations ===========================
set libraryname 	AFAProcessingLib
set vendor      	SystemberatungSchwarzer
set versionH     	0
set versionL     	1001
set proj_name		AFAProcessHW
set display_name	AFAProcessHW
set platform_name	arty
set src_dir     	../src
set design_dir		"/AFA"
set repo_dir        "/AFA/repoHW"
## =============================================================

## == Prepare directory variables ==============================
set cur_dir [pwd]

set prj_dir_src_code "[file normalize "$cur_dir/$src_dir"]"
set prj_dir_scripts  "[file normalize "$cur_dir"]"
set prj_dir_design   "[file normalize "$design_dir"]"
set prj_dir_repo     "[file normalize "$repo_dir"]"
set prj_dir_hls      "[file normalize "$design_dir/hls"]"

puts "SourceCode\: $prj_dir_src_code"
puts "Scripts\:    $prj_dir_scripts"
puts "Design\:     $prj_dir_design"
puts "Repo\:       $prj_dir_repo"
puts "HLS\:        $prj_dir_hls"

## == Prepare project start ====================================

file mkdir $prj_dir_design
file mkdir $prj_dir_hls
cd $prj_dir_hls

## == Create project ===========================================

open_project $proj_name

## == Define variations for "HLS-Solutions" ====================

set all_solution [list ARTY_32Bit_100MHz                                       ]
set all_part     [list xc7a35ticsg324-1L                                       ]
set all_clocks   [list 3.333333ns                                              ]
set all_axi_addr [list "-m_axi_offset off -register_io off"                    ]
set all_repo_dir [list [file normalize "$repo_dir/$proj_name/$platform_name" ] ]

source $prj_dir_scripts/_ProcessHLSCore.tcl
