#ifndef FILES_H
#define FILES_H

#include <sys/types.h>
#include "my_string.h"

off_t get_file_size(const char *filename);
int read_file_to_string(const char *filename, String *string);

#endif // FILES_H
