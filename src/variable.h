#ifndef VARIABLE_H
#define VARIABLE_H

typedef struct variable Variable;
struct variable {
    struct internals* _internals;
    void (*print)(Variable* this);
    void (*delete)(Variable* this);
};

extern const struct variable_class {
    Variable* (*fromString)(char* string);
} VariableClass;

#endif
