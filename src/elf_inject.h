#pragma once
#include <elf.h>
#include <stddef.h>

struct blob
{
	char* buf;
	size_t buf_size;
};
typedef struct blob blob;

size_t new_file_size;

blob elf_inject (char* code, size_t code_size, char* elf_buf, size_t elf_buf_size);
