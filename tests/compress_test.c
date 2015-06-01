#include <stdio.h>
#include "../src/compress.h"

int main (void)
{
	char buf [6];
	char temp;
	uint8_t* decode_buf;
	buf [0] = 'h';
	buf [1] = 'e';
	buf [2] = 'l';
	buf [3] = 'l';
	buf [4] = 'o';
	buf [5] = '\0';
	compressed_data dat = elfp_encode (buf, 6);
	seek_bit (dat.data, 0);
	temp = next_bit (dat.data);
	while (temp != -1)
	{
		printf ("%d\n", temp);
		temp = next_bit (dat.data);
	}
	seek_bit (dat.data, 0);
	decode_buf = elfp_decode (dat, 6);
	printf ("%s\n", decode_buf);
	printf ("saved %d bytes\n", 6 - dat.data->buf_size);
}
