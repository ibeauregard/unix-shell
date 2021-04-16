#include "external_command.h"
#include "not_found_command.h"
#include <stdlib.h>
#include <stdio.h>

static Command* from_command_line(CommandLine* commandLine);
const struct external_command ExternalCommand = {
        .fromCommandLine = &from_command_line
};

struct internals {
    char* pathname;
    char** args_array;
};

static void execute(Command* this);
Command* from_command_line(CommandLine* commandLine)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->pathname = commandLine->command;
    StringList* arguments = commandLine->arguments;
    this->_internals->args_array = arguments->toStringArray(arguments);
    this->execute = &execute;
    return this;
}

static void delete(Command* this);
void execute(Command* this)
{
    for (size_t i = 0; this->_internals->args_array[i]; i++) {
        printf("%s\n", this->_internals->args_array[i]);
    }
    delete(this);
}

void delete(Command* this)
{
    char** args = this->_internals->args_array;
    for (size_t i = 0; args[i]; i++) {
        free(args[i]);
    }
    free(args);
    free(this->_internals);
    free(this); this = NULL;
}
