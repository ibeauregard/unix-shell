#include "shell.h"
#include "concrete_command_class_factory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void display_prompt();
static void execute(CommandLine* line);
static void delete();
struct shell shell = {
        .displayPrompt = &display_prompt,
        .execute = &execute,
        .delete = &delete
};

static char* get_prompt_pwd(char* pwd);
void display_prompt()
{
    char* pwd = shell.environment->getValueFromId(shell.environment, "PWD");
    printf("[%s]\n> ", get_prompt_pwd(pwd));
    free(pwd);
}

void execute(CommandLine* line)
{
    Command* command = ConcreteCommandClassFactory.fromCommand(line->command)->fromArguments(line->arguments);
    command->execute(command);
    line->delete(&line);
    puts("");
}

char* get_prompt_pwd(char* pwd)
{
    char* home = shell.environment->getValueFromId(shell.environment, "HOME");
    size_t home_length;
    for (home_length = strlen(home); home_length > 0 && home[home_length - 1] == '/'; home_length--);
    home[home_length] = 0;
    if (home_length > 0 && !strncmp(home, pwd, home_length)) {
        pwd[home_length - 1] = '~';
        pwd += home_length - 1;
    }
    free(home);
    return pwd;
}

void delete()
{
    shell.environment->delete(&shell.environment);
}
