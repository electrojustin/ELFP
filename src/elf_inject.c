#include "elf_inject.h"
#include <elf.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

blob elf_inject (char* code, size_t code_size, char* elf_buf, size_t elf_buf_size)
{
	Elf64_Ehdr* file_header = (Elf64_Ehdr*)elf_buf;
	Elf64_Phdr* program_tables = (Elf64_Phdr*)(elf_buf + file_header->e_phoff);
	//We not only need to inject the code, but create a new segment for it
	char* new_elf_buf;
	Elf64_Ehdr* new_file_header;
	Elf64_Phdr* new_program_tables;
	int i;
	uint64_t entry_point = file_header->e_entry;
	Elf64_Phdr new_segment;
	Elf64_Phdr* first_loadable = NULL;
	Elf64_Phdr* text_segment;
	blob ret;

	for (i = 0; i < file_header->e_phnum; i ++)
	{
		if (program_tables [i].p_vaddr < entry_point && program_tables [i].p_vaddr + program_tables [i].p_memsz > entry_point)
			text_segment = (Elf64_Phdr*)(new_elf_buf + program_tables [i].p_offset);
		if (program_tables [i].p_type == PT_LOAD && !first_loadable)
			first_loadable = &program_tables [i];
		if (text_segment && first_loadable)
			break;
	}

	//Setup the new segment
	new_segment.p_type = PT_LOAD;
	new_segment.p_offset = elf_buf_size;
	new_segment.p_vaddr = first_loadable->p_vaddr + new_segment.p_offset;
	new_segment.p_paddr = new_segment.p_vaddr;
	new_segment.p_filesz = code_size + (file_header->e_phnum + 1) * file_header->e_phentsize;
	new_segment.p_memsz = new_segment.p_filesz;
	new_segment.p_flags = PF_R | PF_X;
	new_segment.p_align = getpagesize ();

	//Allocate memory for the new segment
	new_elf_buf = (char*)malloc (new_segment.p_offset + code_size + (file_header->e_phnum +1) * file_header->e_phentsize);
	ret.buf_size = new_segment.p_offset + code_size + (file_header->e_phnum + 1) * file_header->e_phentsize;
	ret.buf = new_elf_buf;

	//Copy the ELF
	memcpy (new_elf_buf, elf_buf, elf_buf_size);

	//Copy the old program headers to the end of the file
	memcpy (new_elf_buf + elf_buf_size, elf_buf + file_header->e_phoff, file_header->e_phnum * file_header->e_phentsize);

	//Copy the new segment to the end of the new program header table
	memcpy (new_elf_buf + elf_buf_size + file_header->e_phnum * file_header->e_phentsize, &new_segment, sizeof (Elf64_Phdr));

	//Copy the new code
	memcpy (new_elf_buf + ret.buf_size - code_size, code, code_size);

	//Modify the ELF header
	new_file_header = (Elf64_Ehdr*)new_elf_buf;
	new_file_header->e_phnum ++;
	new_file_header->e_phoff = elf_buf_size;
	new_file_header->e_entry = ret.buf_size - code_size + first_loadable->p_vaddr;
	new_file_header->e_shoff = 0;
	new_file_header->e_shnum = 0;
	new_file_header->e_shentsize = 0;
	new_file_header->e_shstrndx = 0;

	free (elf_buf);
	return ret;
}
