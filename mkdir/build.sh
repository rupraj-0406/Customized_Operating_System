nasm -f elf64 -o start.o start.asm
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c scan.c
ld -nostdlib -Tlink.lds  -o user start.o main.o scan.o lib.a 
objcopy -O binary user mkdir 
