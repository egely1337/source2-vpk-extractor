#ifndef FILE_IO_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <string.h>
#include <strings.h>

uint8_t* read_str(FILE* fp);
uint32_t read_u32(FILE* fp);
uint16_t read_u16(FILE* fp);

bool read_file(const char *path, void *ptr, int from, size_t size);
bool write_file(const char* path, void* ptr, size_t size);
uint32_t file_size(const char* path, size_t from);
void create_directory(const char* directory);

#endif