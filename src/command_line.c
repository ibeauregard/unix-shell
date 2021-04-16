#include "command_line.h"
#include <stdlib.h>
#include <string.h>

static CommandLine* from_string(char* string);
const struct command_line_class CommandLineClass = {
        .fromString = &from_string
};

static void delete(CommandLine* this);
CommandLine* from_string(char* string)
{
    CommandLine* this = malloc(sizeof (CommandLine));
    StringList* tokenList = StringListClass.split(string);
    this->command = tokenList->next(tokenList);
    this->arguments = tokenList;
    this->delete = &delete;
    return this;
}

void delete(CommandLine* this)
{
    free(this->command);
    this->arguments->delete(this->arguments);
    free(this); this = NULL;
}
