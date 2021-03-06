FLAGS=-c -g -w -m32 -fno-stack-protector
all:
	nasm -f aout src/libasm.asm -o bin/libasm.o
	nasm -f aout src/loader.asm -o bin/loader.o
	gcc $(FLAGS) src/kernel.c -o bin/kernel.o -fno-builtin
	gcc $(FLAGS) src/libc.c  -o bin/libc.o -fno-builtin
	gcc $(FLAGS) src/stdio.c  -o bin/stdio.o -fno-builtin
	gcc $(FLAGS) src/shell.c  -o bin/shell.o -fno-builtin
	gcc $(FLAGS) src/string.c  -o bin/string.o -fno-builtin
	ld -melf_i386 -T src/link.ld -o bin/kernel.bin bin/loader.o bin/kernel.o bin/libc.o bin/libasm.o bin/stdio.o bin/shell.o bin/string.o
	mcopy -o bin/kernel.bin b:/boot/
