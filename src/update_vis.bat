echo off
set VIS_DIR="c:\prj\vis"

for %%i in (visWin32Debug.dll visWin32Debug.pdb visWin32Release.dll) do copy "%VIS_DIR%\%%i" ..\

for %%i in (visWin32Debug.lib visWin32Debug.exp visWin32Release.lib visWin32Release.exp) do copy "%VIS_DIR%\%%i" 3rdparty\api\vis\lib\
