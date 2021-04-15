#include "command_factory.h"
#include "echo_command.h"
#include <string.h>
#include <stdlib.h>

static Command* from_command_line(CommandLine* line);
const struct command_factory CommandFactory = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* line)
{
    Command* command;
    if (!strcmp(line->command, "echo")) {
        command = EchoCommand.withArgs(line->arguments);
    } else {
        command = NULL;
    }
    free(line->command);
    free(line); line = NULL;
    return command;
}
