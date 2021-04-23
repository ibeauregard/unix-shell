#include "environment.h"
#include <stdlib.h>
#include <string.h>

static Environment* new();
static Environment* from_string_array(char* envp[]);
const struct environment_class EnvironmentClass = {
        .new = &new,
        .fromStringArray = &from_string_array
};

typedef struct variable_node VariableNode;
struct internals {
    VariableNode* head;
};

static char* get_value_from_id(Environment* this, char* id);
static void set_variable(Environment* this, Variable* variable, bool overwrite);
static void unset_variable(Environment* this, char* id);
static void unset_variables(Environment* this, StringList* ids);
static void print(Environment* this);
static char** serialize(Environment* this);
static Environment* copy(Environment* this);
static void delete(Environment** this);
Environment* new()
{
    Environment* this = malloc(sizeof (Environment));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->head = NULL;
    this->getValueFromId = &get_value_from_id;
    this->setVariable = &set_variable;
    this->unsetVariable = &unset_variable;
    this->unsetVariables = &unset_variables;
    this->print = &print;
    this->serialize = &serialize;
    this->copy = &copy;
    this->delete = &delete;
    return this;
}

static void insert(Environment* this, Variable* variable);
Environment* from_string_array(char* envp[])
{
    Environment* this = new();
    for (size_t i = 0; envp[i]; i++) {
        insert(this, VariableClass.fromString(envp[i]));
    }
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

char* get_value_from_id(Environment* this, char* id)
{
    VariableNode* node = this->_internals->head;
    while (node && strcmp(id, node->variable->getId(node->variable)) != 0) {
        node = node->next;
    }
    return strdup(node ? node->variable->getValue(node->variable) : "");
}

void set_variable(Environment* this, Variable* variable, bool overwrite)
{
    if (!variable) return;
    bool seen = false;
    VariableNode* node = this->_internals->head;
    while (!seen && node) {
        Variable* existingVariable = node->variable;
        if (!strcmp(existingVariable->getId(existingVariable), variable->getId(variable))) {
            if (overwrite) existingVariable->setValue(existingVariable, strdup(variable->getValue(variable)));
            seen = true;
        }
        node = node->next;
    }
    if (!seen) {
        insert(this, variable);
    } else {
        variable->delete(&variable);
    }
}

void unset_variable(Environment* this, char* id)
{
    VariableNode* node = this->_internals->head;
    if (!node) return;
    if (!strcmp(id, node->variable->getId(node->variable))) {
        this->_internals->head = node->next;
        node->variable->delete(&node->variable);
        free(node);
        return;
    }
    while (node->next && strcmp(id, node->next->variable->getId(node->next->variable)) != 0) {
        node = node->next;
    }
    if (node->next) {
        VariableNode* deleted = node->next;
        node->next = node->next->next;
        deleted->variable->delete(&deleted->variable);
        free(deleted);
    }
}

void unset_variables(Environment* this, StringList* ids)
{
    while (!ids->isEmpty(ids)) {
        char* id = ids->next(ids);
        this->unsetVariable(this, id);
        free(id);
    }
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
    size_t length;
    VariableNode* node = this->_internals->head;
    for (length = 0; node; node = node->next, length++);
    char** serialized = malloc((length + 1) * sizeof (char*));
    size_t i;
    for (i = 0, node = this->_internals->head; node; node = node->next, i++) {
        serialized[i] = node->variable->toString(node->variable);
    }
    serialized[i] = NULL;
    return serialized;
}

Environment* copy(Environment* this)
{
    Environment* copy = new();
    VariableNode* node = this->_internals->head;
    while (node) {
        insert(copy, node->variable->copy(node->variable));
        node = node->next;
    }
    return copy;
}

void delete(Environment** this)
{
    VariableNode* node = (*this)->_internals->head;
    while (node) {
        VariableNode* next = node->next;
        node->variable->delete(&node->variable);
        free(node);
        node = next;
    }
    free((*this)->_internals);
    free(*this); *this = NULL;
}
