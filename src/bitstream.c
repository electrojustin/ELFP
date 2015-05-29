#include <stdlib.h>
#include "bitstream.h"

bitstream initialize_bitstream (void)
{
	bitstream target = malloc (sizeof (struct bit_desc));
	
	target->buf = (uint8_t*)malloc (sizeof (uint8_t));
	target->buf [0] = 0;
	target->buf_size = 1;
	target->pos = 0;

	return target;
}

void add_bit (bitstream target, char data)
{
	int i = target->pos / 8;

	if (i + 1 > target->buf_size)
	{
		target->buf_size ++;
		target->buf = (uint8_t*)realloc (target->buf, target->buf_size * sizeof (uint8_t));
		target->buf [i] = 0;
	}

	target->buf [i] |= data << (target->pos % 8);

	target->pos ++;
}

void seek_bit (bitstream target, int pos)
{
	target->pos = pos;
}

char next_bit (bitstream target)
{
	char ret;
	int i = target->pos / 8;

	if (i >= target->buf_size)
		return -1;

	ret = (target->buf [i] >> (target->pos % 8)) & 1;
	target->pos ++;

	return ret;
}

void cleanup_bitstream (bitstream target)
{
	free (target->buf);
}
