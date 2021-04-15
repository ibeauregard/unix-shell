#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

typedef struct command_line CommandLine;
struct command_line {
    char* command;
    char** arguments;
    void (*delete)(CommandLine* this);
};

extern const struct command_line_class {
    CommandLine* (*new)();
} CommandLineClass;

#endif
