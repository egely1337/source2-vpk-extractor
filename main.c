#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <vpk.h>


// Implement
static void print_help(void) {
    fprintf(stdout,
        "VPK Exporter Help:\n" 
        "--input <game_base_directory>\n"
        "--extract <extract_path>\n"
    );

    exit(EXIT_SUCCESS);
} 

int main(int argc, char** argv) {
    if(argc < 2) print_help();
    char game_base_dir[VPK_MAX_PATH], extract_path[VPK_MAX_PATH];

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "--input")) {
            strncpy(game_base_dir, argv[i+1], VPK_MAX_PATH);
            i++;
        }

        if(!strcmp(argv[i], "--export")) {
            strncpy(extract_path, argv[i+1], VPK_MAX_PATH);
            i++;
        }
    }

    assert((strlen(game_base_dir) > 1 && strlen(extract_path) > 1) && "You should pass variables extract_path and game_base_dir");

    vpk_data_t vpk_file = read_vpk(game_base_dir);

    // I dont know why for some reason program does not work when using 0 as index.
    // So don't touch if it works.
    for(int i = 2; i < vpk_file.file_count; i++) {
        fprintf(stdout, 
            "Filename: %s | "
            "Offset: %d | "
            "Lenght: %d | "
            "Archive: %d\n", vpk_file.files[i].path, vpk_file.files[i].cur, vpk_file.files[i].lenght, vpk_file.files[i].archive_index
        );

        extract_vpk(&vpk_file.files[i], extract_path, game_base_dir);
    }
    
    printf("Total Files: %d\n", vpk_file.file_count);

    return 0;
}