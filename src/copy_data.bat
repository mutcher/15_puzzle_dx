@echo off
:: First parameter - out data dir - $(OutDir)\data
:: Second parameter - source data dir - $(SolutionDir)\data 

:: Setting local env variables
setlocal

set FILE_MASK=*.tga
set SRC_DIR_FILEMASK="%2\%FILE_MASK%"
set OUT_DIR="%1"

:: Creating data directory if not exists
if not exist "%1" mkdir "%1"

:: Copying *.tga files from source dir to out dir
xcopy /Y %SRC_DIR_FILEMASK% %OUT_DIR%