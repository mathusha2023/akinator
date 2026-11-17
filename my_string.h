#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

typedef struct String
{
    char *p;
    size_t length;
} String;

char *copy_string_to_char_arr(String string);
char *stringchr(String string, char c);

#endif // MY_STRING_H