#include <vpk.h>
#include <file.h>

/*
 *  Purpose: Reads pak01_dir.vpk file and returns entire files with directories. 
 *
 */
vpk_data_t read_vpk(const char *path)
{
    // vpk header
    vpk_header_t header;

    // format vpk directory path
    char dir_path[VPK_MAX_PATH];
    sprintf(dir_path, "%s/pak01_dir.vpk", path);

    // Create file link
    FILE* fp = fopen(dir_path, "r");

    // Panic if file opening failed
    assert(fp && "File could not be open.");

    // Read header
    fread(&header, sizeof(header), 1, fp);

    // Parse vpk file and return it
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
    // If directory is null, Just return
    if(!strcmp(file->directory, "") || !strcmp(file->directory, " ")) return;
    
    // VPK Path
    char vpk_path[VPK_MAX_PATH] = {0};
    sprintf(&vpk_path, "%s/pak01_%03d.vpk", base_path, file->archive_index);

    // Extract Path 
    char ext_path[VPK_MAX_PATH] = {0};
    sprintf(&ext_path, "%s/%s/%s", extract_path, &file->directory[0], &file->path[0]);

    // Creating Directory String
    char directory[VPK_MAX_PATH] = {0};
    sprintf(&directory, "%s/%s/", extract_path, &file->directory[0]);
    create_directory(directory);

    // Allocate memory
    void* binary = (void*)malloc(file->lenght);

    // Read file
    if(!read_file(vpk_path, binary, file->cur, file->lenght)) {
        fprintf(stderr, "ERROR: While reading from file.\n");
    }

    // Write file
    if(!write_file(ext_path, binary, file->lenght)) {
        fprintf(stderr, "ERROR: While writing file\n");
    }

    free(binary);
}