#include "shell.h"
#include "command.h"
#include "command_factory.h"
#include <stdio.h>
#include <stdlib.h>

static void display_prompt();
static void execute(CommandLine* line);
static void delete();
struct shell shell = {
        .displayPrompt = &display_prompt,
        .execute = &execute,
        .delete = &delete
};

void display_prompt()
{
    char* pwd = shell.environment->getValueFromId(shell.environment, "PWD");
    printf("%s\n> ", pwd);
    free(pwd);
}

void execute(CommandLine* line)
{
    Command* command = CommandFactory.fromCommandLine(line);
    command->execute(command);
    line->delete(&line);
    puts("");
}

void delete()
{
    shell.environment->delete(&shell.environment);
}
