#include "shell.h"
#include "command.h"
#include "echo_command.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
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
    free(line->command);
    free(line); line = NULL;
    if (!failure) command->execute(command);
}

void delete()
{
    shell.environment->delete(shell.environment);
}
