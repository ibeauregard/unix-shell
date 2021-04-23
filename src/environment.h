#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "variable.h"
#include "string_list.h"
#include <stdbool.h>

typedef struct environment Environment;
struct environment {
    struct internals* _internals;
    char* (*getValueFromId)(Environment* this, char* id);
    void (*setVariable)(Environment* this, Variable* variable, bool overwrite);
    void (*unsetVariable)(Environment* this, char* variable_id);
    void (*unsetVariables)(Environment* this, StringList* variable_ids);
    void (*print)(Environment* this);
    char** (*serialize)(Environment* this);
    Environment* (*copy)(Environment* this);
    void(*delete)(Environment** this);
};

extern const struct environment_class {
    Environment* (*new)();
    Environment* (*fromStringArray)(char* envp[]);
} EnvironmentClass;

#endif
