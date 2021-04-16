#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <unistd.h>
#include <stdbool.h>

typedef struct string_list StringList;
struct string_list {
    struct internals* _internals;
    char* (*next)(StringList* this);
    bool (*isEmpty)(StringList* this);
    void (*delete)(StringList* this);
};

extern const struct string_list_class {
    StringList* (*split)(char* string);
} StringListClass;

#endif