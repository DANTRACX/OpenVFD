@echo off & setlocal

:: ### General settings for flashing procedure ###
:: ### THESE SETTINGS CAN BE CHANGED ###
:definitions
set target=firmware
set mcuid=m16
set programmer=avrispmkII
goto :start
:: ### DO NOT CHANGE ANY SETTINGS FROM HERE ###

:: ### Start flashing process
:start
echo "Starte EEPROM Schreibvorgang..."
set binpath=%cd%\bin
set PATH=%binpath%
cd ..
cd ..
goto :flashing

:: ### Writing fuses to AVR and flash .hex file ###
:flashing
cd bin
%binpath%\avrdude -C %binpath%/../etc/avrdude.conf -c %programmer% -v -p %mcuid% -e -U eeprom:w:%target%.eep
cd ..
goto :end

:: ### End flashing process
:end
cd tools/win
pause
goto :eof