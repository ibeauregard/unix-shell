#include "cd_command.h"
#include "shell.h"
#include "environment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static Command* with_args(StringList* arguments);
const struct cd_command CdCommand = {
        .withArgs = &with_args
};

static void initialize_internals(Command* this);
static void parse_arguments(Command* this, StringList* arguments);
static void execute(Command* this);
Command* with_args(StringList* arguments)
{
    Command* this = malloc(sizeof (Command));
    initialize_internals(this);
    parse_arguments(this, arguments);
    this->execute = &execute;
    return this;
}

struct internals {
    bool pOption;
    char* operand;
    bool error;
};

void initialize_internals(Command* this)
{
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->pOption = false;
    this->_internals->operand = NULL;
    this->_internals->error = false;
}

static void handle_flag_argument(Command* this, char* argument);
void parse_arguments(Command* this, StringList* arguments)
{
    free(arguments->next(arguments)); // discard 'cd' argument (program name)
    while (!this->_internals->error
            && !arguments->isEmpty(arguments)
            && arguments->peek(arguments)[0] == '-') {
        handle_flag_argument(this, arguments->next(arguments));
    }
    if (!this->_internals->error && !arguments->isEmpty(arguments)) {
        this->_internals->operand = arguments->next(arguments);
    }
    if (!this->_internals->error && !arguments->isEmpty(arguments)) {
        this->_internals->error = true;
        dprintf(STDERR_FILENO, "%s\n", "cd: Too many arguments");
        while (!arguments->isEmpty(arguments)) free(arguments->next(arguments));
    }
}

void handle_flag_argument(Command* this, char* argument)
{
    for (size_t i = 1; !this->_internals->error && argument[i]; i++) {
        if (argument[i] == 'L') this->_internals->pOption = false;
        else if (argument[i] == 'P') this->_internals->pOption = true;
        else {
            dprintf(STDERR_FILENO, "cd: Illegal option -%c\n", argument[i]);
            this->_internals->error = true;
        }
    }
    free(argument);
}

static void do_execute(Command* this);
static void delete(Command* this);
void execute(Command* this)
{
    if (!this->_internals->error) do_execute(this);
    delete(this);
}

struct state {
    char* curpath;
    char* home_value;
    char* cdpath_value;
    char* oldpwd_value;
    bool print_new_directory_name;
    bool interrupted;
};

static struct state initialize_state();
static void set_operand(Command* this, struct state* state);
static bool starts_with_slash_or_dot_or_dot_dot(char* string);
static void browse_cdpath(char* operand, struct state* state);
static void free_state(struct state* state);
void do_execute(Command* this)
{
    struct state state = initialize_state();
    set_operand(this, &state);
    if (!state.interrupted
        && !starts_with_slash_or_dot_or_dot_dot(this->_internals->operand)) {
        browse_cdpath(this->_internals->operand, &state);
    }
    if (!state.interrupted && !state.curpath) state.curpath = strdup(this->_internals->operand);
    if (!state.interrupted && !this->_internals->pOption)

    free_state(&state);
}

struct state initialize_state()
{
    return (struct state){
        .curpath = NULL,
        .home_value = shell.environment->getValueFromId(shell.environment, "HOME"),
        .cdpath_value = shell.environment->getValueFromId(shell.environment, "CDPATH"),
        .oldpwd_value = shell.environment->getValueFromId(shell.environment, "OLDPWD"),
        .print_new_directory_name = false,
        .interrupted = false
    };
}

void set_operand(Command* this, struct state* state)
{
    if (!this->_internals->operand) {
        if (!strlen(state->home_value)) {
            free_state(state);
            state->interrupted = true;
        } else {
            this->_internals->operand = strdup(state->home_value);
        }
    }  else if (!strcmp(this->_internals->operand, "-") && strlen(state->oldpwd_value) > 0) {
        free(this->_internals->operand);
        this->_internals->operand = strdup(state->oldpwd_value);
        state->print_new_directory_name = true;
    } else if (!strcmp(this->_internals->operand, "~") && strlen(state->home_value) > 0) {
        free(this->_internals->operand);
        this->_internals->operand = strdup(state->home_value);
    }
}

bool starts_with_slash_or_dot_or_dot_dot(char* string)
{
    return string[0] == '/'
           || (strlen(string) == 1 && string[0] == '.')
           || (strlen(string) >= 2 && string[0] == '.' && string[1] == '/')
           || (strlen(string) == 2 && string[0] == '.' && string[1] == '.')
           || (strlen(string) >= 3 && string[0] == '.' && string[1] == '.' && string[2] == '/');
}

static char* get_concatenation(char* pathname, char* operand);
static bool names_a_directory(char* string);
void browse_cdpath(char* operand, struct state* state)
{
    StringList* cdpathElements = StringListClass.split(state->cdpath_value, ':');
    while (!state->curpath && !cdpathElements->isEmpty(cdpathElements)) {
        char* pathname = cdpathElements->next(cdpathElements);
        char* concatenation = get_concatenation(pathname, operand);
        if (names_a_directory(concatenation)) state->curpath = concatenation;
        else free(concatenation);
        free(pathname);
    }
    if (!state->curpath) {
        char* concatenation = get_concatenation(".", operand);
        if (names_a_directory(concatenation)) state->curpath = concatenation;
        else free(concatenation);
    }
    cdpathElements->delete(cdpathElements);
}

char* get_concatenation(char* pathname, char* operand)
{
    size_t pathname_length = strlen(pathname);
    char* concatenation = malloc(pathname_length + (pathname[pathname_length - 1] != '/') + strlen(operand) + 1);
    strcpy(concatenation, pathname);
    if (pathname[pathname_length - 1] != '/') strcat(concatenation, "/");
    strcat(concatenation, operand);
    return concatenation;
}

bool names_a_directory(char* string)
{
    struct stat statbuf;
    if (stat(string, &statbuf) == -1) return false;
    return S_ISDIR(statbuf.st_mode);
}

void free_state(struct state* state)
{
    if (state->curpath) free(state->curpath);
    free(state->home_value);
    free(state->cdpath_value);
    free(state->oldpwd_value);
}

void delete(Command* this)
{
    free(this->_internals->operand);
    free(this->_internals);
    free(this); this = NULL;
}
