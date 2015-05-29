//Defines all the necessary datastructures for reading and writing compressed data to a file

#pragma once

//Compressed files will contain the memory image of an array of the following struct
//Since it's trivial to reconstruct the tree with just the symbol and its frequency, there's no reason to include the prefix in the symbol table. 
//Prefixes are recalculated on decompression.
struct sym_hdr
{
	uint8_t sym;
	int freq;
};

typedef struct sym_hdr sym_hdr;

//The memory image of the following struct will be the first thing in a compressed file
struct elfp_hdr
{
	uint64_t sym_offset; //Offset to the symbol table
	uint32_t num_syms; //Number of entries in symbol table

	uint64_t data_offset; //Offset to compressed data
	size_t comp_size; //Size of the compressed data in file
	size_t uncomp_size; //Size of decompressed data
};

typedef struct elfp_hdr elfp_hdr;
