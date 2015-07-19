#include <stdio.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>

//Short program to extract the relevant executable parts of a static, positition independent ELF

int main (int argc, char** argv)
{
	FILE* input_file;
	FILE* output_file;
	size_t input_file_size;
	char* input_buf;
	char* output_buf;
	Elf64_Shdr* section_table;
	Elf64_Ehdr* file_header;
	char* section_string_table;
	char* string_table = NULL;
	Elf64_Sym* symbol_table = NULL;
	Elf64_Phdr* program_tables;
	int i;
	uint64_t eh_frame_off = 0;
	uint64_t main_off = 0;
	uint64_t base_addr;

	input_file = fopen (argv [1], "r");
	fseek (input_file, 0, SEEK_END);
	input_file_size = ftell (input_file);
	fseek (input_file, 0, SEEK_SET);
	input_buf = malloc (input_file_size);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (input_file);

	file_header = (Elf64_Ehdr*)input_buf;
	section_table = (Elf64_Shdr*)(input_buf + file_header->e_shoff);
	section_string_table = input_buf + section_table [file_header->e_shstrndx].sh_offset;
	program_tables = (Elf64_Phdr*)(input_buf + file_header->e_phoff);

	//Find base virtual address
	for (i = 0; i < file_header->e_phnum; i ++)
	{
		if (program_tables [i].p_type == PT_LOAD)
		{
			base_addr = program_tables [i].p_vaddr;
			break;
		}
	}
		

	//Find .eh_frame, string table, and symbol table
	for (i = 0; i < file_header->e_shnum; i ++)
	{
		if (eh_frame_off && string_table && symbol_table)
			break;
		if (!strcmp (section_string_table + section_table [i].sh_name, ".eh_frame"))
			eh_frame_off = section_table [i].sh_offset;
		if (!strcmp (section_string_table + section_table [i].sh_name, ".strtab"))
			string_table = input_buf + section_table [i].sh_offset;
		if (!strcmp (section_string_table + section_table [i].sh_name, ".symtab"))
			symbol_table = (Elf64_Sym*)(input_buf + section_table [i].sh_offset);
	}

	//Find main
	i = 0;
	while (!main_off)
	{
		if (!strcmp (string_table + symbol_table [i].st_name, "main"))
			main_off = symbol_table [i].st_value - base_addr;

		i ++;
	}

	output_buf = malloc (eh_frame_off - main_off);
	memcpy (output_buf, input_buf + main_off, eh_frame_off - main_off);

	output_file = fopen (argv [2], "w");
	fwrite (output_buf, eh_frame_off - main_off, 1, output_file);
	fclose (output_file);
	free (output_buf);
}
