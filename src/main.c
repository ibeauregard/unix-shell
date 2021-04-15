#include "environment.h"
#include "shell.h"
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    (void)argc, (void)argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    shell.environment->print(shell.environment);
    shell.delete();
    return EXIT_SUCCESS;
}
