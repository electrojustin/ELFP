all: huff.o bitstream.o
huff.o: huff.c huff.h bitstream.h
	gcc -g -c huff.c
bitstream.o: bitstream.c bitstream.h
	gcc -g -c bitstream.c
clean:
	rm bitstream.o huff.o
