all: huff_test bitstream_test compress_test file_comp_test file_uncomp_test injection_test infected
huff_test: huff_test.o ../src/huff.o ../src/bitstream.o
	gcc -g huff_test.o ../src/huff.o ../src/bitstream.o -o huff_test
huff_test.o: huff_test.c ../src/huff.h
	gcc -g -c huff_test.c
bitstream_test: bitstream_test.o ../src/bitstream.o
	gcc -g bitstream_test.o ../src/bitstream.o -o bitstream_test
bitstream_test.o: bitstream_test.c ../src/bitstream.o
	gcc -g -c bitstream_test.c
compress_test: compress_test.o ../src/compress.o ../src/bitstream.o ../src/huff.o
	gcc -g compress_test.o ../src/compress.o ../src/bitstream.o ../src/huff.o -o compress_test
compress_test.o: compress_test.c ../src/compress.h ../src/bitstream.h ../src/huff.h
	gcc -g -c compress_test.c
file_comp_test: file_comp_test.o ../src/huff.o ../src/bitstream.o ../src/compress.o
	gcc -g file_comp_test.o ../src/compress.o ../src/bitstream.o ../src/huff.o -o file_comp_test
file_comp_test.o: file_comp_test.c ../src/compress.h ../src/format.h ../src/huff.h
	gcc -g -c file_comp_test.c
file_uncomp_test: file_uncomp_test.o ../src/huff.o ../src/bitstream.o ../src/compress.o
	gcc -g file_uncomp_test.o ../src/huff.o ../src/bitstream.o ../src/compress.o -o file_uncomp_test
file_uncomp_test.o: file_uncomp_test.c ../src/compress.h ../src/format.h ../src/huff.h
	gcc -g -c file_uncomp_test.c
injection_test: ../src/elf_inject.o injection_test.o
	gcc -g injection_test.o ../src/elf_inject.o -o injection_test -lm
injection_test.o: injection_test.c ../src/elf_inject.h
	gcc -g -c injection_test.c
infected: infected.c
	/usr/local/musl/bin/musl-gcc -fPIC -static infected.c -o infected
clean:
	rm bitstream_test.o huff_test.o compress_test.o file_comp_test.o file_uncomp_test.o
