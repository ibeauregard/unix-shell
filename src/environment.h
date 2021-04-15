#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "variable.h"

typedef struct environment Environment;
struct environment {
    struct internals* _internals;
    void(*print)(Environment* this);
    void(*delete)(Environment* this);
};

extern const struct environment_class {
    Environment* (*fromStringArray)(char* envp[]);
} EnvironmentClass;

#endif
