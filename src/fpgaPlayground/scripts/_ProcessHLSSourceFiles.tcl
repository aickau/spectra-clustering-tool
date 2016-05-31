# Hardware entry function
set top_function	AFAProcess_HW
set_top $top_function

# Files to build the hardware
add_files     $prj_dir_src_code/AFADefines.h
add_files     $prj_dir_src_code/AFANetworkSettingsHW.h
add_files     $prj_dir_src_code/AFAProcessingHW.c
add_files     $prj_dir_src_code/AFAProcessingHW.h
add_files     $prj_dir_src_code/AFATypes.h

# Testbench files
add_files -tb $prj_dir_src_code/main.c
# -cflags "-D_FILE_OFFSET_BITS=64"
add_files -tb $prj_dir_src_code/AFAConfig.h
add_files -tb $prj_dir_src_code/AFADefines.h
add_files -tb $prj_dir_src_code/AFANetworkSettings.c
add_files -tb $prj_dir_src_code/AFANetworkSettings.h
add_files -tb $prj_dir_src_code/AFANetworkSettingsCommon.h
add_files -tb $prj_dir_src_code/AFAProcessing.c
add_files -tb $prj_dir_src_code/AFAProcessing.h
add_files -tb $prj_dir_src_code/AFAProcessingHWWrapper.c
add_files -tb $prj_dir_src_code/AFAProcessingCommon.h
add_files -tb $prj_dir_src_code/AFARandom.c
add_files -tb $prj_dir_src_code/AFARandom.h
add_files -tb $prj_dir_src_code/AFARandomCommon.h
add_files -tb $prj_dir_src_code/AFASpectra.c
add_files -tb $prj_dir_src_code/AFASpectra.h
add_files -tb $prj_dir_src_code/AFASpectraCommon.h
add_files -tb $prj_dir_src_code/AFATypes.c
add_files -tb $prj_dir_src_code/AFATypes.h
add_files -tb $prj_dir_src_code/AFATestDataSpectra00.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra01.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra02.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra03.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra04.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra05.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra06.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra07.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra08.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra09.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra10.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra11.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra12.c
add_files -tb $prj_dir_src_code/AFATestDataSpectra13.c
add_files -tb $prj_dir_src_code/AFATestDataGoldenResult.c
add_files -tb $prj_dir_src_code/AFATestDataSpectraShort.c
add_files -tb $prj_dir_src_code/AFATestDataSpectraShortIndex0000.c
add_files -tb $prj_dir_src_code/AFATestDataSpectraShortIndex0200.c
add_files -tb $prj_dir_src_code/BoardIO.c
add_files -tb $prj_dir_src_code/BoardIO.h
