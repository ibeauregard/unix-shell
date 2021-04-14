#include "environment.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[])
{
    if (argc) puts(argv[0]);
    Environment* environment = EnvironmentClass.fromStringArray(env);
    environment->print(environment);
    environment->delete(environment);
    return EXIT_SUCCESS;
}
