@echo off
pause
cls

if '%path_set_flag%' == '1' goto label
set path_set_flag=1
echo Extend the path ...
path %path%;C:\Xilinx\Vivado\2016.1\bin;C:\Xilinx\Vivado_HLS\2016.1\bin
:label

rem goto vc709
goto arty

echo please select board!
goto end

:arty
rem ARTY section ------------------------------------------
echo VIVADO_HLS start ...
vivado_hls -f ProcessHLSarty.tcl
echo VIVADO start ...
vivado -mode batch -source ProcessVIVarty.tcl -tempDir ../_tmp
goto end

:vc709
rem vc709 section -----------------------------------------
echo VIVADO_HLS start ...
rem vivado_hls -f ProcessHLSvc709.tcl
echo VIVADO start ...
vivado -mode batch -source ProcessVIVvc709.tcl -tempDir ../_tmp
goto end

:end
pause
