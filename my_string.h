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
size_t get_min_bracket_sequence_length(String string);

#endif // MY_STRING_H