#include "command_line.h"
#include "shell.h"
#include "environment.h"
#include <stdlib.h>
#include <string.h>

static CommandLine* from_string(char* string);
const struct command_line_class CommandLineClass = {
        .fromString = &from_string
};

static char* replace_env_variables(char* string);
static void delete(CommandLine* this);
CommandLine* from_string(char* string)
{
    CommandLine* this = malloc(sizeof (CommandLine));
    StringList* tokenList = StringListClass.splitTransform(string, ' ', &replace_env_variables);
    this->command = tokenList->peek(tokenList);
    this->arguments = tokenList;
    this->delete = &delete;
    return this;
}

static bool is_alpha_num(char c);
static char* replace_env_variable(char* string, size_t dollar_sign_index, size_t var_id_length);
char* replace_env_variables(char* string)
{
    long dollar_sign_index;
    while ((dollar_sign_index = strchr(string, '$') - string) >= 0) {
        size_t var_id_length;
        for (var_id_length = 0; is_alpha_num(string[dollar_sign_index + 1 + var_id_length]); var_id_length++);
        string = replace_env_variable(string, dollar_sign_index, var_id_length);
    }
    return string;
}

bool is_alpha_num(char c)
{
    return ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || c == '_';
}

char* replace_env_variable(char* string, size_t dollar_sign_index, size_t var_id_length)
{
    char* var_id = strndup(&string[dollar_sign_index + 1], var_id_length);
    char* var_value = shell.environment->getValueFromId(shell.environment, var_id);
    char* new_string = malloc(strlen(string) - var_id_length + strlen(var_value)); // -1 for '$', +1 for '\0'
    string[dollar_sign_index] = 0;
    char* before_variable = string;
    char* after_variable = &string[dollar_sign_index + 1 + var_id_length];
    strcat(strcat(strcpy(new_string, before_variable), var_value), after_variable);
    free(string); free(var_id); free(var_value);
    return new_string;
}

void delete(CommandLine* this)
{
    this->arguments->delete(this->arguments);
    free(this); this = NULL;
}
