#include "command_factory.h"
#include "null_command.h"
#include "echo_command.h"
#include "setenv_command.h"
#include "external_command.h"
#include <string.h>

static Command* from_command_line(CommandLine* line);
const struct command_factory CommandFactory = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* line)
{
    if (!line->command) {
        return NullCommand;
    } if (!strcmp(line->command, "echo")) {
        return EchoCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "setenv")) {
        return SetenvCommand.withArgs(line->arguments);
    } else {
        return ExternalCommand.fromCommandLine(line);
    }
}
