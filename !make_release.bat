
set ReleaseDir=c:/incoming/sdss

mkdir "%ReleaseDir%/export"

for %%d in (analyzer dump search compare spectraMapper) do (
	copy "%%dWin32Release.exe" "%ReleaseDir%" /Y /B
	copy "%%dx64Release.exe" "%ReleaseDir%" /Y /B
)

for %%d in (IL ILU) do (
	copy "%%dWin32.dll" "%ReleaseDir%" /Y /B
	copy "%%dx64.dll" "%ReleaseDir%" /Y /B
)


copy "settings.xml" "%ReleaseDir%" /Y /B
copy "spectraParamDR9.bin" "%ReleaseDir%" /Y /B
copy "spectraParamDR10.bin" "%ReleaseDir%" /Y /B
copy "spectraParamDR12.bin" "%ReleaseDir%" /Y /B
copy "spectraParamDR14.bin" "%ReleaseDir%" /Y /B

cd export
for %%d in (up down left right empty) do (
	copy "%%d.png" "%ReleaseDir%/export" /Y /B
)

for %%d in (!DELETE_ALL !DELETE_HTMLS !DELETE_ICONS) do (
	copy "%%d.bat" "%ReleaseDir%/export" /Y /B
)

cd..