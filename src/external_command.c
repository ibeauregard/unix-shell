#include "external_command.h"
#include "not_found_command.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static Command* from_command_line(CommandLine* commandLine);
const struct external_command ExternalCommand = {
        .fromCommandLine = &from_command_line
};

struct internals {
    char* pathname;
    char** args_array;
};

static void execute(Command* this);
Command* from_command_line(CommandLine* commandLine)
{
    Command* this = malloc(sizeof (Command));
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->pathname = commandLine->command;
    StringList* arguments = commandLine->arguments;
    this->_internals->args_array = arguments->toStringArray(arguments);
    this->execute = &execute;
    return this;
}

static void delete(Command* this);
void execute(Command* this)
{
    pid_t pid = fork();
    struct internals* internals = this->_internals;
    if (pid == 0) {
        execve(internals->pathname, internals->args_array, NULL);
    }
    int status;
    do {
        waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    delete(this);
}

void delete(Command* this)
{
    char** args = this->_internals->args_array;
    for (size_t i = 0; args[i]; i++) {
        free(args[i]);
    }
    free(args);
    free(this->_internals);
    free(this); this = NULL;
}
