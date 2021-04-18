#include "external_command.h"
#include "not_found_command.h"
#include "environment.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static Command* from_command_line(CommandLine* commandLine);
const struct external_command ExternalCommand = {
        .fromCommandLine = &from_command_line
};

struct internals {
    CommandLine* commandLine;
};

static void execute(Command* this);
Command* from_command_line(CommandLine* commandLine)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->commandLine = commandLine;
    this->execute = &execute;
    return this;
}

static char* get_command_pathname(Command* this);
static void delete(Command* this);
void execute(Command* this)
{
    char* pathname = get_command_pathname(this);
    if (!pathname) return;
    pid_t pid = fork();
    if (pid == 0) {
        StringList* arguments = this->_internals->commandLine->arguments;
        Environment* environment = shell.environment;
        execve(pathname, arguments->toStringArray(arguments), environment->serialize(environment));
    }
    int status;
    do {
        waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    delete(this);
}

static char* search_in_path(char* command);
char* get_command_pathname(Command* this)
{
    char* command = this->_internals->commandLine->command;
    return strchr(command, '/') ? NULL : search_in_path(command);
}

char* search_in_path(char* command)
{
    return command;
}

void delete(Command* this)
{
    free(this->_internals);
    free(this); this = NULL;
}
