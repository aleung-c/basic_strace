# ft_strace
Recreating a basic executable tracer.

This little project is an implementation of the strace/truss command.
The idea here is to create a program that will execute another program and "trace" it.

That means the "tracer" will catch the desired instructions from the "tracee" and print its informations.
To do that, the [ptrace()](http://man7.org/linux/man-pages/man2/ptrace.2.html) system call is used.
It has many options, and allows to break the execution of the "tracee" and "peek" into its memory.

In my opinion, this project is a good introduction to understand the underlying workings of utilities like gdb.
