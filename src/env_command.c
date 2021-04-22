#include "env_command.h"
#include <stdlib.h>

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
    bool ignoreExistingEnvironment;
    StringList* variablesToUnset;
    StringList* variablesToSet;
    char* command;
};

struct internals* new_internals()
{
    struct internals* internals = malloc(sizeof (struct internals));
    internals->ignoreExistingEnvironment = false;
    internals->variablesToUnset = StringListClass.new();
    internals->variablesToSet = StringListClass.new();
    internals->command = NULL;
    return internals;
}

void parse_arguments(Command* this, StringList* arguments)
{
    (void)this, (void)arguments;
}

static void delete(Command* this);
void execute(Command* this)
{
    delete(this);
}

void delete(Command* this)
{
    struct internals* internals = this->_internals;
    internals->variablesToUnset->delete(internals->variablesToUnset);
    internals->variablesToSet->delete(internals->variablesToSet);
    if (internals->command) free(internals->command);
    free(internals);
    free(this); this = NULL;
}
