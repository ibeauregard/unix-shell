#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "string_list.h"

typedef struct command_line CommandLine;
struct command_line {
    char* command;
    StringList* arguments;
    void (*delete)(CommandLine** this);
};

extern const struct command_line_class {
    CommandLine* (*fromString)(char* string);
    CommandLine* (*fromStringList)(StringList* tokenList);
} CommandLineClass;

#endif
