@echo off & setlocal

:: ### General settings for Code Generation ###
:definitions
set target=firmware
set mcu=atmega16a
goto :start

:: ### Start compilation process ###
:start
echo "Starte Compiliervorgang..."
set binpath=%cd%\bin
set PATH=%binpath%
cd ..
cd ..
goto :makehex

:: ### Generate .hex file and move it to bin, delete .lst file ###
:makehex
cd src
%binpath%\gavrasm %target%.asm
move %target%.hex ..\bin\%target%.hex
del %target%.lst
cd ..
goto :end

:: ### End compilation process
:end
cd tools\win
pause
goto :eof