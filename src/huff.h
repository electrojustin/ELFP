#pragma once

#include <inttypes.h>
#include "bitstream.h"

struct huff_node
{
	struct huff_node* a;
	struct huff_node* d;

	uint8_t sym;

	int freq;

	struct huff_node* next;
	struct huff_node* prev;

	int prefix;
	int prefix_len;
};

typedef struct huff_node huff_node;

huff_node* link_huff_node (huff_node* current, huff_node* to_link);
huff_node* unlink_huff_node (huff_node* current);
huff_node* gen_huff_tree (huff_node* head);
//Invoke with gen_huff_prefixes (head, 0, 0, 0);
void gen_huff_prefixes (huff_node* head, uint8_t last_prefix, uint8_t prefix_len, char branch);
//Turns the tree into a queue again, this time with prefixes
//Note: due to the recursive nature of this algorithm, a pointer to a NULL pointer must be initially passed as the "head" parameter
//After the queue has been made, that NULL pointer will no longer be null and will instead point to the head of the queue
//Note: destructive to the tree, internal nodes will be free'd
void recreate_queue (huff_node** head, huff_node* tree);
//Traverses the Huffman tree using the current position in the bitstream and returns the symbol the current prefix corresponds to
int16_t next_sym (huff_node* tree, bitstream in);
