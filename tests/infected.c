#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char** argv)
{
	int (*real_main)(int argc, char** argv);
	int temp;
	temp = 0;
	real_main = 0x400506;
	temp += 123;
	temp ++;
	char* temp2 = malloc (temp);
	free (temp2);
	strlen ("asdf");
	bzero (temp2, temp);
	return real_main (argc, argv);
}
