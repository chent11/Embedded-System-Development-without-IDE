#!/bin/bash

# expected input file is the builded .elf file.
if [ ! "$1" = "" ]; then
/usr/local/bin/JLinkGDBServerCL -select USB -device STM32F427VI -endian little -if SWD -speed auto -noir -LocalhostOnly &
# /usr/local/bin/arm-none-eabi-gdb $1 -ex "target remote localhost:2331" -ex "monitor reset" -ex "load"
gdbgui -g /usr/local/bin/arm-none-eabi-gdb --project ./ --gdb-args "$1 -ex \"target remote localhost:2331\" -ex \"monitor reset\" -ex \"load\" -ex \"b main\" -ex \"c\""
killall JLinkGDBServerCL
else
echo "no input file"
fi
