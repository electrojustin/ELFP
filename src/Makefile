all: huff.o bitstream.o compress.o elf_inject.o
huff.o: huff.c huff.h bitstream.h
	gcc -g -c huff.c
bitstream.o: bitstream.c bitstream.h
	gcc -g -c bitstream.c
compress.o: compress.c compress.h bitstream.h huff.h
	gcc -g -c compress.c
elf_inject.o: elf_inject.c elf_inject.h
	gcc -g -c elf_inject.c
clean:
	rm bitstream.o huff.o compress.o elf_inject.o
