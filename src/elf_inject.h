#pragma once
#include <elf.h>

Elf64_Phdr* text_segment;

void elf_inject (char* code, size_t code_size, char* elf_buf);
