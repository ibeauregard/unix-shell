#include "external_command.h"
#include "environment.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>

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

static char* get_command_pathname(char* command);
static void fork_and_execute(Command* this, char* pathname);
static void delete(Command* this);
void execute(Command* this)
{
    char* invokedCommand = this->_internals->commandLine->command;
    char* pathname = get_command_pathname(invokedCommand);
    if (pathname) {
        fork_and_execute(this, pathname);
    } else {
        dprintf(STDERR_FILENO, "my_zsh: Command not found: %s\n", invokedCommand);
    }
    free(pathname);
    delete(this);
}

static char* search_in_path(char* command);
char* get_command_pathname(char* command)
{
    return strchr(command, '/') ? strdup(command) : search_in_path(command);
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

static bool command_in_directory(char* command, char* dirpath);
char* search_in_path(char* command)
{
    char* path = shell.environment->getValueFromId(shell.environment, "PATH");
    StringList* pathElements = StringListClass.split(path, ':'); free(path);
    char* pathname = NULL;
    while (!pathname && !pathElements->isEmpty(pathElements)) {
        char* directory = pathElements->next(pathElements);
        if (command_in_directory(command, directory)) {
            pathname = malloc(strlen(directory) + strlen(command) + 2);
            strcat(strcat(strcpy(pathname, directory), "/"), command);
        }
        free(directory);
    }
    pathElements->delete(pathElements);
    return pathname;
}

void child_process_execute(Command* this, char* pathname)
{
    StringList* arguments = this->_internals->commandLine->arguments;
    char** argv = arguments->toStringArray(arguments);
    Environment* environment = shell.environment;
    char** envp = environment->serialize(environment);
    execve(pathname, argv, envp);
    dprintf(STDERR_FILENO, "my_zsh: %s: %s\n", strerror(errno), pathname);
    for (size_t i = 0; argv[i]; i++) {
        free(argv[i]);
    }
    for (size_t i = 0; envp[i]; i++) {
        free(envp[i]);
    }
    free(argv); free(envp);
    exit(EXIT_FAILURE);
}

bool command_in_directory(char* command, char* dirpath)
{
    DIR* directory = opendir(dirpath);
    if (!directory) return false;
    bool found = false;
    struct dirent* entry;
    while (!found && (entry = readdir(directory))) {
        if (!strcmp(command, entry->d_name)) found = true;
    }
    closedir(directory);
    return found;
}

void delete(Command* this)
{
    free(this->_internals);
    free(this); this = NULL;
}
