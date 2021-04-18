#include "external_command.h"
#include "not_found_command.h"
#include "environment.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

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
static void fork_and_execute(Command* this, char* pathname);
static void delete(Command* this);
void execute(Command* this)
{
    char* pathname = get_command_pathname(this);
    if (pathname) fork_and_execute(this, pathname);
    delete(this);
}

static char* search_in_path(char* command);
char* get_command_pathname(Command* this)
{
    char* command = this->_internals->commandLine->command;
    return strchr(command, '/') ? command : search_in_path(command);
}

static void child_process_execute(Command* this, char* pathname);
void fork_and_execute(Command* this, char* pathname)
{
    pid_t pid = fork();
    if (pid == 0) child_process_execute(this, pathname);
    int status;
    do {
        waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

char* search_in_path(char* command)
{
    return command;
}

void child_process_execute(Command* this, char* pathname)
{
    StringList* arguments = this->_internals->commandLine->arguments;
    char** argv = arguments->toStringArray(arguments);
    Environment* environment = shell.environment;
    char** envp = environment->serialize(environment);
    execve(pathname, argv, envp);
    dprintf(STDERR_FILENO, "my_zsh: %s: %s\n", strerror(errno), pathname);
    arguments->delete(arguments);
    for (size_t i = 0; envp[i]; i++) {
        free(envp[i]);
    }
    free(argv); free(envp);
    exit(EXIT_FAILURE);
}

void delete(Command* this)
{
    free(this->_internals);
    free(this); this = NULL;
}
