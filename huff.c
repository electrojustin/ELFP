// Implementation of Huffman coding
#include <stdlib.h>
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
		current = head;
		while (current)
		{
			if (current->freq < temp_parent->freq || !current->next)
			{
				link_huff_node (current, temp_parent);
				break;
			}
		}
	}

	return head;
}

void gen_huff_prefixes (huff_node* head, uint8_t last_prefix, uint8_t prefix_len, uint8_t branch)
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
