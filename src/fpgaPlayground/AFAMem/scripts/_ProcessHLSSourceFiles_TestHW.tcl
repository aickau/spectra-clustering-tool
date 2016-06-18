# Hardware entry function
set top_function	AFAProcess_HW
set_top $top_function

# Files to build the hardware
add_files     $prj_dir_src_code/AFADefines.h
add_files     $prj_dir_src_code/AFAProcessingHW.c
add_files     $prj_dir_src_code/AFAProcessingHW.h
add_files     $prj_dir_src_code/AFATypes.h

# Testbench files
add_files -tb $prj_dir_src_code/main.c
# -cflags "-D_FILE_OFFSET_BITS=64"
add_files -tb $prj_dir_src_code/AFAConfig.h
add_files -tb $prj_dir_src_code/AFADefines.h
add_files -tb $prj_dir_src_code/AFAProcessing.h
add_files -tb $prj_dir_src_code/AFAProcessingHWWrapper.c
add_files -tb $prj_dir_src_code/AFAProcessingCommon.h
add_files -tb $prj_dir_src_code/AFATypes.c
add_files -tb $prj_dir_src_code/AFATypes.h
add_files -tb $prj_dir_src_code/BoardIO.c
add_files -tb $prj_dir_src_code/BoardIO.h
add_files -tb $prj_dir_src_code/AFATime.h
add_files -tb $prj_dir_src_code/AFATime.c