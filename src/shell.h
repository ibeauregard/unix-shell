#ifndef SHELL_H
#define SHELL_H

#include "environment.h"

extern struct shell {
    Environment* environment;
    void (*delete)();
} shell;

#endif
