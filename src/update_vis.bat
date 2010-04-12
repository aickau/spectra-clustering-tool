echo off
set VIS_DIR="c:\prj\vis"

for %%i in (visWin32Debug.dll visWin32Debug.pdb visWin32Release.dll) do copy "%VIS_DIR%\%%i" ..\

for %%i in (visWin32Debug.lib visWin32Debug.exp visWin32Release.lib visWin32Release.exp) do copy "%VIS_DIR%\%%i" 3rdparty\api\vis\lib\

for %%i in (viperror.h vis_app.h vis_basehelper.h vis_basehelper2.h vis_colorbar.h vis_colortable.h vis_compoundtypes.h vis_cursor.h vis_debug.h vis_dialogs.h vis_dialogsdataopt.h vis_dialogsglobalopt.h vis_dllexport.h vis_error.h vis_font.h vis_glhelper.h vis_glview.h vis_gradient.h vis_gradientctrl.h vis_gradientdlg.h vis_helper.h vis_hotkeys.h vis_interndata.h vis_interndatahelper.h vis_interface.h vis_interface_lines.h vis_interface_opt.h vis_interface_points.h vis_interface_tri.h vis_main.h vis_IO.h vis_move.h vis_patterns.h vis_projection.h vis_ruler.h vis_selection.h vis_types.h vis_typesintern.h) do copy "%VIS_DIR%\src\vis\%%i" 3rdparty\api\vis\



