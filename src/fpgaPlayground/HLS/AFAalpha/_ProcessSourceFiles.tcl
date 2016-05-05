# Hardware entry function
set_top AFAProcess_HW

# Files to build the hardware
add_files     $src_dir/AFADefines.h
add_files     $src_dir/AFANetworkSettingsHW.h
add_files     $src_dir/AFAProcessingHW.c
add_files     $src_dir/AFAProcessingHW.h
add_files     $src_dir/AFATypes.h

# Testbench files
add_files -tb $src_dir/AFADefines.h
add_files -tb $src_dir/AFANetworkSettings.c
add_files -tb $src_dir/AFANetworkSettings.h
add_files -tb $src_dir/AFANetworkSettingsCommon.h
add_files -tb $src_dir/AFAProcessing.c
add_files -tb $src_dir/AFAProcessing.h
add_files -tb $src_dir/AFAProcessingHWWrapper.c
add_files -tb $src_dir/AFAProcessingCommon.h
add_files -tb $src_dir/AFARandom.c
add_files -tb $src_dir/AFARandom.h
add_files -tb $src_dir/AFARandomCommon.h
add_files -tb $src_dir/AFASpectra.c
add_files -tb $src_dir/AFASpectra.h
add_files -tb $src_dir/AFASpectraCommon.h
add_files -tb $src_dir/AFATypes.c
add_files -tb $src_dir/AFATypes.h
add_files -tb $src_dir/main.c
