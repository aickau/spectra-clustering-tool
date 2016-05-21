# Hardware entry function
set top_function	AFAProcess_HW
set_top $top_function

# Files to build the hardware
add_files     $cur_dir/$src_dir/AFADefines.h
add_files     $cur_dir/$src_dir/AFANetworkSettingsHW.h
add_files     $cur_dir/$src_dir/AFAProcessingHW.c
add_files     $cur_dir/$src_dir/AFAProcessingHW.h
add_files     $cur_dir/$src_dir/AFATypes.h

# Testbench files
add_files -tb $cur_dir/$src_dir/main.c
# -cflags "-D_FILE_OFFSET_BITS=64"
add_files -tb $cur_dir/$src_dir/AFADefines.h
add_files -tb $cur_dir/$src_dir/AFANetworkSettings.c
add_files -tb $cur_dir/$src_dir/AFANetworkSettings.h
add_files -tb $cur_dir/$src_dir/AFANetworkSettingsCommon.h
add_files -tb $cur_dir/$src_dir/AFAProcessing.c
add_files -tb $cur_dir/$src_dir/AFAProcessing.h
add_files -tb $cur_dir/$src_dir/AFAProcessingHWWrapper.c
add_files -tb $cur_dir/$src_dir/AFAProcessingCommon.h
add_files -tb $cur_dir/$src_dir/AFARandom.c
add_files -tb $cur_dir/$src_dir/AFARandom.h
add_files -tb $cur_dir/$src_dir/AFARandomCommon.h
add_files -tb $cur_dir/$src_dir/AFASpectra.c
add_files -tb $cur_dir/$src_dir/AFASpectra.h
add_files -tb $cur_dir/$src_dir/AFASpectraCommon.h
add_files -tb $cur_dir/$src_dir/AFATypes.c
add_files -tb $cur_dir/$src_dir/AFATypes.h
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra00.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra01.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra02.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra03.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra04.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra05.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra06.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra07.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra08.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra09.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra10.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra11.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra12.c
add_files -tb $cur_dir/$src_dir/AFATestDataSpectra13.c
add_files -tb $cur_dir/$src_dir/AFATestDataGoldenResult.c
