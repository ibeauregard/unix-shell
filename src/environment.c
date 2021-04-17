#include "environment.h"
#include <stdlib.h>

static Environment* from_string_array(char* envp[]);
const struct environment_class EnvironmentClass = {
        .fromStringArray = &from_string_array
};

typedef struct variable_node VariableNode;
struct internals {
    VariableNode* head;
    size_t length;
};

static void insert(Environment* this, Variable* variable);
static void print(Environment* this);
static char** serialize(Environment* this);
static void delete(Environment* this);
Environment* from_string_array(char* envp[])
{
    Environment* this = malloc(sizeof (Environment));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = NULL;
    size_t i;
    for (i = 0; envp[i]; i++) {
        insert(this, VariableClass.fromString(envp[i]));
    }
    this->_internals->length = i;
    this->print = &print;
    this->serialize = &serialize;
    this->delete = &delete;
    return this;
}

struct variable_node {
    Variable* variable;
    VariableNode* next;
};

void insert(Environment* this, Variable* variable)
{
    VariableNode* inserted = malloc(sizeof (VariableNode));
    inserted->variable = variable;
    inserted->next = this->_internals->head;
    this->_internals->head = inserted;
}

void print(Environment* this)
{
    VariableNode* node = this->_internals->head;
    while (node) {
        node->variable->print(node->variable);
        node = node->next;
    }
}

char** serialize(Environment* this)
{
    char** serialized = malloc((this->_internals->length + 1) * sizeof (char*));
    VariableNode* node = this->_internals->head;
    size_t i;
    for (i = 0; node; ++i, node = node->next) {
        serialized[i] = node->variable->toString(node->variable);
    }
    serialized[i] = NULL;
    return serialized;
}

void delete(Environment* this)
{
    VariableNode* node = this->_internals->head;
    while (node) {
        VariableNode* next = node->next;
        node->variable->delete(node->variable);
        free(node);
        node = next;
    }
    free(this->_internals);
    free(this);
}
