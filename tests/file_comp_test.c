#include "../src/compress.h"
#include "../src/format.h"
#include "../src/huff.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main (int argc, char** argv)
{
	int input_file;
	FILE* output_file;
	struct stat input_stat;
	size_t input_file_size;
	char* input_buf;
	compressed_data output;
	huff_node* current;
	huff_node* temp;
	int num_syms;
	elfp_hdr header;
	sym_hdr current_sym;

	if (argc != 3)
	{
		printf ("Usage: file_comp_test infilename outfilename\n");
		exit (-1);
	}

	input_file = open (argv [1], O_RDONLY);

	fstat (input_file, &input_stat);
	input_file_size = input_stat.st_size;

	input_buf = mmap (0, input_file_size, PROT_READ, 0, input_file, 0);
	
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
	cleanup_bitstream (output);
	fclose (output_file);
	close (input_file);
}
