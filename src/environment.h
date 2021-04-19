#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "variable.h"
#include <stdbool.h>

typedef struct environment Environment;
struct environment {
    struct internals* _internals;
    char* (*getValueFromId)(Environment* this, char* id);
    void (*setVariable)(Environment* this, Variable* variable, bool overwrite);
    void (*unsetVariable)(Environment* this, char* variable_id);
    void (*print)(Environment* this);
    char** (*serialize)(Environment* this);
    void(*delete)(Environment* this);
};

extern const struct environment_class {
    Environment* (*fromStringArray)(char* envp[]);
} EnvironmentClass;

#endif
