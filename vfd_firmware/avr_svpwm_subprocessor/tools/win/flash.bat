@echo off & setlocal

:: ### General settings for flashing procedure ###
:: ### THESE SETTINGS CAN BE CHANGED ###
:definitions
set target=firmware
set mcuid=m16
set programmer=avrispmkII
set LFUSE=0x3F
set HFUSE=0xD1
goto :start
:: ### DO NOT CHANGE ANY SETTINGS FROM HERE ###

:: ### Start flashing process
:start
echo "Starte Flaschvorgang..."
set binpath=%cd%\bin
set PATH=%binpath%
cd ..
cd ..
goto :flashing

:: ### Writing fuses to AVR and flash .hex file ###
:flashing
cd bin
%binpath%\avrdude -C %binpath%/../etc/avrdude.conf -c %programmer% -v -p %mcuid% -e -U lfuse:w:%LFUSE%:m -U hfuse:w:%HFUSE%:m -U flash:w:%target%.hex
cd ..
goto :end

:: ### End flashing process
:end
cd tools/win
pause
goto :eof