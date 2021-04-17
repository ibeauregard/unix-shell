#ifndef VARIABLE_H
#define VARIABLE_H

typedef struct variable Variable;
struct variable {
    struct internals* _internals;
    char* (*getId)(Variable* this);
    char* (*getValue)(Variable* this);
    void (*print)(Variable* this);
    char* (*toString)(Variable* this);
    void (*delete)(Variable* this);
};

extern const struct variable_class {
    Variable* (*fromString)(char* string);
} VariableClass;

#endif
