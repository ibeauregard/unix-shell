#include "string_list.h"
#include <stdlib.h>
#include <string.h>

static StringList* split(char* string, char sep);
const struct string_list_class StringListClass = {
        .split = &split
};

static StringList* new();
static void append(StringList* this, char* string);
StringList* split(char* string, char sep)
{
    StringList* this = new();
    while (*string) {
        for (; *string && *string == sep; string++);
        int j;
        for (j = 0; string[j] && string[j] != sep; ++j);
        if (j) append(this, strndup(string, j));
        string += j;
    }
    return this;
}

typedef struct string_node StringNode;
struct internals {
    StringNode* head;
    StringNode* tail;
};

static char* next(StringList* this);
static bool is_empty(StringList* this);
static void delete(StringList* this);
StringList* new()
{
    StringList* this = malloc(sizeof (StringList));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = this->_internals->tail = NULL;
    this->next = &next;
    this->isEmpty = &is_empty;
    this->delete = &delete;
    return this;
}

struct string_node {
    char* value;
    StringNode* next;
};

void append(StringList* this, char* string)
{
    StringNode* appended = malloc(sizeof (StringNode));
    appended->value = string; appended->next = NULL;
    struct internals* internals = this->_internals;
    if (is_empty(this)) {
        internals->head = internals->tail = appended;
    } else {
        internals->tail = internals->tail->next = appended;
    }
}

bool is_empty(StringList* this)
{
    return !this->_internals->head;
}

char* next(StringList* this)
{
    StringNode* current_head = this->_internals->head;
    StringNode* new_head = current_head->next;
    char* popped = current_head->value;
    free(current_head); current_head = NULL;
    this->_internals->head = new_head;
    if (!new_head) this->_internals->tail = NULL;
    return popped;
}

void delete(StringList* this)
{
    while (!is_empty(this)) {
        char* string = next(this);
        free(string);
    }
    free(this->_internals);
    free(this); this = NULL;
}
