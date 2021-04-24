#include "null_command.h"

static void execute(Command* this);
Command* NullCommand = &(Command){
        .execute = &execute
};

void execute(Command* this)
{
    (void)this;
}
