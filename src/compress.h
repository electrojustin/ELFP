#pragma once

struct compressed_data
{
	huff_node* stat_data;
	bitstream data;
};

typedef struct compressed_data compressed_data;

huff_node* find_sym (huff_node* head, uint8_t sym);
huff_node* gen_huff_nodes (uint8_t* buf, size_t buf_size);
compressed_data elfp_encode (uint8_t* buf, size_t buf_size);
uint8_t* elfp_decode (compressed_data to_decode, size_t ret_size);
