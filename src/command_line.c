#include "command_line.h"
#include <stdlib.h>
#include <string.h>

static CommandLine* new();
const struct command_line_class CommandLineClass = {
        .new = &new
};

static void delete(CommandLine* this);
CommandLine* new()
{
    CommandLine* this = malloc(sizeof (CommandLine));
    this->command = strdup("echo");
    this->arguments = malloc(3 * sizeof (char*));
    this->arguments[0] = strdup("roger");
    this->arguments[1] = strdup("sawyer");
    this->arguments[2] = NULL;
    this->delete = &delete;
    return this;
}

void delete(CommandLine* this)
{
    free(this->command);
    for (size_t i = 0; this->arguments[i]; i++) {
        free(this->arguments[i]);
    }
    free(this); this = NULL;
}
