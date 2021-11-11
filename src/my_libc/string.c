#include "string.h"
#include <string.h>
#include <ctype.h>

char* strstrip (char* string)
{
    while (isspace(*string)) string++;
    if (*string == 0) return string;
    char* end = string + strlen(string);
    while (isspace(end[-1])) end--;
    *end = 0;
    return string;
}

inline bool is_space(char c)
{
    return isspace(c);
}

inline bool is_colon(char c)
{
    return c == ':';
}
