#include "environment.h"
#include <stdlib.h>

static Environment* from_string_array(char* envp[]);
const struct environment_class EnvironmentClass = {
        .fromStringArray = &from_string_array
};

typedef struct variable_node VariableNode;
struct internals {
    VariableNode* head;
};

static void insert(Environment* this, Variable* variable);
static void print(Environment* this);
static void delete(Environment* this);
Environment* from_string_array(char* envp[])
{
    Environment* this = malloc(sizeof (Environment));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = NULL;
    for (int i = 0; envp[i]; i++) {
        insert(this, VariableClass.fromString(envp[i]));
    }
    this->print = &print;
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
