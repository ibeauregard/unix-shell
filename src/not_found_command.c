#include "not_found_command.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static Command* with_name(char* name);
const struct not_found_command NotFoundCommand = {
        .withName = &with_name
};

struct internals {
    char* name;
};

static void execute(Command* this);
Command* with_name(char* name)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->name = name;
    this->execute = &execute;
    return this;
}

static void delete(Command* this);
void execute(Command* this)
{
    dprintf(STDERR_FILENO, "my_zsh: command not found: %s\n", this->_internals->name);
    delete(this);
}

void delete(Command* this)
{
    free(this->_internals);
    free(this); this = NULL;
}

