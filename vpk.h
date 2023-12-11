#ifndef VPK_H
#define VPK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define VPK_HEADER_SIZE (uint32_t)32
#define VPK_MAX_PATH (uint32_t)256

#define SIGNATURE 0x55aa1234
#define VPK_VERSION_1 1
#define VPK_VERSION_2 2


typedef struct VPKHeader {
    uint32_t signature;
    uint32_t version;
    
    uint32_t tree_size;
    uint32_t unk1;
    uint32_t unk2;
    uint32_t unk3;
    uint32_t unk4;
} vpk_header_t;

typedef struct VPKFile {
    char path[VPK_MAX_PATH];
    char directory[VPK_MAX_PATH];
    uint16_t archive_index;
	uint32_t CRC;
	uint32_t cur;
	uint32_t lenght;
} vpk_file_t;

typedef struct VPKData {
    vpk_header_t header;
    vpk_file_t* files;
    uint32_t file_count;
} vpk_data_t;

vpk_data_t read_vpk(const char* path);
vpk_data_t parse_file(FILE* fp, vpk_header_t* header);
void extract_vpk(vpk_file_t* file, const char* extract_path, const char* base_path);

uint8_t* read_str(FILE* fp);
uint32_t read_u32(FILE* fp);
uint16_t read_u16(FILE* fp);
#endif