@echo off
pause
cls

if '%path_set_flag%' == '1' goto label
set path_set_flag=1
echo Extend the path ...
path %path%;C:\Xilinx\Vivado\2015.4\bin;C:\Xilinx\Vivado_HLS\2015.4\bin
:label

rem echo VIVADO_HLS start ...
rem vivado_hls -f ProcessHLSarty.tcl
rem vivado_hls -f ProcessHLSvc709.tcl

echo VIVADO start ...
rem vivado -mode batch -source ProcessVIVarty.tcl -tempDir ../_tmp
vivado -mode batch -source ProcessVIVvc709.tcl -tempDir ../_tmp
pause
