all: huff_test bitstream_test
huff_test: huff_test.o ../src/huff.o ../src/bitstream.o
	gcc -g huff_test.o ../src/huff.o ../src/bitstream.o -o huff_test
huff_test.o: huff_test.c ../src/huff.h
	gcc -g -c huff_test.c
bitstream_test: bitstream_test.o ../src/bitstream.o
	gcc -g bitstream_test.o ../src/bitstream.o -o bitstream_test
bitstream_test.o: bitstream_test.c ../src/bitstream.o
	gcc -g -c bitstream_test.c
clean:
	rm bitstream_test.o huff_test.o
