#include "shell.h"

static void delete();
struct shell shell = {
        .delete = &delete
};

void delete()
{
    shell.environment->delete(shell.environment);
}
