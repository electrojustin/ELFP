#include <stdio.h>
#include <stdlib.h>
#include "../src/elf_inject.h"
#include <elf.h>

int main (int argc, char** argv)
{
	FILE* input_file;
	FILE* input_file2;
	FILE* output_file;
	size_t input_file_size;
	char* input_buf;
	char* code;
	size_t code_size;
	Elf64_Phdr* first_loadable;
	Elf64_Phdr* program_tables;
	blob output;
	int i;

	input_file = fopen (argv [1], "r");
	fseek (input_file, 0, SEEK_END);
	input_file_size = ftell (input_file);
	fseek (input_file, 0, SEEK_SET);
	input_buf = malloc (input_file_size);
	fread (input_buf, 1, input_file_size, input_file);
	fclose (input_file);

	input_file2 = fopen (argv [2], "r");
	fseek (input_file2, 0, SEEK_END);
	code_size = ftell (input_file2);
	fseek (input_file2, 0, SEEK_SET);
	code = malloc (code_size);
	fread (code, 1, code_size, input_file2);
	fclose (input_file2);

	program_tables = (Elf64_Phdr*)(input_buf + ((Elf64_Ehdr*)input_buf)->e_phoff);
	for (i = 0; i < ((Elf64_Ehdr*)input_buf)->e_phnum; i ++)
	{
		if (program_tables [i].p_type == PT_LOAD)
		{
			first_loadable = &program_tables [i];
			break;
		}
	}
	//*(int*)(&code [1]) = *(int*)(input_buf + ((Elf64_Ehdr*)input_buf)->e_entry - first_loadable->p_vaddr + 0x20);
	output = elf_inject (code, code_size, input_buf, input_file_size);

	output_file = fopen (argv [3], "w");
	fwrite (output.buf, output.buf_size, 1, output_file);
	fclose (output_file);
	free (output.buf);
}
