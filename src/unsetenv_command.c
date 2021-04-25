#include "unsetenv_command.h"
#include "shell.h"
#include "environment.h"
#include <stdlib.h>

static Command* from_arguments(StringList* arguments);
const struct unsetenv_command UnsetenvCommand = {
        .fromArguments = &from_arguments
};

struct internals {
    StringList* variableIds;
};

static void execute(Command* this);
Command* from_arguments(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    free(arguments->next(arguments)); // discard command's name ("unsetenv")
    this->_internals->variableIds = arguments;
    this->execute = &execute;
    return this;
}

static void delete(Command** this);
void execute(Command* this)
{
    StringList* variableIds = this->_internals->variableIds;
    Environment* environment = shell.environment;
    while (!variableIds->isEmpty(variableIds)) {
        char* variable_id = variableIds->next(variableIds);
        environment->unsetVariable(environment, variable_id);
        free(variable_id);
    }
    delete(&this);
}

void delete(Command** this)
{
    free((*this)->_internals);
    free(*this); *this = NULL;
}
