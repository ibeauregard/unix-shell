#include "env_command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Command* with_args(StringList* arguments);
const struct env_command EnvCommand = {
        .withArgs = &with_args
};

static struct internals* new_internals();
static void parse_arguments(Command* this, StringList* arguments);
static void execute(Command* this);
Command* with_args(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = new_internals();
    parse_arguments(this, arguments);
    this->execute = &execute;
    return this;
}

struct internals {
    bool parseError;
    bool ignoreExistingEnvironment;
    StringList* variablesToUnset;
    StringList* variablesToSet;
    StringList* command;
};

struct internals* new_internals()
{
    struct internals* internals = malloc(sizeof (struct internals));
    internals->parseError = false;
    internals->ignoreExistingEnvironment = false;
    internals->variablesToUnset = StringListClass.new();
    internals->variablesToSet = StringListClass.new();
    internals->command = NULL;
    return internals;
}

static void process_flag_argument(Command* this, StringList* arguments);
static void process_setenv_argument(Command* this, char* argument);
void parse_arguments(Command* this, StringList* arguments)
{
    free(arguments->next(arguments)); // discard "env" (program name)
    while (!this->_internals->parseError
           && !arguments->isEmpty(arguments)
           && arguments->peek(arguments)[0] == '-') {
        process_flag_argument(this, arguments);
    }
    while (!this->_internals->parseError
            && !arguments->isEmpty(arguments)
            && strchr(arguments->peek(arguments), '=')) {
        process_setenv_argument(this, arguments->next(arguments));
    }
    this->_internals->command = arguments;
}

static void delete(Command* this);
void execute(Command* this)
{
    delete(this);
}

void process_flag_argument(Command* this, StringList* arguments)
{
    char* argument = arguments->peek(arguments);
    bool u_argument_expected = false;
    size_t i;
    for (i = 1; !this->_internals->parseError && !u_argument_expected && argument[i]; i++) {
        if (argument[i] == 'i') this->_internals->ignoreExistingEnvironment = true;
        else if (argument[i] == 'u') {
            u_argument_expected = true;
        } else {
            dprintf(STDERR_FILENO, "env: illegal option -- %c\n", argument[i]);
            this->_internals->parseError = true;
        }
    }
    StringList* variablesToUnset = this->_internals->variablesToUnset;
    if (!this->_internals->parseError && u_argument_expected) {
        if (argument[i]) {
            variablesToUnset->append(variablesToUnset, strdup(&argument[i]));
        } else {
            free(arguments->next(arguments));
            if (arguments->isEmpty(arguments)) {
                dprintf(STDERR_FILENO, "%s\n", "env: option requires an argument -- u");
                this->_internals->parseError = true;
                return;
            } else {
                argument = arguments->peek(arguments);
                if (strchr(argument, '=')) {
                    dprintf(STDERR_FILENO, "env: unsetenv %s: Invalid argument\n", argument);
                    this->_internals->parseError = true;
                } else {
                    variablesToUnset->append(variablesToUnset, strdup(argument));
                }
            }
        }
    }
    if (!argument[1]) this->_internals->ignoreExistingEnvironment = true;
    free(arguments->next(arguments));
}

void process_setenv_argument(Command* this, char* argument)
{
    StringList* variablesToSet = this->_internals->variablesToSet;
    variablesToSet->append(variablesToSet, argument);
}

void delete(Command* this)
{
    struct internals* internals = this->_internals;
    internals->variablesToUnset->delete(internals->variablesToUnset);
    internals->variablesToSet->delete(internals->variablesToSet);
    free(internals);
    free(this); this = NULL;
}
