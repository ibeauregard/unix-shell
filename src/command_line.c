#include "command_line.h"
#include "shell.h"
#include "environment.h"
#include <stdlib.h>
#include <string.h>

static CommandLine* from_string(char* string);
const struct command_line_class CommandLineClass = {
        .fromString = &from_string
};

static char* possibly_replace_with_env_var(char* string);
static void delete(CommandLine* this);
CommandLine* from_string(char* string)
{
    CommandLine* this = malloc(sizeof (CommandLine));
    StringList* tokenList = StringListClass.splitTransform(string, ' ', &possibly_replace_with_env_var);
    this->command = tokenList->peek(tokenList);
    this->arguments = tokenList;
    this->delete = &delete;
    return this;
}

char* possibly_replace_with_env_var(char* string)
{
    if (!string || strlen(string) == 0 || string[0] != '$') {
        return string;
    }
    Environment* environment = shell.environment;
    char* value = environment->getValueFromId(environment, string + 1);
    free(string);
    return value;
}

void delete(CommandLine* this)
{
    this->arguments->delete(this->arguments);
    free(this); this = NULL;
}
