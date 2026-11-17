#include "my_string.h"
#include <stdlib.h>
#include <assert.h>

char *copy_string_to_char_arr(String string)
{
    assert(string.p);

    char *p = (char *)calloc(string.length + 1, sizeof(char));
    if (!p)
    {
        return NULL;
    }

    size_t i = 0;
    for (i = 0; i < string.length; i++)
    {
        p[i] = string.p[i];
    }
    p[i] = '\0';

    return p;
}

char *stringchr(String string, char c)
{
    assert(string.p);

    for (size_t i = 0; i < string.length; i++)
    {
        if (string.p[i] == c)
            return string.p + i;
    }
    return NULL;
}
