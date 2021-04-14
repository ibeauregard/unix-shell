#ifndef VARIABLE_LIST
#define VARIABLE_LIST

#include "variable.h"

typedef struct variable_list VariableList;
struct variable_list {
    struct internals* _internals;
    void (*insert)(VariableList* this, Variable* variable);
    void(*print)(VariableList* this);
    void(*delete)(VariableList* this);
};

extern const struct variable_list_class {
    VariableList* (*new)();
} VariableListClass;

#endif
