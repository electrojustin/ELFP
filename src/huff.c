// Implementation of Huffman coding
#include <stdlib.h>
#include <stdio.h>
#include "huff.h"

huff_node* link_huff_node (huff_node* current, huff_node* to_link)
{
	if (current)
	{
		to_link->next = current->next;
		current->next = to_link;
	}
	
	return to_link;
}

huff_node* unlink_huff_node (huff_node* current)
{
	huff_node* temp = current->next;

	current->next = temp->next;

	return temp;
}

huff_node* gen_huff_tree (huff_node* head)
{
	huff_node* temp_a;
	huff_node* temp_d;
	huff_node* temp_parent;
	huff_node* current;

	if (!head)
	{
		printf ("Error: priority queue empty\n");
		exit (-1);
	}

	if (!head->next)
		return head;

	while (head->next->next)
	{
		temp_a = head;
		temp_d = head->next;

		//Dequeue the highest priority nodes
		head = head->next->next;
		temp_a->next = NULL;
		temp_d->next = NULL;

		//Create the temporary parent
		temp_parent = (huff_node*)malloc (sizeof (huff_node));
		temp_parent->next = NULL;
		temp_parent->a = temp_a;
		temp_parent->d = temp_d;
		temp_parent->freq = temp_a->freq + temp_d->freq;

		//Add parent to the priority queue
		if (head->freq > temp_parent->freq)
		{
			temp_parent->next = head;
			head = temp_parent;
		}
		else
		{
			current = head;
			while (current)
			{
				if (!current->next || current->next->freq > temp_parent->freq)
				{
					link_huff_node (current, temp_parent);
					break;
				}
				current = current->next;
			}
		}
	}

	if (head->next)
	{
		temp_a = head;
		temp_d = head->next;
		temp_parent = (huff_node*)malloc (sizeof (huff_node));
		temp_parent->next = NULL;
		temp_parent->a = temp_a;
		temp_parent->d = temp_d;
		temp_parent->freq = temp_a->freq + temp_d->freq;

		head = temp_parent;
	}

	return head;
}

void gen_huff_prefixes (huff_node* head, uint8_t last_prefix, uint8_t prefix_len, char branch)
{
	if (!prefix_len)
	{
		if (head->a) //This is the root node
		{
			gen_huff_prefixes (head->a, 0, 1, 0);
			gen_huff_prefixes (head->d, 0, 1, 1);
		}
		else //This is the ONLY node
		{
			head->prefix = 0;
			head->prefix_len = 1;
		}
	}
	else
	{
		//Figure out the current prefix
		head->prefix_len = prefix_len;
		head->prefix = (last_prefix << (prefix_len - 1)) | branch;

		if (head->a) //This node isn't a leaf
		{
			gen_huff_prefixes (head->a, head->prefix, prefix_len + 1, 0);
			gen_huff_prefixes (head->d, head->prefix, prefix_len + 1, 1);
		}
	}
}

//Turns the tree into a queue again, this time with prefixes
//Note: due to the recursive nature of this algorithm, a pointer to a NULL pointer must be initially passed as the "head" parameter
//After the queue has been made, that NULL pointer will no longer be null and will instead point to the head of the queue
void recreate_queue (huff_node** head, huff_node* tree)
{
	if (!tree->a) //Leaf
	{
		if (*head)
			link_huff_node (*head, tree);
		else
			*head = tree;
	}
	else //Internal node
	{
		recreate_queue (head, tree->a);
		recreate_queue (head, tree->d);
	}
}

//Traverses the Huffman tree using the current position in the bitstream and returns the symbol the current prefix corresponds to
uint8_t next_sym (huff_node* tree, bitstream in)
{
	char bit;

	if (!tree->a) //Leaf
		return tree->sym;
	else
	{
		bit = next_bit (in);

		if (bit == -1)
		{
			printf ("Error: reached end of stream before valid prefix\n");
			exit (-1);
		}

		if (!bit)
			return next_sym (tree->a, in);
		else
			return next_sym (tree->d, in);
	}
}
