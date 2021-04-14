#include "variable_list.h"
#include <stdlib.h>

static VariableList* new();
const struct variable_list_class VariableListClass = {
        .new = &new
};

typedef struct variable_node VariableNode;
struct internals {
    VariableNode* head;
};

static void insert(VariableList* this, Variable* variable);
static void print(VariableList* this);
static void delete(VariableList* this);
VariableList* new()
{
    VariableList* this = malloc(sizeof (VariableList));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = NULL;
    this->insert = &insert;
    this->print = &print;
    this->delete = &delete;
    return this;
}

struct variable_node {
    Variable* variable;
    VariableNode* next;
};

void insert(VariableList* this, Variable* variable)
{
    VariableNode* inserted = malloc(sizeof (VariableNode));
    inserted->variable = variable;
    inserted->next = this->_internals->head;
    this->_internals->head = inserted;
}

void print(VariableList* this)
{
    VariableNode* node = this->_internals->head;
    while (node) {
        node->variable->print(node->variable);
        node = node->next;
    }
}

void delete(VariableList* this)
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
