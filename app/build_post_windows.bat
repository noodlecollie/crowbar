@echo off

REM Crowbar post-build script for Windows - X6Herbius, 11/11/13

REM I fucking hate Windows batch.
setlocal ENABLEDELAYEDEXPANSION

echo ===== Crowbar post-build script for Windows =====
REM When this is run, !CD! will be <build dir>/app and the input should be the original directory.

REM Resource directory
set SOURCE=%1
set CONFIG=%2
set DEST=!CD!\..

set resourceSrc=!SOURCE!\resource
set resourceDest=!DEST!\app\!CONFIG!\resource

echo - Compiling in !CONFIG! mode -
echo Source directory: !SOURCE!
echo Destination root directory: !DEST!
echo Resource - SOURCE: !resourceSrc!
echo Resource - DESTINATION: !resourceDest!

REM Search all files in the directory and copy them into the second directory.
mkdir !resourceDest!
cd !resourceSrc!
if ERRORLEVEL 1 goto ErrorResource

for /r %%i in (*) do (
copy /Y %%i !resourceDest!
)
exit

:ErrorResource
echo Could not CD into resource directory.
exit