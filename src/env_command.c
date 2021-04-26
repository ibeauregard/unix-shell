#include "env_command.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Command* from_arguments(StringList* arguments);
const ConcreteCommandClass EnvCommandClass = {
        .fromArguments = &from_arguments
};

static struct internals* new_internals(void);
static void parse_arguments(Command* this, StringList* arguments);
static void execute(Command* this);
Command* from_arguments(StringList* arguments)
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

struct internals* new_internals(void)
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
    free(arguments->next(arguments)); // discard "env" (command name)
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

static void do_execute(Command* this);
static void delete(Command** this);
void execute(Command* this)
{
    if (!this->_internals->parseError) do_execute(this);
    delete(&this);
}

static void process_u_argument(Command* this, char* argument, StringList* arguments);
void process_flag_argument(Command* this, StringList* arguments)
{
    char* argument = arguments->peek(arguments);
    bool u_argument_expected = false;
    size_t i;
    for (i = 1; !this->_internals->parseError && !u_argument_expected && argument[i]; i++) {
        if (argument[i] == 'i') this->_internals->ignoreExistingEnvironment = true;
        else if (argument[i] == 'u') {
            u_argument_expected = true;
        } else if (argument[i] != '-') {
            dprintf(STDERR_FILENO, "env: illegal option -- %c\n", argument[i]);
            this->_internals->parseError = true;
        }
    }
    if (!this->_internals->parseError && u_argument_expected) process_u_argument(this, &argument[i], arguments);
    if (!argument[1]) this->_internals->ignoreExistingEnvironment = true;
    if (!arguments->isEmpty(arguments)) free(arguments->next(arguments));
}

void process_setenv_argument(Command* this, char* argument)
{
    StringList* variablesToSet = this->_internals->variablesToSet;
    variablesToSet->append(variablesToSet, argument);
}

static void prepare_modified_environment(Command* this);
static void run_command_with_modified_environment(Command* this);
static void delete_modified_environment(void);
void do_execute(Command* this)
{
    Environment* existing_environment = shell.environment;
    prepare_modified_environment(this);
    run_command_with_modified_environment(this);
    delete_modified_environment();
    shell.environment = existing_environment;
}

static void process_separate_u_argument(Command* this, char* argument);
void process_u_argument(Command* this, char* argument, StringList* arguments)
{
    if (argument[0]) {
        this->_internals->variablesToUnset->append(this->_internals->variablesToUnset, strdup(argument));
    } else {
        free(arguments->next(arguments));
        if (arguments->isEmpty(arguments)) {
            dprintf(STDERR_FILENO, "%s\n", "env: option requires an argument -- u");
            this->_internals->parseError = true;
        } else {
            process_separate_u_argument(this, arguments->peek(arguments));
        }
    }
}

void prepare_modified_environment(Command* this)
{
    Environment* modified_environment =
        this->_internals->ignoreExistingEnvironment ?
            EnvironmentClass.new() :
            shell.environment->copy(shell.environment);
    modified_environment->unsetVariables(modified_environment, this->_internals->variablesToUnset);
    modified_environment->setVariables(modified_environment, this->_internals->variablesToSet, true);
    shell.environment = modified_environment;
}

void run_command_with_modified_environment(Command* this)
{
    StringList* commandTokens = this->_internals->command;
    if (commandTokens->isEmpty(commandTokens)) {
        shell.environment->print(shell.environment);
    } else {
        shell.execute(CommandLineClass.fromStringList(
                this->_internals->command->copy(this->_internals->command)));
    }
}

inline void delete_modified_environment(void)
{
    shell.environment->delete(&shell.environment);
}

void process_separate_u_argument(Command* this, char* argument)
{
    if (strchr(argument, '=')) {
        dprintf(STDERR_FILENO, "env: unsetenv %s: Invalid argument\n", argument);
        this->_internals->parseError = true;
    } else {
        this->_internals->variablesToUnset->append(this->_internals->variablesToUnset, strdup(argument));
    }
}

void delete(Command** this)
{
    struct internals* internals = (*this)->_internals;
    internals->variablesToUnset->delete(&internals->variablesToUnset);
    internals->variablesToSet->delete(&internals->variablesToSet);
    free(internals);
    free(*this); *this = NULL;
}
