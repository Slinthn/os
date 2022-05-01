pushd bin

rm *

clear
echo "[Compiling binaries]"
echo 

nasm ../src/bootloader.s -o bootloader.bin
nasm ../src/kernel.s -g -f elf64 -o kernels.obj
gcc -g -c ../src/kernel.c -o kernelc.obj -ffreestanding -Wall -Wextra -ffreestanding -fno-asynchronous-unwind-tables
ld -Ttext 0x7E00 -o kernel.elf -T ../linker.ld kernelc.obj kernels.obj -nostdlib


if [ $? -ne 0 ]
then
	exit
fi

objcopy --remove-section .comment --remove-section .note.gnu.property -O binary kernel.elf kernel.bin
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

	qemu-system-x86_64 -d cpu_reset,guest_errors,pcall,int,mmu bin/os.img -D bin/log.txt -monitor stdio
fi

if [ $# -eq 2 ]
then
	echo
	echo "[Exporting OS]"
	echo

	if [ -e "/dev/sdb" ]
	then
	  sudo dd if=bin/os.img of=/dev/sdb count=1 bs=4096
	else
		echo "Insert USB!"
	fi
fi

if [ $# -eq 3 ]
then
	echo
	echo "[Debugging program]"
	echo

	qemu-system-x86_64 -s -S bin/os.img -D bin/log.txt -monitor stdio
fi
