#include "../src/compress.h"
#include "../src/format.h"
#include "../src/bitstream.h"
#include <stdio.h>
#include <stdlib.h>

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
	input_buf = malloc (input_file_size);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (input_file);

	header = (elfp_hdr*)input_buf;
	sym_tab = (sym_hdr*)(input_buf + header->sym_offset);
	input.data = (bitstream)malloc (sizeof (struct bit_desc));
	input.data->buf = input_buf + header->data_offset;
	input.data->buf_size = header->comp_size;
	input.data->num_bits = header->num_bits;
	seek_bit (input.data, 0);
	input.stat_data = NULL;
	for (i = 0; i < header->num_syms; i ++)
	{
		if (!input.stat_data)
		{
			input.stat_data = (huff_node*)malloc (sizeof (huff_node));
			input.stat_data->next = NULL;
			input.stat_data->prev = NULL;
			temp = input.stat_data;
		}
		else
		{
			link_huff_node (temp, malloc (sizeof (huff_node)));
			temp = temp->next;
		}
		temp->sym = sym_tab [i].sym;
		temp->freq = sym_tab [i].freq;
		temp->a = NULL;
		temp->d = NULL;
	}

	out_buf = elfp_decode (input, header->uncomp_size);

	output_file = fopen (argv [2], "w");
	fwrite (out_buf, 1, header->uncomp_size, output_file);
	free (out_buf);	
	fclose (output_file);
	free (input_buf);
	free (input.data);
}
