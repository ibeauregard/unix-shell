#include "variable_list.h"
#include <stdlib.h>

static VariableList* initialize();
static void insert(Variable* variable);
static void print();
static void delete();
VariableList variableList = {
        .initialize = &initialize,
        .insert = &insert,
        .print = &print,
        .delete = &delete
};

typedef struct variable_node VariableNode;
struct internals {
    VariableNode* head;
};

VariableList* initialize()
{
    variableList._internals = malloc(sizeof (struct internals));
    variableList._internals->head = NULL;
    return &variableList;
}

struct variable_node {
    Variable* variable;
    VariableNode* next;
};

void insert(Variable* variable)
{
    VariableNode* inserted = malloc(sizeof (VariableNode));
    inserted->variable = variable;
    inserted->next = variableList._internals->head;
    variableList._internals->head = inserted;
}

void print()
{
    VariableNode* node = variableList._internals->head;
    while (node) {
        node->variable->print(node->variable);
        node = node->next;
    }
}

void delete()
{
    VariableNode* node = variableList._internals->head;
    while (node) {
        VariableNode* next = node->next;
        node->variable->delete(node->variable);
        free(node);
        node = next;
    }
    free(variableList._internals);
}
