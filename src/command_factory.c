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
        return NullCommand.fromArguments(line->arguments);
    } if (!strcmp(line->command, "echo")) {
        return EchoCommand.fromArguments(line->arguments);
    } if (!strcmp(line->command, "setenv")) {
        return SetenvCommand.fromArguments(line->arguments);
    } if (!strcmp(line->command, "unsetenv")) {
        return UnsetenvCommand.fromArguments(line->arguments);
    } if  (!strcmp(line->command, "cd")) {
        return CdCommand.fromArguments(line->arguments);
    } if (!strcmp(line->command, "env")) {
        return EnvCommand.fromArguments(line->arguments);
    } if (!strcmp(line->command, "exit")) {
        return ExitCommand.fromArguments(line->arguments);
    } else {
        return ExternalCommand.fromArguments(line->arguments);
    }
}
