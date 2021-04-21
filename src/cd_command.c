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
    bool parseError;
};

void initialize_internals(Command* this)
{
    this->_internals = malloc(sizeof (struct internals));
    this->_internals->pOption = false;
    this->_internals->operand = NULL;
    this->_internals->parseError = false;
}

static void process_flag_argument(Command* this, char* argument);
void parse_arguments(Command* this, StringList* arguments)
{
    free(arguments->next(arguments)); // discard 'cd' argument (program name)
    while (!this->_internals->parseError
            && !arguments->isEmpty(arguments)
            && arguments->peek(arguments)[0] == '-') {
        process_flag_argument(this, arguments->next(arguments));
    }
    if (!this->_internals->parseError && !arguments->isEmpty(arguments)) {
        this->_internals->operand = arguments->next(arguments);
    }
    if (!this->_internals->parseError && !arguments->isEmpty(arguments)) {
        this->_internals->parseError = true;
        dprintf(STDERR_FILENO, "%s\n", "cd: Too many arguments");
        while (!arguments->isEmpty(arguments)) free(arguments->next(arguments));
    }
}

void process_flag_argument(Command* this, char* argument)
{
    for (size_t i = 1; !this->_internals->parseError && argument[i]; i++) {
        if (argument[i] == 'L') this->_internals->pOption = false;
        else if (argument[i] == 'P') this->_internals->pOption = true;
        else {
            dprintf(STDERR_FILENO, "cd: Illegal option -%c\n", argument[i]);
            this->_internals->parseError = true;
        }
    }
    free(argument);
}

static void do_execute(Command* this);
static void delete(Command* this);
void execute(Command* this)
{
    if (!this->_internals->parseError) do_execute(this);
    delete(this);
}

struct state {
    char* curpath;
    char* absolute_curpath;
    char* home_value;
    char* cdpath_value;
    char* pwd_value;
    char* oldpwd_value;
    bool print_new_directory_name;
    bool interrupted;
};

static struct state initialize_state();
static void set_operand(Command* this, struct state* state);
static bool starts_with_slash_or_dot_or_dot_dot(char* string);
static void browse_cdpath(char* operand, struct state* state);
static void adjust_curpath(struct state* state);
static void change_directory(Command* this, struct state* state);
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
    if (!state.interrupted && !this->_internals->pOption) adjust_curpath(&state);
    if (!state.interrupted) change_directory(this, &state);
    free_state(&state);
}

struct state initialize_state()
{
    return (struct state){
        .curpath = NULL,
        .absolute_curpath = NULL,
        .home_value = shell.environment->getValueFromId(shell.environment, "HOME"),
        .cdpath_value = shell.environment->getValueFromId(shell.environment, "CDPATH"),
        .pwd_value = shell.environment->getValueFromId(shell.environment, "PWD"),
        .oldpwd_value = shell.environment->getValueFromId(shell.environment, "OLDPWD"),
        .print_new_directory_name = false,
        .interrupted = false
    };
}

static void process_no_operand(Command* this, struct state* state);
static void process_hyphen_operand(Command* this, struct state* state);
static void process_tilde_operand(Command* this, struct state* state);
void set_operand(Command* this, struct state* state)
{
    if (!this->_internals->operand) {
        process_no_operand(this, state);
    } else if (!strcmp(this->_internals->operand, "-")) {
        process_hyphen_operand(this, state);
    } else if (!strcmp(this->_internals->operand, "~")) {
        process_tilde_operand(this, state);
    }
}

void process_no_operand(Command* this, struct state* state)
{
    if (!strlen(state->home_value)) {
        free_state(state);
        state->interrupted = true;
    } else {
        this->_internals->operand = strdup(state->home_value);
    }
}

void process_hyphen_operand(Command* this, struct state* state)
{
    if (strlen(state->oldpwd_value) > 0) {
        free(this->_internals->operand);
        this->_internals->operand = strdup(state->oldpwd_value);
        state->print_new_directory_name = true;
    } else {
        dprintf(STDERR_FILENO, "%s\n", "cd: OLDPWD not set");
        state->interrupted = true;
    }
}

void process_tilde_operand(Command* this, struct state* state)
{
    if (strlen(state->home_value) > 0) {
        free(this->_internals->operand);
        this->_internals->operand = strdup(state->home_value);
    } else {
        dprintf(STDERR_FILENO, "%s\n", "cd: HOME not set");
        state->interrupted = true;
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

static char* join_with_slash(char* string1, char* string2);
static bool names_a_directory(char* string);
void browse_cdpath(char* operand, struct state* state)
{
    StringList* cdpathElements = StringListClass.split(state->cdpath_value, ':');
    while (!state->curpath && !cdpathElements->isEmpty(cdpathElements)) {
        char* pathname = cdpathElements->next(cdpathElements);
        char* concatenation = join_with_slash(pathname, operand);
        if (names_a_directory(concatenation)) state->curpath = concatenation;
        else free(concatenation);
        free(pathname);
    }
    if (!state->curpath) {
        char* concatenation = join_with_slash(".", operand);
        if (names_a_directory(concatenation)) state->curpath = concatenation;
        else free(concatenation);
    }
    cdpathElements->delete(cdpathElements);
}

char* join_with_slash(char* string1, char* string2)
{
    size_t string1Length = strlen(string1);
    char* concatenation = malloc(string1Length + (string1[string1Length - 1] != '/') + strlen(string2) + 1);
    strcpy(concatenation, string1);
    if (string1[string1Length - 1] != '/') strcat(concatenation, "/");
    strcat(concatenation, string2);
    return concatenation;
}

bool names_a_directory(char* string)
{
    struct stat statbuf;
    if (stat(string, &statbuf) == -1) return false;
    return S_ISDIR(statbuf.st_mode);
}

static void prepend_to_curpath(struct state* state);
static void make_curpath_canonical(struct state* state);
static void make_curpath_relative(struct state* state);
void adjust_curpath(struct state* state)
{
    if (state->curpath[0] != '/') prepend_to_curpath(state);
    make_curpath_canonical(state);
    if (!state->interrupted) make_curpath_relative(state);
}

void prepend_to_curpath(struct state* state)
{
    char* old_curpath = state->curpath;
    state->curpath = join_with_slash(state->pwd_value, old_curpath);
    free(old_curpath);
}

static void process_dot_component(struct state* state, size_t component_index);
static size_t process_dot_dot_component(struct state* state, size_t component_index);
void make_curpath_canonical(struct state* state)
{
    size_t i = 0;
    while (!state->interrupted && state->curpath[i]) {
        for (; state->curpath[i]
                && !(state->curpath[i] == '.'
                        && (i == 0 || state->curpath[i - 1] == '/')); i++);
        if (state->curpath[i]) {
            if (state->curpath[i + 1] == 0
                || state->curpath[i + 1] == '/') process_dot_component(state, i);
            else if (state->curpath[i + 1] == '.'
                     && (state->curpath[i + 2] == 0
                         || state->curpath[i + 2] == '/')) i = process_dot_dot_component(state, i);
        }
    }
    if (i == 0) state->interrupted = true;
}

void process_dot_component(struct state* state, size_t component_index)
{
    size_t offset;
    for (offset = 1; state->curpath[component_index + offset] == '/'; offset++);
    size_t path_length = strlen(state->curpath);
    for (size_t i = component_index + offset; i <= path_length; i++) { // <= to copy '\0'
        state->curpath[i - offset] = state->curpath[i];
    }
}

static size_t get_previous_component_length(struct state* state, size_t component_index);
static size_t get_backward_offset(
                struct state* state,
                size_t component_index,
                size_t previous_component_length);
static size_t get_forward_offset(struct state* state, size_t component_index);
size_t process_dot_dot_component(struct state* state, size_t component_index)
{
    size_t previous_component_length = get_previous_component_length(state, component_index);
    if (previous_component_length == 0) return component_index + 2; // if there is no previous component
    state->curpath[previous_component_length] = 0;
    if (!names_a_directory(state->curpath)) {
        dprintf(STDERR_FILENO, "cd: Not a directory: %s\n", state->curpath);
        state->interrupted = true;
        return component_index + 2;
    }
    state->curpath[previous_component_length] = '/';
    size_t backward_offset = get_backward_offset(state, component_index, previous_component_length);
    size_t forward_offset = get_forward_offset(state, component_index);
    size_t path_length = strlen(state->curpath);
    for (size_t i = component_index; i + forward_offset <= path_length; i++) {
        state->curpath[i - backward_offset] = state->curpath[i + forward_offset];
    }
    return component_index - backward_offset;
}

size_t get_previous_component_length(struct state* state, size_t component_index)
{
    size_t previous_component_length;
    for (previous_component_length = component_index;
         previous_component_length > 0 && state->curpath[previous_component_length - 1] == '/';
         previous_component_length--);
    return previous_component_length;
}

size_t get_backward_offset(
        struct state* state,
        size_t component_index,
        size_t previous_component_length)
{
    size_t backward_offset;
    for (backward_offset = component_index - previous_component_length + 1;
         backward_offset < component_index  && state->curpath[component_index - backward_offset - 1] != '/';
         backward_offset++);
    return backward_offset;
}

size_t get_forward_offset(struct state* state, size_t component_index)
{
    size_t forward_offset;
    for (forward_offset = 2;
         state->curpath[component_index + forward_offset] == '/';
         forward_offset++);
    return forward_offset;
}

void make_curpath_relative(struct state* state)
{
    state->absolute_curpath = strdup(state->curpath);
    char* pwd = join_with_slash(state->pwd_value, "");
    size_t pwd_length = strlen(pwd);
    if (!strncmp(pwd, state->curpath, pwd_length)) {
        size_t curpath_length = strlen(state->curpath);
        for (size_t i = pwd_length; i <= curpath_length; i++) { // <= to copy '\0'
            state->curpath[i - pwd_length] = state->curpath[i];
        }
    }
    free(pwd);
}

void change_directory(Command* this, struct state* state)
{
    (void)this, (void)state;
}

void free_state(struct state* state)
{
    if (state->curpath) free(state->curpath);
    if (state->absolute_curpath) free(state->absolute_curpath);
    free(state->home_value);
    free(state->cdpath_value);
    free(state->pwd_value);
    free(state->oldpwd_value);
}

void delete(Command* this)
{
    free(this->_internals->operand);
    free(this->_internals);
    free(this); this = NULL;
}
