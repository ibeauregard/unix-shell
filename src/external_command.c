#include "external_command.h"
#include "not_found_command.h"

static Command* from_command_line(CommandLine* commandLine);
const struct external_command ExternalCommand = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* commandLine)
{
    return NotFoundCommand.withName(commandLine->command);
}
