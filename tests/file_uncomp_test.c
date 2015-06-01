#include "../src/format.h"
#include "../src/bitstream.h"
#include <stdio.h>

int main (int argc, char** argv)
{
	FILE* input_file;
	char* input_buf;
	size_t input_file_size;
	FILE* output_file;
	elfp_hdr* header;
	sym_hdr* sym_tab;
	compressed_data input;
	int i;
	huff_node* temp;
	char* out_buf;

	if (argc != 3)
	{
		printf ("Usage: file_comp_test infilename outfilename\n");
		exit (-1);
	}

	input_file = fopen (argv [1], "r");
	fseek (input_file, 0, SEEK_END);
	input_file_size = ftell (input_file);
	fseek (input_file, 0 , SEEK_SET);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (executable);

	header = input_buf;
	sym_tab = input_buf + header->sym_offset;
	input.data->buf = input_buf + header->data_offset;
	input.data->buf_size = header->comp_size;
	input.data->num_bits = header->num_bits;
	seek_bit (input, 0);
	input.stat_data = NULL;
	for (i = 0; i < header->num_syms; i ++)
	{
		if (input.stat_data)
		{
			input.stat_data = (huff_node*)malloc (sizeof (huff_node));
			input.stat_data->next = NULL;
			input.stat_data->prev = NULL;
		}
		else
		{
			temp = (huff_node*)malloc (sizeof (huff_node));
			temp->next = input.stat_data;
			temp->next->prev = temp;
			input.stat_data = temp;
		}
		input.stat_data->sym = sym_tab [i].sym;
		input.stat_data->freq = sym_tab [i].freq;
	}

	out_buf = elfp_decode (input, header->uncomp_size);

	output_file = fopen (argv [2], "w");
	fwrite (out_buf, 1, header->uncomp_size, output_file);
	free (out_buf);
	cleanup_bitstream (input.data);
	while (input.stat_data)
	{
		temp = input.stat_data->next;
		free (input.stat_data);
		input.stat_data = temp;
	}
	fclose (output_file);
