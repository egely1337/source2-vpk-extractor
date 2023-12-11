#include <vpk.h>
#include <assert.h>

/*
 *  Purpose: Reads pak01_dir.vpk file and returns entire files with directories. 
 *
 */
vpk_data_t read_vpk(const char *path)
{
    char dir_path[VPK_MAX_PATH];
    sprintf(dir_path, "%s/pak01_dir.vpk", path);

    vpk_header_t header;
    
    FILE* fp = fopen(dir_path, "r");

    assert(fp && "File could not be open.");

    fread(&header, sizeof(header), 1, fp);

    return parse_file(fp, &header);
}

/*
 *  Purpose: Parses file and returns a vpk_data_t object. 
 *
 */
vpk_data_t parse_file(FILE* fp, vpk_header_t* header) {
    int i = 1;

    vpk_file_t* arr = (vpk_file_t*)malloc(sizeof(vpk_file_t) * i);
    
    while(1) {
        char* ext = read_str(fp);
        if(!strcmp(ext, "")) break;
        while(1) {
            char* folder = read_str(fp);
            if(!strcmp(folder, "")) break;

            while(1) {
                char* file_name = read_str(fp);
                if(!strcmp(file_name, "")) break;

                uint32_t crc = read_u32(fp); // crc
                uint16_t preload_bytes = read_u16(fp); // preload bytes
                uint16_t archive_index = read_u16(fp); // archive index
                uint32_t offset = read_u32(fp); // file offset
                uint32_t lenght = read_u32(fp); // file lenght
                read_u16(fp); // terminator

                vpk_file_t vpk_file = {
                    .archive_index = archive_index,
                    .CRC = crc,
                    .lenght = lenght,
                    .cur = offset,
                };
                
                sprintf(&vpk_file.path,"%s.%s", file_name, ext);
                strcpy(vpk_file.directory, folder);
                i = i + 1;

                arr = realloc(arr, sizeof(vpk_file_t)*(i+10));
                memcpy(&arr[i], &vpk_file, sizeof(vpk_file_t));


                // freed up the temporary thingz
                free(file_name);
            }

            // same as above
            free(folder);
        }

        // same as above
        free(ext);
    }

    return (vpk_data_t){
        .file_count = i,
        .files = arr,
        .header = header
    };
}

/*
 *  Purpose: Extracts file from .vpk and creates a file in extract/*
 *
 */
void extract_vpk(vpk_file_t* file, const char* extract_path, const char* base_path) {

    assert(file && "file should be pointer");
    char vpk_path[VPK_MAX_PATH] = {0};
    sprintf(&vpk_path, "%s/pak01_%03d.vpk", base_path, file->archive_index);

    // If directory is null, Just return
    if(!strcmp(file->directory, "") || !strcmp(file->directory, " ")) return;

    char ext_path[VPK_MAX_PATH] = {0};
    sprintf(&ext_path, "%s/%s/%s", extract_path, &file->directory[0], &file->path[0]);

    char command[VPK_MAX_PATH] = {0};
    sprintf(&command, "%s/%s", extract_path, &file->directory);
    mkdir(command, 0777);

    void* binary = (void*)malloc(file->lenght);
    FILE* fp = fopen(vpk_path, "r");
    if(!fp) {
        fprintf(stderr, "ERROR: While opening %s\n", vpk_path);
        return;
    };
    fseek(fp, file->cur , SEEK_SET);
    fread(binary, file->lenght, 1, fp);
    fclose(fp);

    fp = fopen(ext_path, "w");
    if(!fp) return;
    fwrite(binary, file->lenght, 1, fp);
    fclose(fp);

    free(binary);
}



uint8_t* read_str(FILE* fp) {
    char *str = (void*)calloc(sizeof(char), VPK_MAX_PATH);
    int i = 0;
    while(!feof(fp) && i < 256) {
        char ch = fgetc(fp);
        if(ch != 0) str[i] = ch;
        else break;
        i++;
    }

    return str;
}

uint32_t read_u32(FILE* fp) {
    uint32_t ret = {0};
    fread(&ret, 4, 1, fp);
    return ret;
}

uint16_t read_u16(FILE* fp) {
    uint16_t ret = {0};
    fread(&ret, 2, 1, fp);
    return ret;
}