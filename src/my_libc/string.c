#include "string.h"
#include <stdlib.h>

char* my_strchr(char* str, int chr) {
    for (; *str; str++) {
        if (*str == chr) return str;
    }
    return NULL;
}
