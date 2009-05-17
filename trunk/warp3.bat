cd export
del /S *.html 
del /S spSpec*.PNG
del /S LocalComparsions*.PNG
cd..
analyzerWin32Release.exe zrange1.bin
xcopy export zrange1 /S /R /Y

