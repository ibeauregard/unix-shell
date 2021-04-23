#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <unistd.h>
#include <stdbool.h>

typedef struct string_list StringList;
struct string_list {
    struct internals* _internals;
    void (*append)(StringList* this, char* string);
    char* (*peek)(StringList* this);
    char* (*next)(StringList* this);
    bool (*isEmpty)(StringList* this);
    char** (*toStringArray)(StringList* this);
    StringList* (*copy)(StringList* this);
    void (*delete)(StringList** this);
};

typedef char* StringTransformation(char* string);
extern const struct string_list_class {
    StringList* (*new)();
    StringList* (*split)(char* string, char sep);
    StringList* (*splitTransform)(char* string, char sep, StringTransformation* transformation);
} StringListClass;

#endif
