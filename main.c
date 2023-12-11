#include <stdint.h>
#include <stdlib.h>

#include <vpk.h>

const char* game_base_dir = "/mnt/d/SteamLibrary/steamapps/common/Counter-Strike Global Offensive/game/csgo";
const char* extract_path = "extract";

static void print_help(void) {
    return;
} 

int main(int argc, char** argv) {
    vpk_data_t vpk_file = read_vpk(game_base_dir);

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