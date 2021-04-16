#include "shell.h"
#include "command.h"
#include "echo_command.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static void execute(CommandLine* line);
static void delete();
struct shell shell = {
        .execute = &execute,
        .delete = &delete
};

void execute(CommandLine* line)
{
    Command* command;
    bool failure = false;
    if (!strcmp(line->command, "echo")) {
        command = EchoCommand.withArgs(line->arguments);
    } else {
        dprintf(STDERR_FILENO, "my_zsh: command not found: %s\n", line->command);
        failure = true;
    }
    if (!failure) command->execute(command);
    line->delete(line);
}

void delete()
{
    shell.environment->delete(shell.environment);
}
