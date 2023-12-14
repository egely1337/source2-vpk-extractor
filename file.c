#include <file.h>
#include <vpk.h>


/*
 *  Purpose: Returns first string from file
 *  fp      = file
 * */
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

/*
 *  Purpose: Reads 4 bytes from file and returns
 *  fp      = file
 * */
uint32_t read_u32(FILE* fp) {
    uint32_t ret = {0};
    fread(&ret, 4, 1, fp);
    return ret;
}

/*
 *  Purpose: Reads 2 bytes from file and returns
 *  fp      = file
 * */
uint16_t read_u16(FILE* fp) {
    uint16_t ret = {0};
    fread(&ret, 2, 1, fp);
    return ret;
}

/*
 *  Purpose: Reads file
 *  path    = file path
 *  ptr     = read from
 *  size    = how much bytes will be read
 *  from    = from where to read
 * */
bool read_file(const char *path, void *ptr, int from, size_t size)
{
    
    // Create file link
    FILE* fp = fopen(path, "r");

    // Return if syscall fails
    if(!fp) return false;

    // Roll the file
    fseek(fp, from , SEEK_SET);
    
    // Read file
    fread(ptr, size, 1, fp);
    
    // Close file
    fclose(fp);

    // Return true if reading success
    return true;
}

/*
 *  Purpose: Writes file
 *  path    = file path
 *  ptr     = write from
 *  size    = how much bytes will be writed
 * */
bool write_file(const char* path, void* ptr, size_t size) {
    // Create file link
    FILE* fp = fopen(path, "w");
    
    // Return if opening file failed
    if(!fp) return false;

    // Write file
    fwrite(ptr, size, 1, fp);

    // Close File
    fclose(fp);

    // Return true if writing success
    return true;
}

/*
 *  Purpose: Returns file size
 *  path    = const char*
 *  size_t  = from where
 * */
uint32_t file_size(const char* path, size_t from) {
    
    // Return stack
    uint32_t i = 0;
    
    // Create file link
    FILE* fp = fopen(path, "r");

    // Return if 
    if(!fp) return 0;

    // Roll file
    fseek(fp, from, SEEK_END);

    // Make i the file size
    i = ftell(fp);

    // Close file
    fclose(fp);

    // Return file
    return i;
}

/*
 *  Purpose: Create directory
 *  Source: https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c
 *  directory = directory string that wants to be created
 */
void create_directory(const char *directory)
{
    char *p = strdup(directory);
    char *sep = strchr(p+1, '/');
    while(sep != NULL)
    {
        *sep = '\0';
        mkdir(p, 0755);
        *sep = '/';
        sep = strchr(sep+1, '/');
    }
    free(p);
}
