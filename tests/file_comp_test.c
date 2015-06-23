#include "../src/compress.h"
#include "../src/format.h"
#include "../src/huff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char** argv)
{
	FILE* input_file;
	FILE* output_file;
	size_t input_file_size;
	char* input_buf;
	compressed_data output;
	huff_node* current;
	huff_node* temp;
	int num_syms = 0;
	elfp_hdr header;
	memset (&header, 0, sizeof (elfp_hdr));
	sym_hdr current_sym;
	memset (&current_sym, 0, sizeof (sym_hdr));

	if (argc != 3)
	{
		printf ("Usage: file_comp_test infilename outfilename\n");
		exit (-1);
	}

	input_file = fopen (argv [1], "r");
	fseek (input_file, 0, SEEK_END);
	input_file_size = ftell (input_file);
	fseek (input_file, 0, SEEK_SET);
	input_buf = malloc (input_file_size);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (input_file);
	
	output = elfp_encode (input_buf, input_file_size);

	current = output.stat_data;
	while (current)
	{
		num_syms ++;
		current = current->next;
	}

	header.sym_offset = sizeof (elfp_hdr);
	header.num_syms = num_syms;
	header.data_offset = sizeof (elfp_hdr) + num_syms * sizeof (sym_hdr);
	header.num_bits = output.data->num_bits;
	header.comp_size = output.data->buf_size;
	header.uncomp_size = input_file_size;

	output_file = fopen (argv [2], "w");
	fwrite (&header, sizeof (elfp_hdr), 1, output_file);
	current = output.stat_data;
	while (current)
	{
		current_sym.sym = current->sym;
		current_sym.freq = current->freq;
		current = current->next;
		fwrite (&current_sym, sizeof (sym_hdr), 1, output_file);
	}
	fwrite (output.data->buf, sizeof (uint8_t), output.data->buf_size, output_file);

	current = output.stat_data;
	while (current)
	{
		temp = current->next;
		free (current);
		current = temp;
	}
	cleanup_bitstream (output.data);
	fclose (output_file);
	free (input_buf);
}
