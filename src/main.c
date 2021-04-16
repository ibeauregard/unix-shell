#include "environment.h"
#include "shell.h"
#include "command_line.h"
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    (void)argc, (void)argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    shell.execute(CommandLineClass.fromString(""));
    shell.execute(CommandLineClass.fromString("echo roger cyr"));
    shell.execute(CommandLineClass.fromString("/bin/ls -a"));
    shell.delete();
    return EXIT_SUCCESS;
}
