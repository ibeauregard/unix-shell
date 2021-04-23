#include "string.h"
#include <string.h>
#include <ctype.h>

char* strstrip (char* string)
{
    while (isspace((unsigned char) *string)) string++;
    if (*string == 0) return string;
    char* end = string + strlen(string);
    while (isspace((unsigned char)end[-1])) end--;
    *end = 0;
    return string;
}

inline bool is_space(char c)
{
    return isspace((unsigned char) c);
}

inline bool is_colon(char c)
{
    return c == ':';
}
