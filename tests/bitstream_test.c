#include "../src/bitstream.h"
#include <stdio.h>

int main (void)
{
	bitstream tester = initialize_bitstream ();

	add_bit (tester, 1);
	add_bit (tester, 0);
	add_bit (tester, 1);
	add_bit (tester, 0);
	add_bit (tester, 1);
	add_bit (tester, 0);
	add_bit (tester, 1);
	add_bit (tester, 1);

	add_bit (tester, 1);
	add_bit (tester, 1);
	add_bit (tester, 1);
	add_bit (tester, 1);

	printf ("%d %d\n", tester->buf [0], tester->buf [1]);

	seek_bit (tester, 0);
	printf ("%d\n", tester->num_bits);
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));
	printf ("%d\n", next_bit (tester));

	cleanup_bitstream (tester);
}
