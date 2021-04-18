#include "environment.h"
#include "shell.h"
#include "command_line.h"
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    (void)argc, (void)argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    shell.execute(CommandLineClass.fromString(""));
    shell.execute(CommandLineClass.fromString("echo"));
    shell.execute(CommandLineClass.fromString("echo roger cyr"));
    shell.execute(CommandLineClass.fromString("echo $PATH"));
    shell.execute(CommandLineClass.fromString("echo $PATH cyr"));
    shell.execute(CommandLineClass.fromString("echo $PICKUP"));
    shell.execute(CommandLineClass.fromString("echo eric $PICKUP"));
    shell.execute(CommandLineClass.fromString("/roger -a"));
    shell.execute(CommandLineClass.fromString("/bin/ls -a"));
    shell.delete();
    return EXIT_SUCCESS;
}
