#include "environment.h"
#include "shell.h"
#include "command_line.h"
#include <stdlib.h>
#include <string.h>

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
    shell.execute(CommandLineClass.fromString("/not_found -a"));
    shell.execute(CommandLineClass.fromString("./permission_denied"));
    shell.execute(CommandLineClass.fromString("/bin/ls -a"));
    shell.execute(CommandLineClass.fromString("not_found -a"));
    shell.execute(CommandLineClass.fromString("ls -a"));
    shell.execute(CommandLineClass.fromString("pwd"));
    shell.execute(CommandLineClass.fromString("setenv"));
    shell.execute(CommandLineClass.fromString("setenv TEST=test"));
    shell.execute(CommandLineClass.fromString("setenv HOME=home"));
    shell.execute(CommandLineClass.fromString("setenv TEST=test HOME=home"));
    shell.execute(CommandLineClass.fromString("echo $TEST $HOME $ABC"));
    shell.execute(CommandLineClass.fromString("setenv -o"));
    shell.execute(CommandLineClass.fromString("setenv -o TEST=test2"));
//    shell.execute(CommandLineClass.fromString("setenv -o ABC=abc"));
//    shell.execute(CommandLineClass.fromString("setenv -o TEST=test2 ABC=abc"));
//    shell.execute(CommandLineClass.fromString("echo $TEST $HOME $ABC"));
    shell.delete();
    return EXIT_SUCCESS;
}
