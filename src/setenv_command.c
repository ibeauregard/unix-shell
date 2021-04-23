#include "setenv_command.h"
#include "variable.h"
#include "shell.h"
#include "environment.h"
#include <stdlib.h>
#include <string.h>

static Command* with_args(StringList* arguments);
const struct setenv_command SetenvCommand = {
        .withArgs = &with_args
};

struct internals {
    bool overwrite;
    StringList* variables;
};

static struct internals* initialize_internals(StringList* arguments);
static void execute(Command* this);
Command* with_args(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = initialize_internals(arguments);
    this->execute = &execute;
    return this;
}

struct internals* initialize_internals(StringList* arguments)
{
    free(arguments->next(arguments));
    struct internals* internals = malloc(sizeof (struct internals));
    internals->overwrite = false;
    if (!arguments->isEmpty(arguments) && !strcmp(arguments->peek(arguments), "-o")) {
        internals->overwrite = true;
        free(arguments->next(arguments));
    }
    internals->variables = arguments;
    return internals;
}

static void delete(Command** this);
void execute(Command* this)
{
    StringList* variables = this->_internals->variables;
    bool overwrite = this->_internals->overwrite;
    Environment* environment = shell.environment;
    while (!variables->isEmpty(variables)) {
        char* variable_string = variables->next(variables);
        Variable* variable = VariableClass.fromString(variable_string);
        free(variable_string);
        if (variable) environment->setVariable(environment, variable, overwrite);
    }
    delete(&this);
}

void delete(Command** this)
{
    free((*this)->_internals);
    free(*this); *this = NULL;
}
