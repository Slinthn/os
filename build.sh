pushd bin

rm *

clear
echo "[Compiling binaries]"
echo 

nasm ../src/bootloader.s -o bootloader.bin
gcc -c ../src/kernel.c -o kernel.obj -ffreestanding -Wall -Wextra -ffreestanding -fno-asynchronous-unwind-tables
ld -Ttext 0x7E00 -o kernel.elf -T ../linker.ld kernel.obj -nostdlib


if [ $? -ne 0 ]
then
	exit
fi

objcopy --remove-section .comment --remove-section .data.rel.local --remove-section .note.gnu.property -O binary kernel.elf kernel.bin
cat bootloader.bin kernel.bin > os.bin

echo
echo "[Outputting debug information]"
echo

objdump -s kernel.elf > kernel.objdump
objdump -j .text -D kernel.elf > kernel.disassembly

dd if=os.bin of=os.img bs=512 count=100

popd 

if [ $# -eq 1 ]
then
	echo
	echo "[Running program]"
	echo

	qemu-system-x86_64 bin/os.img
fi

if [ $# -eq 2 ]
then
	echo
	echo "[Exporting OS]"
	echo

	dd if=bin/os.img of=/dev/sdb count=1
fi
