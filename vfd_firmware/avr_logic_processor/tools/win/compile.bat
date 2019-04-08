@echo off & setlocal

:: ### General settings for Code Generation ###
:definitions
set target=firmware
set mcu=atmega16a
set cflags=-Wall -Wextra -Os -mmcu=%MCU%
goto :start

:: ### Start compilation process ###
:start
echo "Starte Compiliervorgang..."
set binpath=%cd%\bin
set PATH=%binpath%
cd ..
cd ..
goto :filesearch

:: ### Generate list of all source files in ./src ###
:filesearch
cd src
FOR /F "tokens=*" %%a in ('dir /s /b *.c') do call :createlist %%a
cd ..
goto :makeelf

:createlist
set objects=%objects% %1
goto :eof

:: ### Generate single.elf file for all source files ###
:makeelf
cd obj
%binpath%\avr-gcc %cflags% %objects% -o %target%.elf
cd ..
goto :makehex

:: ### Generate .hex file from .elf file ###
:makehex
cd bin
%binpath%\avr-objcopy -O ihex -j .data -j .text ../obj/%target%.elf %target%.hex
cd ..
goto :calcsize

:: ### Calculate filesize allocation for AVR ###
:calcsize
cd obj
%binpath%\avr-size --mcu=%mcu% -C %target%.elf
cd ..
goto :end

:: ### End compilation process
:end
cd tools/win
pause
goto :eof