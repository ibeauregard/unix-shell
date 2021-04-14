#include "environment.h"
#include <stdio.h>

int main(int argc, char* argv[], char* env[])
{
    if (argc) puts(argv[0]);
    environment.parse(env);
    environment.print();
    environment.delete();
}
