#include "command_factory.h"
#include "concrete_command_class_selector.h"

static Command* from_command_line(CommandLine* line);
const struct command_factory CommandFactory = {
        .fromCommandLine = &from_command_line
};

Command* from_command_line(CommandLine* line)
{
    return ConcreteCommandClassSelector.fromCommandName(line->command)->fromArguments(line->arguments);
}
