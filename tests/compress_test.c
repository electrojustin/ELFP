#include "../src/compress.h"

int main (void)
{
	char buf [6];
	buf [0] = 'h';
	buf [1] = 'e';
	buf [2] = 'l';
	buf [3] = 'l';
	buf [4] = 'o';
	buf [5] = '\0';
	compressed_data dat = elfp_encode (buf, 6);
	dat.data.pos = 0;
	printf ("%s\n", elfp_decode (dat, 6));
}
