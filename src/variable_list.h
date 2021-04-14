#ifndef VARIABLE_LIST
#define VARIABLE_LIST

#include "variable.h"

typedef struct variable_list VariableList;
struct variable_list {
    struct internals* _internals;
    VariableList* (*initialize)();
    void (*insert)(Variable* variable);
    void(*print)();
    void(*delete)();
};

extern VariableList variableList;

#endif
