@echo off
pause
cls

if '%path_set_flag%' == '1' goto label
set path_set_flag=1
echo Extend the path ...
path %path%;C:\Xilinx\Vivado\2015.4\bin;C:\Xilinx\Vivado_HLS\2015.4\bin
:label

echo VIVADO_HLS start ...
vivado_hls -f ProcessHLSarty.tcl

echo VIVADO start ...
rem vivado -mode batch -source ProcessVIVArty.tcl -tempDir ../_tmp
pause
