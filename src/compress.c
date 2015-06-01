#include "huff.h"
#include "bitstream.h"
#include "compress.h"

huff_node* find_sym (huff_node* head, uint8_t sym)
{
	huff_node* current = head;

	while (current)
	{
		if (current->sym == sym)
			return current;
		current = current->next;
	}

	return NULL;
}

huff_node* gen_huff_nodes (uint8_t* buf, size_t buf_size)
{
	huff_node* head = NULL;
	huff_node* temp;
	huff_node* current;
	int i;

	for (i = 0; i < buf_size; i ++)
	{
		if (!head)
		{
			head = (huff_node*)malloc (sizeof (huff_node));
			head->sym = buf [i];
			head->next = NULL;
			head->prev = NULL;
			head->a = NULL;
			head->d = NULL;
			head->freq = 1;
		}
		else
		{
			temp = find_sym (head, buf [i]);

			if (temp)
			{
				temp->freq ++;
				if (temp->freq > temp->next->freq)
				{
					if (temp == head)
						head = head->next;
					else
						unlink_huff_node (temp);

					current = temp;
					while (current->next && current->next->freq < temp->freq)
						current = current->next;

					link_huff_node (current, temp);
				}
			}
			else
			{
				temp = (huff_node*)malloc (sizeof (huff_node));
				temp->sym = buf [i];
				temp->next = head;
				temp->prev = NULL;
				head->prev = temp;
				temp->a = NULL;
				temp->d = NULL;
				temp->freq = 1;
				head = temp;
			}
		}
	}

	return head;
}

compressed_data elfp_encode (uint8_t* buf, size_t buf_size)
{
	huff_node* head;
	huff_node* current;
	huff_node* new_current;
	huff_node* huff_tree;
	huff_node* stat_data = NULL;
	int i;
	int j;
	bitstream data = initialize_bitstream ();
	compressed_data ret;

	//Determine syms and their respective prefixes
	head = gen_huff_nodes (buf, buf_size);
	current = head;
	while (current)
	{
		if (!stat_data)
		{
			stat_data = (huff_node*)malloc (sizeof (huff_node));
			stat_data->sym = current->sym;
			stat_data->freq = current->freq;
			new_current = stat_data;
		}
		else
		{
			link_huff_node (new_current, (huff_node*)malloc (sizeof (huff_node)));
			new_current = new_current->next;
			new_current->sym = current->sym;
			new_current->freq = current->freq;
		}
		current = current->next;
	}		
	huff_tree = gen_huff_tree (head);
	gen_huff_prefixes (huff_tree, 0, 0, 0);
	head = NULL;
	recreate_queue (&head, huff_tree);

	for (i = 0; i < buf_size; i ++)
	{
		current = find_sym (head, buf [i]);

		for (j = 0; j < current->prefix_len; j ++)
			add_bit (data, (current->prefix >> (current->prefix_len - 1 - j)) & 1);
	}

	ret.data = data;
	ret.stat_data = stat_data;

	return ret;
}

uint8_t* elfp_decode (compressed_data to_decode, size_t ret_size)
{
	uint8_t* ret = (uint8_t*)malloc (ret_size);
	int16_t temp_sym;
	int i = 0;
	huff_node* head = NULL;
	huff_node* huff_tree;

	huff_tree = gen_huff_tree (to_decode.stat_data);
	//gen_huff_prefixes (huff_tree, 0, 0, 0);
	//recreate_queue (&head, huff_tree);

	temp_sym = next_sym (huff_tree, to_decode.data);
	while (temp_sym != -1)
	{
		ret [i] = temp_sym;
		i ++;
		temp_sym = next_sym (huff_tree, to_decode.data);
	}

	return ret;
}
