#include "echo_command.h"
#include <stdlib.h>
#include <stdio.h>

static Command* from_arguments(StringList* arguments);
const struct echo_command EchoCommand = {
        .fromArguments = &from_arguments
};

struct internals {
    StringList* arguments;
};

static void execute(Command* this);
Command* from_arguments(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    free(arguments->next(arguments)); // discard command's name ("echo")
    this->_internals->arguments = arguments;
    this->execute = &execute;
    return this;
}

static void delete(Command** this);
void execute(Command* this)
{
    StringList* arguments = this->_internals->arguments;
    while (!arguments->isEmpty(arguments)) {
        char* argument = arguments->next(arguments);
        printf("%s ", argument);
        free(argument);
    }
    puts("");
    delete(&this);
}

void delete(Command** this)
{
    free((*this)->_internals);
    free(*this); *this = NULL;
}
