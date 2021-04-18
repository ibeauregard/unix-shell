#include "string_list.h"
#include <stdlib.h>
#include <string.h>

static StringList* split(char* string, char sep, StringTransformation* transformation);
const struct string_list_class StringListClass = {
        .split = &split
};

static StringList* new();
static void append(StringList* this, char* string);
StringList* split(char* string, char sep, StringTransformation* transformation)
{
    StringList* this = new();
    while (*string) {
        for (; *string && *string == sep; string++);
        int j;
        for (j = 0; string[j] && string[j] != sep; ++j);
        if (j) append(this, transformation(strndup(string, j)));
        string += j;
    }
    return this;
}

typedef struct string_node StringNode;
struct internals {
    StringNode* head;
    StringNode* tail;
    size_t length;
};

static char* peek(StringList* this);
static char* next(StringList* this);
static bool is_empty(StringList* this);
static char** to_string_array(StringList* this);
static void delete(StringList* this);
StringList* new()
{
    StringList* this = malloc(sizeof (StringList));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = this->_internals->tail = NULL; this->_internals->length = 0;
    this->peek = &peek;
    this->next = &next;
    this->isEmpty = &is_empty;
    this->toStringArray = &to_string_array;
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
    this->_internals->length++;
}

bool is_empty(StringList* this)
{
    return !this->_internals->head;
}

char* peek(StringList* this)
{
    StringNode* head = this->_internals->head;
    return head ? head->value : NULL;
}

char* next(StringList* this)
{
    StringNode* current_head = this->_internals->head;
    StringNode* new_head = current_head->next;
    char* popped = current_head->value;
    free(current_head); current_head = NULL;
    this->_internals->head = new_head;
    if (!new_head) this->_internals->tail = NULL;
    this->_internals->length--;
    return popped;
}

char** to_string_array(StringList* this)
{
    char** array = malloc((this->_internals->length + 1) * sizeof (char*));
    size_t i;
    for (i = 0; !is_empty(this); i++) {
        array[i] = next(this);
    }
    array[i] = NULL;
    return array;
}

void delete(StringList* this)
{
    while (!is_empty(this)) {
        free(next(this));
    }
    free(this->_internals);
    free(this); this = NULL;
}
