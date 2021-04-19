#include "cd_command.h"
#include <stdlib.h>
#include <stdio.h>

static Command* with_args(StringList* arguments);
const struct cd_command CdCommand = {
        .withArgs = &with_args
};

static void initialize_internals(Command* this);
static void parse_arguments(Command* this, StringList* arguments);
static void execute(Command* this);
Command* with_args(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    initialize_internals(this);
    parse_arguments(this, arguments);
    this->execute = &execute;
    return this;
}

struct internals {
    bool pOption;
    char* directory;
    bool error;
};

void initialize_internals(Command* this)
{
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->pOption = false;
    this->_internals->directory = NULL;
    this->_internals->error = false;
}

static void handle_flag_argument(Command* this, char* argument);
void parse_arguments(Command* this, StringList* arguments)
{
    free(arguments->next(arguments)); // discard 'cd' argument (program name)
    while (!this->_internals->error
            && !arguments->isEmpty(arguments)
            && arguments->peek(arguments)[0] == '-') {
        handle_flag_argument(this, arguments->next(arguments));
    }
    if (!this->_internals->error && !arguments->isEmpty(arguments)) {
        this->_internals->directory = arguments->next(arguments);
    }
    if (!this->_internals->error && !arguments->isEmpty(arguments)) {
        this->_internals->error = true;
        dprintf(STDERR_FILENO, "%s\n", "cd: Too many arguments");
        while (!arguments->isEmpty(arguments)) free(arguments->next(arguments));
    }
}

void handle_flag_argument(Command* this, char* argument)
{
    for (size_t i = 1; !this->_internals->error && argument[i]; i++) {
        if (argument[i] == 'L') this->_internals->pOption = false;
        else if (argument[i] == 'P') this->_internals->pOption = true;
        else {
            dprintf(STDERR_FILENO, "cd: Illegal option -%c\n", argument[i]);
            this->_internals->error = true;
        }
    }
    free(argument);
}

static void delete(Command* this);
void execute(Command* this)
{
    delete(this);
}

void delete(Command* this)
{
    free(this->_internals->directory);
    free(this->_internals);
    free(this); this = NULL;
}
