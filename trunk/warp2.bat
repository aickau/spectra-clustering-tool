cd export
del /S *.html 
del /S spSpec*.PNG
del /S LocalComparsions*.PNG
cd..
analyzerWin32Release.exe zrange0.bin
xcopy export zrange0 /S /R /Y


cd export
del /S *.html 
del /S spSpec*.PNG
del /S LocalComparsions*.PNG
cd..
analyzerWin32Release.exe zrange1.bin
xcopy export zrange1 /S /R /Y



cd export
del /S *.html 
del /S spSpec*.PNG
del /S LocalComparsions*.PNG
cd..
analyzerWin32Release.exe zrange2.bin
xcopy export zrange2 /S /R /Y



cd export
del /S *.html 
del /S spSpec*.PNG
del /S LocalComparsions*.PNG
cd..
analyzerWin32Release.exe zrange3.bin
xcopy export zrange3 /S /R /Y
