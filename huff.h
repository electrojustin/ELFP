struct huff_node
{
	struct huff_node* a;
	struct huff_node* d;

	uint8_t data;

	uint32_t freq;

	struct huff_node* next;

	uint8_t prefix;
	uint8_t prefix_len;
};

typedef struct huff_node huff_node;

huff_node* link_huff_node (huff_node* current, huff_node* to_link);
void unlink_huff_node (huff_node* current);
huff_node* gen_huff_tree (huff_node* head);
//Invoke with gen_huff_prefixes (head, 0, 0, 0);
void gen_huff_prefixes (huff_node* head, uint8_t last_prefix, uint8_t prefix_len, uint8_t branch);
