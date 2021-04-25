# my_zsh

## Description

This is a Unix command line interpreter (CLI). It implements the following built-ins: echo, cd, setenv, unsetenv, env and exit. Other commands are delegated to the system.

Note that this CLI only supports basic command lines; it does not support pipes, redirections or auto-completion, or other advanced features.

## Compiling

To compile the application, run `make` from the project's root directory.

## Running the application

After the project has compiled, run the `my_zsh` executable file.

## echo

echo -- write arguments to the standard output

### SYNOPSIS

`echo [string ...]`

### DESCRIPTION

The `echo` built-in writes any specified operands, separated by single blank ('` `') characters and followed by a  newline ('`\n`') character, to the standard output.

## cd

cd -- change the working directory

### SYNOPSIS

`cd [-L|-P] [directory]`
`cd <-|~>`

### DESCRIPTION

The `cd` built-in changes the working directory of the current shell execution environment.

### OPTIONS

The following options are supported by the implementation:

    -L        Handle the operand dot-dot logically; symbolic link
                 components are not resolved before dot-dot
                 components are processed.

    -P        Handle the operand dot-dot physically; symbolic link
                 components are resolved before dot-dot components
                 are processed.

If both -L and -P options are specified, the last of these options is used and all others ignored. If neither -L nor -P is specified, the operand shall be handled dot-dot logically.

### OPERANDS

The following operands are supported:

    directory   An absolute or relative pathname of the directory that
                 shall become the new working directory. The
                 interpretation of a relative pathname by cd depends on
                 the -L option and the CDPATH and PWD environment
                 variables. If directory is an empty string, the directory
                 specified by the HOME environment variable becomes the new
                 working directory, if that variable is set.

    -         When a <hyphen-minus> is used as the operand, this
                 shall be equivalent to the command:

                     cd "$OLDPWD" && pwd

                 which changes to the previous working directory and
                 then writes its name.

    ~         When a <tilde> is used as the operand, this
                 shall be equivalent to the command:

                     cd "$HOME"

                 which changes to the directory specified by the HOME environment
                 variable, if that variable is set.

## setenv

setenv -- change or add an environment variable

### SYNOPSIS

`setenv [-o] [name=value ...]` 

### DESCRIPTION

The `setenv` built-in adds the variable `name` to the environment with the value `value`, if `name` does not already exist.  If `name` does exist in the environment, then its value is changed to `value` if the option `-o` is specified; if `-o` is not specified, then the value of name is not changed.

The `-o` option is only recognized when it is specified before any `name=value` argument.

## unsetenv

unsetenv -- remove an environment variable

### SYNOPSIS

`unsetenv [name ...]`

### DESCRIPTION

The `unsetenv` built-in shall remove environment variables from the environment of the command line interpreter. The `name` argument is the name of a variable to be removed. The named arguments shall not contain an '=' character. If a named variable does not exist in the current environment, the environment shall be unchanged and the function is considered to have completed successfully.

## env

env -- set environment and execute command, or print environment

### SYNOPSIS

`env [-i] [[-u <name>] ...] [name=value ...] [utility [argument ...]]`

### DESCRIPTION

The `env` built-in executes another utility after modifying the environment as specified on the command line.  Each `name=value` option specifies the setting of an environment variable, `name`, with a value of `value`.  All such environment variables are set before the utility is executed.

If no utility is specified, `env` prints out the names and values of the variables in the environment, with one name/value pair per line.

### OPTIONS

    -i          Execute the utility with only those environment variables
                specified by name=value options.  The environment inherited
                by env is ignored completely.

    -u name
                If the environment variable name is in the environment,
                then remove it before processing the remaining options. 
                The value for name must not include the `=' character.

The above options are only recognized when they are specified before any `name=value` options.

## exit

exit -- cause the command line interpreter to exit

### SYNOPSIS

`exit [n]`

### DESCRIPTION

The `exit` utility shall cause the command-line interpreter to exit from its current execution environment with the exit status specified by the unsigned 8-bit integer `n`. If n is specified, but cannot be converted to an unsigned integer value, the exit status is 0. If n is specified, but its value is not between 0 and 255 inclusively, the exit status is n % 256. Note that for any integer n, `n % 256 == n - 256 * floor(n / 256)`, where `floor(x)` rounds `x` towards minus infinity.

## Implementation notes

As a central architecture principle, a `Command` interface is defined, which consists of only one method named `execute`. All commands implement the `Command` interface, and throughout the program, the code only deals with the `Command` abstract type. There are no `CdCommand` or `EchoCommand` types, nor any other type representing a concrete command.

There is, however, an abstract type representing a concrete command _class_: `ConcreteCommandClass`, which consists of a single method named `fromArguments`. All concrete command types implement the `ConcreteCommandClass` interface, in order to provide a _uniform_ way to construct a concrete command from a list of arguments.

When the application has to build a concrete command, the choice of the right concrete command class is centralized in one place, i.e. the `ConcreteCommandClassFactory`.

The code base also generally uses a coding style called "C-plus", emulating object-oriented programming and encapsulation of data with the C programming language.
