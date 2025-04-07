# Minishell

42 assignment to do a simple shell, similar to Bash.

# Instructions

Instructions to complete the project (copied from the subject)

## Allowed functions

The following functions are able to be used freely in the project.
- readline
- rl_clear_history
- rl_on_new_line
- rl_replace_line
- rl_redisplay
- add_history
- printf
- malloc
- free
- write
- access
- open
- read,
- close
- fork
- wait
- waitpid
- wait3
- wait4
- signal
- sigaction
- sigemptyset
- sigaddset
- kill
- exit
- getcwd
- chdir
- stat
- lstat
- fstat
- unlink
- execve
- dup
- dup2
- pipe
- opendir
- readdir
- closedir
- strerror
- perror
- isatty
- ttyname
- ttyslot
- ioctl
- getenv
- tcsetattr
- tcgetattr
- tgetent
- tgetflag
- tgetnum
- tgetstr
- tgoto
- tputs

Additionally, the use of libft is authorized.

## Required features

These features are required for the mandatory part

### General requirements:
- Display a **prompt** when waiting for a new command.
- Have a working **history**.
- Search and launch the right executable (based on the `PATH` variable or using a relative or absolute path).
- Avoid using more than **one global variable** to indicate a received signal. Consider the implications: this approach ensures that your signal handler will not access your main data structures.
- Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
- Handle ’ (single quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence.
- Handle " (double quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence except for $ (dollar sign).

### Implement redirections:
- < should redirect input.
- \> should redirect output.
- << should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
- \>\> should redirect output in append mode.

- Implement **pipes** (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
- Handle **environment variables** (`$` followed by a sequence of characters) which should expand to their values.
- Handle `$?` which should expand to the exit status of the most recently executed
foreground pipeline.
- Handle `ctrl-C`, `ctrl-D` and `ctrl-\` which should behave like in bash.

Required commands:
- `echo` with option `-n`
- `cd` with only a relative or absolute path
- `pwd` with no options
- `export` with no options
- `unset` with no options
- `env` with no options or arguments
- `exit` with no options

### Additional infos

The readline() function can cause memory leaks. You don’t have to fix them. But that doesn’t mean your own code, yes the code you wrote, can have memory leaks.

## Bonus part

Your program has to implement:
- `&&` and `||` with parenthesis for priorities.
- Wildcards `*` should work for the current working directory.

# Development notes

This is just a section to put thoughts and info on how we want the code to look (basically our own norm).

\> kmautner

I suggest we have a file structure along the lines of "{repo root}/feature_name/feature.c".
Also, since I have a tendency to write spaghetti code, I'll do my best to add documentation to everything.

If an error occurs, I propose we don't exit the program immediately, but instead pass the error code up
the call stack until we reach a significant function and exit there. This allows it so that each of the
called functions can print their own error message, giving us a simple stack trace.
For example: If the call stack looks like this...
```c
main()
    init()
        load_configs()
            get_config_file()
                read_data()
                    readline()
```
... and `readline()` fails, an error code would be passed up all the way to `init()` or `main()`,
each called function printing their own error message as they pass it along.
The terminal ouput would then look something like this:
```
Error
unable to read data
unable to read config file
unable to load configs
failed to initialize configs
init fail
```
`readline()` fails, making `read_data()` print "unable to read data" in the terminal and return an error code, prompting `get_config_file()` to print its own error message, etc. all the way up to the main
function where the program exits.

\> kfan

//new to do list April 7
// <<eof < hello cat |echo 123 --- ctrl + d eof warning comes before STDOUT, bash error comes after STDOUT
//ctrl + c then <<eof cat and ctrl + c/ or d then ctrl + c, why extra \n???? after changing gnl to readline in here_doc
// to Koloman: please check the add_history empty line thing if there is leak, thanks!

//fixed April 7
//signals
//cat | echo 123
//waitpid
// ctrl + \ should do nth in minishell and heredoc!

//to do list 3 April
// mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD; unset OLDPWD; pwd
// cat /dev/random | head -c 100 | wc -c // waitpid ??? signal?
// ls '" and ls "' // just leave it as it is?

fixed 2 April
// echo hello $NOT_A_VAT $NOT_A_VAT $NOT_A_VAT// with space handling still one last space left
// export L="ls -la"; $L // change tree order and split?
// unset HOME; cd $HOME // also unset PATH  // just leave it as it is?


fixed: 1 April
//.. > hardcoded
// export A=a A2=a; unset A; echo $A $A2 // also space handling
// exit -9223372036854775808 // hardcoded LONG_MIN
// check pipes forbitten files // this was tricky
// < valid_finfile_1 cat >outfile1 // if open failed just continue? no return 1? // and this one too, so weird
// cat < forbidden_file: echo $? // also ./forbidden_file // easy fix at the end of the child
