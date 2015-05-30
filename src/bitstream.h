#pragma once

#include <inttypes.h>
#include <stdlib.h>

struct bit_desc
{
	uint8_t* buf;
	size_t buf_size;
	int64_t num_bits;
	int pos;
};

typedef struct bit_desc* bitstream;

bitstream initialize_bitstream (void);
void add_bit (bitstream target, char data);
void seek_bit (bitstream target, int pos);
char next_bit (bitstream target);
void cleanup_bitstream (bitstream target);
