## Shell Lab!

### Purpose

## LEARNING!!! :p

### Assignment

#### Part 1
Using your tokenizer and the system calls fork(), exec(), and wait() create a simple shell that:

- prints a command prompt which is "$ " and waits for the user to enter a command
- parse the command using your tokenizer 
- create a child process that uses execve to run the command with its arguments.  
- If an absolute path is not specified, your shell should instead find it using the $PATH environment variable.
- the parent process should wait for the child to terminate before printing another command prompt.
- print "command not found" if the command is not found
- if the command fails (with a non-zero exit value N), your shell should print "Program terminated with exit code N."
- empty commands should do nothing
- the "exit" command should cause your shell to terminate.

We have provided a program called tt (tokenizer tester) which is meant to test your tokenizer. Compile and add this program to your PATH 
environment variable before running your shell.

#### Part 2

Create a user shell in the C programming language that can understand:

- simple commands (e.g. $ /bin/ls or $ ls )
- simple pipes (e.g. $ /bin/ls | /bin/sort -r)
- background tasks (e.g. $ find /etc -print & )
-  "cd" ## for "cd" you will need to lookup the library routine "chdir" in the (online) unix manual

You have additional components required for testing this part of the shell. See the section below for further details.

optional (except for grad students)
- redirecting input and output (e.g. $ ls > /tmp/files.txt)
- setting environment variables: "var=value" 
- expanding ~ at the beginning of a parameter
- expanding non-embedded environment variables (e.g. "echo $PATH")

optional for everyone
- command line editing using readline(3)
- expanding embedded environment variables (e.g. "echo ${HOME}/bin"
- expanding ~ within a parameter (e.g. "ls | dd of=~/files.txt")
- Programs that cannot be exec'd or terminate with errors (non-zero exit codes) should be reported.

Your lab must directly use the posix calls:

- pipe()
- fork()
- dup() or dup2()
- execve() (you cannot use execvp)
- wait()
- open() and close()
- chdir()

These system calls are documented in chapter 2 of the unix manual (e.g. $ man 2 fork).

In addition, your program should dynamically allocate memory using the
library calls malloc and free. You may (if you like) use calloc and
strdup which are part of the same library.

### Executing

1. go to shell directory
2. 'make'
3. 'make demo'
4. Have fun!
5. 'exit' to quit program
