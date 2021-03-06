#ifndef SHELL_H
#define SHELL_H

#include "environment.h"
#include "command_line.h"

extern struct shell {
    Environment* environment;
    void (*displayPrompt)(void);
    void (*execute)(CommandLine* commandLine);
    void (*delete)(void);
} shell;

#endif
