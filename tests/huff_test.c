#include <stdio.h>
#include "../src/huff.h"

int main (void)
{
	//Straight from the wikipedia article :D
	huff_node sym1 = {.sym = ' ', .freq = 7};
	huff_node sym2 = {.sym = 'a', .freq = 4};
	huff_node sym3 = {.sym = 'e', .freq = 4};
	huff_node sym4 = {.sym = 'f', .freq = 3};
	huff_node sym5 = {.sym = 'h', .freq = 2};
	huff_node sym6 = {.sym = 'i', .freq = 2};
	huff_node sym7 = {.sym = 'm', .freq = 2};
	huff_node sym8 = {.sym = 'n', .freq = 2};
	huff_node sym9 = {.sym = 's', .freq = 2};
	huff_node sym10 = {.sym = 't', .freq = 2};
	huff_node sym11 = {.sym = 'l', .freq = 1};
	huff_node sym12 = {.sym = 'o', .freq = 1};
	huff_node sym13 = {.sym = 'p', .freq = 1};
	huff_node sym14 = {.sym = 'r', .freq = 1};
	huff_node sym15 = {.sym = 'u', .freq = 1};
	huff_node sym16 = {.sym = 'x', .freq = 1};
	sym16.next = &sym15;
	sym15.next = &sym14;
	sym14.next = &sym13;
	sym13.next = &sym12;
	sym12.next = &sym11;
	sym11.next = &sym10;
	sym10.next = &sym9;
	sym9.next = &sym8;
	sym8.next = &sym7;
	sym7.next = &sym6;
	sym6.next = &sym5;
	sym5.next = &sym4;
	sym4.next = &sym3;
	sym3.next = &sym2;
	sym2.next = &sym1;
	sym1.next = NULL;
	
	huff_node* current;
	current = &sym16;
	while (current)
	{
		current->a = NULL;
		current->d = NULL;
		current = current->next;
	}

	huff_node* new_queue = NULL;
	huff_node* huff_tree = gen_huff_tree (&sym16);
	gen_huff_prefixes (huff_tree, 0, 0, 0);
	recreate_queue (&new_queue, huff_tree);

	current = new_queue;

	while (current)
	{
		printf ("%c %d %d\n", current->sym, current->prefix_len, current->prefix);
		current = current->next;
	}
}
