all: huff_test bitstream_test compress_test
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
clean:
	rm bitstream_test.o huff_test.o
