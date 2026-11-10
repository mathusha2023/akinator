#include "files.h"
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include "log.h"
#include "my_string.h"

// -1 в случае ошибки
off_t get_file_size(const char *filename)
{
    assert(filename);

    struct stat stat_struct = {};

    if (stat(filename, &stat_struct) == -1)
    {
        int error = errno;
        FLOG("Cannot get stat from file %s (err_code = %d)\n", filename, error);
        return -1;
    };

    return stat_struct.st_size;
}

// не 0 в случае ошибки
int read_file_to_string(const char *filename, String *string)
{
    assert(filename);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        FLOG("Error: cant open file '%s' to read data from", filename);
        return 1;
    }

    off_t filesize = get_file_size(filename);
    if (filesize <= 0)
    {
        FLOG("Error: cant get file '%s' size", filename);
        return 1;
    }

    char *buffer = (char *)calloc((size_t)filesize + 1, sizeof(char));
    if (!buffer)
    {
        int error = errno;
        FLOG("Can not allocate memory for buffer (%d)\n", error);
        return error;
    }

    size_t read_size = fread((void *)buffer, sizeof(char), (size_t)filesize, file);
    if (read_size < (size_t)filesize && ferror(file))
    {
        FLOG("Error while reading file '%s'", filename);
        return 1;
    }
    buffer[read_size] = '\0';

    size_t curr = read_size - 1;
    while (curr && isspace(buffer[curr]))
        curr--;

    String new_string = {.p = buffer, .length = curr + 1};
    *string = new_string;

    return 0;
}