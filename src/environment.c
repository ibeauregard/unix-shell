#include "environment.h"
#include "variable_list.h"
#include <stdlib.h>

static void parse(char* env[]);
static void print();
static void delete();
Environment environment = {
        .parse = &parse,
        .print = &print,
        .delete = &delete
};

static void initialize();
static void add_variable(char* variable);
void parse(char* env[])
{
    initialize();
    for (int i = 0; env[i]; i++) {
        add_variable(env[i]);
    }
}

struct internals {
    VariableList* variableList;
};

void initialize()
{
    environment._internals = malloc(sizeof (struct internals));
    environment._internals->variableList = VariableListClass.new();
}

void add_variable(char* variable)
{
    VariableList *list = environment._internals->variableList;
    list->insert(list, VariableClass.fromString(variable));
}

void print()
{
    VariableList *list = environment._internals->variableList;
    list->print(list);
}

void delete()
{
    struct internals* internals = environment._internals;
    internals->variableList->delete(internals->variableList);
    free(internals); internals = NULL;
}
