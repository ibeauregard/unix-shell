#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Variable* from_string(char* string);
const struct variable_class VariableClass = {
        .fromString = &from_string
};

static struct internals* initialize_internals(char* variable);
static char* get_id(Variable* this);
static char* get_value(Variable* this);
static void set_value(Variable* this, char* value);
static void print(Variable* this);
static char* to_string(Variable* this);
static void delete(Variable* this);
Variable* from_string(char* string)
{
    struct internals* internals = initialize_internals(string);
    if (!internals) return NULL;
    Variable* this = malloc(sizeof (Variable));
    this->_internals = internals;
    this->getId = &get_id;
    this->getValue = &get_value;
    this->setValue = &set_value;
    this->print = &print;
    this->toString = &to_string;
    this->delete = &delete;
    return this;
}

struct internals {
    char* id;
    char* value;
};

struct internals* initialize_internals(char* variable)
{
    long equal_sign_index = strchr(variable, '=') - variable;
    if (equal_sign_index < 0) return NULL;
    struct internals* internals = malloc(sizeof (struct internals));
    internals->id = strndup(variable, equal_sign_index);
    internals->value = strdup(&variable[equal_sign_index + 1]);
    return internals;
}

char* get_id(Variable* this)
{
    return this->_internals->id;
}

char* get_value(Variable* this)
{
    return this->_internals->value;
}

void set_value(Variable* this, char* value)
{
    free(this->_internals->value);
    this->_internals->value = value;
}

void print(Variable* this)
{
    struct internals* internals = this->_internals;
    printf("%s=%s\n", internals->id, internals->value);
}

char* to_string(Variable* this)
{
    struct internals* internals = this->_internals;
    char* string = malloc(strlen(internals->id) + strlen(internals->value) + 2);
    strcpy(string, internals->id);
    strcat(string, "=");
    strcat(string, internals->value);
    return string;
}

void delete(Variable* this)
{
    struct internals* internals = this->_internals;
    free(internals->id);
    free(internals->value);
    free(internals);
    free(this); this = NULL;
}
