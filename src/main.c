#include "environment.h"
#include "shell.h"
#include "command_line.h"
#include "command.h"
#include "command_factory.h"
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    (void)argc, (void)argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    shell.execute(CommandLineClass.new());
    shell.delete();
    return EXIT_SUCCESS;
}
