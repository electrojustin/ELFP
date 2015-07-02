#pragma once
#include <elf.h>

Elf64_Phdr* text_segment;
size_t new_file_size;

char* elf_inject (char* code, size_t code_size, char* elf_buf, size_t elf_buf_size);
