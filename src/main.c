#include "environment.h"
#include "shell.h"
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    (void)argc, (void)argv;
    shell.environment = EnvironmentClass.fromStringArray(envp);
    exit(0);
    shell.delete();
    return EXIT_SUCCESS;
}
