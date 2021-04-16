#include "command_factory.h"
#include "echo_command.h"
#include "not_found_command.h"
#include <string.h>

static Command* from_command_line(CommandLine* line);
const struct command_factory CommandFactory = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* line)
{
    if (!strcmp(line->command, "echo")) {
        return EchoCommand.withArgs(line->arguments);
    } else {
        return NotFoundCommand.withName(line->command);
    }
}
