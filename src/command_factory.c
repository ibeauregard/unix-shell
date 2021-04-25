#include "command_factory.h"
#include "null_command.h"
#include "echo_command.h"
#include "setenv_command.h"
#include "unsetenv_command.h"
#include "cd_command.h"
#include "env_command.h"
#include "exit_command.h"
#include "external_command.h"
#include <string.h>

static Command* from_command_line(CommandLine* line);
const struct command_factory CommandFactory = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* line)
{
    if (!line->command) {
        return NullCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "echo")) {
        return EchoCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "setenv")) {
        return SetenvCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "unsetenv")) {
        return UnsetenvCommand.withArgs(line->arguments);
    } if  (!strcmp(line->command, "cd")) {
        return CdCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "env")) {
        return EnvCommand.withArgs(line->arguments);
    } if (!strcmp(line->command, "exit")) {
        return ExitCommand.withArgs(line->arguments);
    } else {
        return ExternalCommand.fromCommandLine(line);
    }
}
