#include "string_list.h"
#include <stdlib.h>
#include <string.h>

static StringList* new();
static StringList* split(char* string, CharTest* is_separator);
static StringList* split_transform(char* string, CharTest* is_separator, StringTransformation* transform);
const struct string_list_class StringListClass = {
        .new = &new,
        .split = &split,
        .splitTransform = &split_transform
};

static char* null_transformation(char* string);
StringList* split(char* string, CharTest* is_separator)
{
    return split_transform(string, is_separator, &null_transformation);
}

static void append(StringList* this, char* string);
StringList* split_transform(char* string, CharTest* is_separator, StringTransformation* transform)
{
    StringList* this = new();
    while (*string) {
        for (; *string && is_separator(*string); string++);
        int j;
        for (j = 0; string[j] && !is_separator(string[j]); ++j);
        if (j) append(this, transform(strndup(string, j)));
        string += j;
    }
    return this;
}

inline char* null_transformation(char* string)
{
    return string;
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
static StringList* deep_copy(StringList* this);
static void delete(StringList** this);
StringList* new()
{
    StringList* this = malloc(sizeof (StringList));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = this->_internals->tail = NULL; this->_internals->length = 0;
    this->append = &append;
    this->peek = &peek;
    this->next = &next;
    this->isEmpty = &is_empty;
    this->toStringArray = &to_string_array;
    this->copy = &deep_copy;
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
    free(current_head); this->_internals->head = NULL;
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

StringList* deep_copy(StringList* this)
{
    StringList* copy = new();
    StringNode* node = this->_internals->head;
    while (node) {
        append(copy, strdup(node->value));
        node = node->next;
    }
    return copy;
}

void delete(StringList** this)
{
    while (!is_empty(*this)) {
        free(next(*this));
    }
    free((*this)->_internals);
    free(*this); *this = NULL;
}
