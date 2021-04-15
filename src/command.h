#ifndef COMMAND_H
#define COMMAND_H

typedef struct command Command;
struct command {
    struct internals* _internals;
    void (*execute)(Command* command);
};

#endif
