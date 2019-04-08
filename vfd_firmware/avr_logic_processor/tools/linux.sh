#!/bin/bash

# ### General settings for code generation ###
# ### THESE SETTINGS CAN BE CHANGED ###
target="firmware"
mcu="atmega16a"
mcuid="m16"
cflags="-Wall -Wextra -Os -mmcu=$mcu"
programmer="avrispmkII"
LFUSE="0x3F"
HFUSE="0xD1"
# ### DO NOT CHANGE ANY SETTINGS FROM HERE ###


case "$1" in
	flash)
		# ### Start flashing process ###
		echo "Starte Flashvorgang..."
		binpath="$(pwd)/linux/bin"
		export PATH="$PATH:$binpath"
		cd ..

		# ### Writing fuses to AVR and flash .hex file
		cd bin
		$binpath/avrdude -C $binpath/../etc/avrdude.conf -c $programmer -v -p $mcuid -e -U lfuse:w:$LFUSE:m -U hfuse:w:$HFUSE:m -U flash:w:$target.hex
		cd ..

		# ### End flashing process ###
		cd tools
 	;;
 	compile)
 		# ### Start compilation process ###
		echo "Starte Compiliervorgang..."
		binpath="$(pwd)/linux/bin"
		export PATH="$PATH:$binpath"
		cd ..

		# ### Generate list of all source files in ./src ###
		cd src
		objects=$(find $(pwd) -name \*.c)
		cd ..

		# ### Generate single .elf file for all sources files ###
		cd obj
		$binpath/avr-gcc $cflags $objects -o $target.elf
		cd ..

		# ### Generate .hex file from .elf file ###
		cd bin
		$binpath/avr-objcopy -O ihex -j .data -j .text ../obj/$target.elf $target.hex
		cd ..

		# ### Calculate filesize allocation for AVR ###
		cd obj
		$binpath/avr-size --mcu=$mcu -C $target.elf
		cd ..

		# ### End compilation process ###
		cd tools
 	;;
 	assemble)
		# ### Start compilation process ###
		echo "Starte Compiliervorgang..."
		binpath="$(pwd)/linux/bin"
		export PATH="$PATH:$binpath"
		cd ..

		# ### Generate .hex file and move it to bin, delete .lst file###
		cd src
		$binpath/gavrasm $target.asm
		mv ./$target.hex ../bin/$target.hex
		rm ./$target.lst
		cd ..

		# ### End compilation process ###
		cd tools
 	;;
 	eeprom)
		# ### Start flashing process ###
		echo "Starte EEPROM Schreibvorgang..."
		binpath="$(pwd)/linux/bin"
		export PATH="$PATH:$binpath"
		cd ..

		# ### Writing EEPROM
		cd bin
		$binpath/avrdude -C $binpath/../etc/avrdude.conf -c $programmer -v -p $mcuid -e -U eeprom:w:$target.eep
		cd ..

		# ### End flashing process ###
		cd tools
 	;;
 	*)
 		echo "Usage: ${0} {flash|compile|assemble|eeprom}"
 		exit 2
esac
exit 0