#include "shell.h"
#include "command.h"
#include "command_factory.h"

static void execute(CommandLine* line);
static void delete();
struct shell shell = {
        .execute = &execute,
        .delete = &delete
};

void execute(CommandLine* line)
{
    Command* command = CommandFactory.fromCommandLine(line);
    command->execute(command);
    line->delete(line);
}

void delete()
{
    shell.environment->delete(shell.environment);
}
