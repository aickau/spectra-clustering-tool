@echo off
cls
del *.log >nul 2>nul
del *.jou >nul 2>nul
del *.dmp >nul 2>nul
del *.txt >nul 2>nul
vivado -mode batch -init
