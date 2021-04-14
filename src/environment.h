#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

typedef struct environment {
    struct internals* _internals;
    void (*parse)(char* env[]);
    void (*print)();
    void (*delete)();
} Environment;

extern Environment environment;

#endif
