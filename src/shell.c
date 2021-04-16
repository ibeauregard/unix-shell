#include "shell.h"
#include "command.h"
#include "echo_command.h"
#include "not_found_command.h"
#include <string.h>

static void execute(CommandLine* line);
static void delete();
struct shell shell = {
        .execute = &execute,
        .delete = &delete
};

void execute(CommandLine* line)
{
    Command* command;
    if (!strcmp(line->command, "echo")) {
        command = EchoCommand.withArgs(line->arguments);
    } else {
        command = NotFoundCommand.withName(line->command);
    }
    command->execute(command);
    line->delete(line);
}

void delete()
{
    shell.environment->delete(shell.environment);
}
