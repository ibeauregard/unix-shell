#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Variable* from_string(char* string);
static Variable* from_id_and_value(char* id, char* value);
const struct variable_class VariableClass = {
        .fromString = &from_string,
        .fromIdAndValue = &from_id_and_value
};

struct internals* get_internals_from_id_and_value(char* id, char* value);
static Variable* new(struct internals* internals);
Variable* from_string(char* string)
{
    long equal_sign_index = strchr(string, '=') - string;
    if (equal_sign_index < 0) return NULL;
    return new(get_internals_from_id_and_value(
            strndup(string, equal_sign_index),
            strdup(&string[equal_sign_index + 1])));
}


Variable* from_id_and_value(char* id, char* value)
{
    if (strchr(id, '=')) return NULL;
    return new(get_internals_from_id_and_value(strdup(id), strdup(value)));
}

struct internals {
    char* id;
    char* value;
};

struct internals* get_internals_from_id_and_value(char* id, char* value)
{
    struct internals* internals = malloc(sizeof (struct internals));
    internals->id = id;
    internals->value = value;
    return internals;
}

static char* get_id(Variable* this);
static char* get_value(Variable* this);
static void set_value(Variable* this, char* value);
static void print(Variable* this);
static char* to_string(Variable* this);
static void delete(Variable* this);
Variable* new(struct internals* internals)
{
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
    strcat(strcat(strcpy(string, internals->id), "="), internals->value);
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
