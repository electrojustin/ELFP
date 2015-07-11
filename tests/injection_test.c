#include <stdio.h>
#include <stdlib.h>
#include "../src/elf_inject.h"

int main (int argc, char** argv)
{
	FILE* input_file;
	FILE* output_file;
	size_t input_file_size;
	char* input_buf;
	blob output;
	char code [7] = { 0xBD, 0x20, 0x0C, 0x40, 0x00, 0xFF, 0xE5 };

	input_file = fopen (argv [1], "r");
	fseek (input_file, 0, SEEK_END);
	input_file_size = ftell (input_file);
	fseek (input_file, 0, SEEK_SET);
	input_buf = malloc (input_file_size);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (input_file);

	*(int*)(&code [1]) = ((Elf64_Ehdr*)input_buf)->e_entry;
	output = elf_inject (code, 7, input_buf, input_file_size);

	output_file = fopen (argv [2], "w");
	fwrite (output.buf, output.buf_size, 1, input_file);
	fclose (output_file);
	free (output.buf);
}
