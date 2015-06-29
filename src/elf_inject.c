#include <unistd.h>
#include <math.h>

char* elf_inject (char* code, size_t code_size, char* elf_buf, size_t elf_buf_size)
{
	Elf64_Ehdr* file_header = (Elf64_Ehdr*)elf_buf;
	Elf64_Phdr* program_tables = (Elf64_Phdr*)(elf_buf + file_header->e_phoff);
	//We not only need to inject the code, but create a new segment for it
	char* new_elf_buf;
	Elf64_Ehdr* new_file_header;
	memset (new_elf_buf, 0, elf_buf_size + sizeof (Elf64_Phdr) + code_size);
	Elf64_Phdr* new_program_tables = (Elf64_Phdr*)(new_elf_buf + file_header->e_phoff);
	int i;
	uint64_t entry_point = file_header->e_entry;
	uint64_t current_offset;
	Elf64_Phdr new_segment;

	//Setup the new segment
	new_segment.p_type = PT_LOAD;
	//"Loadable process segments must have congruent values for p_vaddr and p_offset, modulo
	//the page size."
	//"p_vaddr should equal p_offset modulo p_align." >:|
	new_segment.p_offset = ceil ((elf_buf_size + sizeof (Elf64_Phdr)) / (double)getpagesize()) * getpagesize();
	new_segment.p_vaddr = ceil ((text_segment->p_vaddr + text_segment->p_memsz) / (double)getpagesize()) * getpagesize();
	new_segment.p_filesz = code_size;
	new_segment.p_memsz = code_size;
	new_segment.p_flags = PF_R | PF_X;
	new_segment.p_align;

	//Allocate memory for the new segment
	new_elf_buf = (char*)malloc (new_segment.p_offset + code_size);

	//Copy everything up to the end of the program tables
	memcpy (new_elf_buf, elf_buf, file_header->e_phoff + sizeof (Elf64_Phdr)*file_header->e_phnum);
	current_offset = file_header->e_phoff + sizeof (Elf64_Phdr)*file_header->e_phnum;

	//Modify the offsets on all the segments since we're adding a new segment
	for (i = 0; i < file_header->e_phnum; i ++)
	{
		if (new_progam_tables [i].p_vaddr < entry_point && new_program_tables [i].p_vaddr + new_program_tables [i].p_memsz > entry_point)
			text_segment = new_elf_buf + new_program_tables [i].p_offset;

		if (new_program_table [i].p_offset > file_header->e_phoff)
			new_program_tables [i].p_offset += sizeof (Elf64_Phdr);
	}

	//Copy the new segment
	memcpy (new_elf_buf + current_offset, &new_segment, sizeof (Elf64_Phdr));
	current_offset += sizeof (Elf64_Phdr);

	//Copy the rest of the elf
	memcpy (new_elf_buf + current_offset, elf_buf + current_offset - sizeof (Elf64_Phdr), elf_buf_size - file_header->e_phoff);

	//Copy the new code
	memcpy (new_elf_buf + new_segment.p_offset, code, code_size);

	//Modify the ELF header
	new_file_header = (Elf64_Ehdr*)new_elf_buf;
	new_file_header->e_phnum ++;
	new_file_header->e_entry = new_segment.p_vaddr;

	free (elf_buf);
	return new_elf_buf;
}
