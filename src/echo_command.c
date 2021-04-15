#include "echo_command.h"
#include <stdlib.h>
#include <stdio.h>

static Command* with_args(char** argArray);
const struct echo_command EchoCommand = {
        .withArgs = &with_args
};

struct internals {
    char** strings;
};

static void execute(Command* this);
Command* with_args(char** argArray)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->strings = argArray;
    this->execute = &execute;
    return this;
}

static void delete(Command* this);
void execute(Command* this)
{
    char** strings = this->_internals->strings;
    for (size_t i = 0; strings[i]; i++) {
        printf("%s ", strings[i]);
    }
    puts("");
    delete(this);
}

void delete(Command* this)
{
    char** strings = this->_internals->strings;
    for (size_t i = 0; strings[i]; i++) {
        free(strings[i]);
    }
    free(this->_internals->strings);
    free(this->_internals);
    free(this); this = NULL;
}
