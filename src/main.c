#include "environment.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    if (argc) puts(argv[0]);
    Environment* environment = EnvironmentClass.fromStringArray(envp);
    shell.environment = environment;
    environment->print(environment);
    environment->delete(environment);
    return EXIT_SUCCESS;
}
