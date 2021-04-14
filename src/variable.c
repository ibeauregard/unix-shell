#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Variable* from_string(char* string);
const struct variable_class VariableClass = {
        .fromString = &from_string
};

static struct internals* initialize_internals(char* variable);
static void print(Variable* this);
static void delete(Variable* this);
Variable* from_string(char* string)
{
    Variable* this = malloc(sizeof (Variable));
    this->_internals = initialize_internals(string);
    this->print = &print;
    this->delete = &delete;
    return this;
}

struct internals {
    char* id;
    char* value;
};

struct internals* initialize_internals(char* variable)
{
    struct internals* internals = malloc(sizeof (struct internals));
    long equal_sign_index = strchr(variable, '=') - variable;
    internals->id = strndup(variable, equal_sign_index);
    internals->value = strdup(&variable[equal_sign_index + 1]);
    return internals;
}

void print(Variable* this)
{
    struct internals* internals = this->_internals;
    printf("%s=%s\n", internals->id, internals->value);
}

void delete(Variable* this)
{
    struct internals* internals = this->_internals;
    free(internals->id);
    free(internals->value);
    free(internals);
    free(this); this = NULL;
}
