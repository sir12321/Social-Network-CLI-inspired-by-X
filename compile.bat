
@echo off
rem Compile script for Windows (batch)
setlocal

rem Try to use MSYS2 UCRT g++ if present, otherwise use plain g++ from PATH
set MSYS_GPP=C:\msys64\ucrt64\bin\g++.exe
set SRC=long_ass.cpp
set OUT=long_ass.exe

if exist "%MSYS_GPP%" (
	echo Using %MSYS_GPP%
	"%MSYS_GPP%" -std=c++17 -O2 "%SRC%" -o "%OUT%"
) else (
	echo Using g++ from PATH
	g++ -std=c++17 -O2 "%SRC%" -o "%OUT%"
)

if %ERRORLEVEL% neq 0 (
	echo Compilation failed with error %ERRORLEVEL%
	exit /b %ERRORLEVEL%
) else (
	echo Compilation successful. Output: %OUT%
)

endlocal