#include "environment.h"
#include "shell.h"
#include "my_libc/string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[], char* envp[])
{
    (void) argc, (void) argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    shell.displayPrompt();
    char* line = NULL;
    size_t length = 0;
    while (getline(&line, &length, stdin) != -1) {
        shell.execute(CommandLineClass.fromString(strstrip(line)));
        shell.displayPrompt();
    }
    free(line);
    shell.delete();
    puts("");
    return EXIT_SUCCESS;
}
