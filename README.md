CS 537 Homework

A basic shell that can be run in two modes: interactive and batch. In interactive mode a prompt (mysh>) is diplayed and the user of 
the shell can type in a command at the prompt. In batch mode, the shell is started by specifying a batch file on its command line:
the batch file contains the list of commands (each on its own line) that will be executed. In batch mode the prompt won't be 
displayed. In both interactive and batch mode, the shell is terminated when it sees the exit command or reaches the end of the input 
stream (i.e. the end of batch file or the user types Ctrl-D).

Jobs are executed in either the foreground or the background. When a job is run in the foreground, the shell waits until that 
job completes before it proceeds and displays the next prompt. When a job is run in the background (as denoted with the '&' 
character as the last non-whitespace character on the line), the shell starts the job, but then immediately returns and 
displays the next prompt with the background job still running.

Each job that is executed by the shell is given its own unique job id (jid). The shell must assign each new command (whether it 
completes successfully or not) the next integer, starting with jid 1. Empty lines and built-in shell commands 
(such as 'j' and 'myw' described below) will advance the jid count.

Users are able to perform very limited job control with this shell. First, users are able to find out which of their jobs are 
still running by using the shell built-in command 'j' . When the shell sees the 'j' command, it is to print to standard output 
the jid of each of the currently running jobs (i.e., the background jobs that have not yet finished) followed by a colon, :, 
and the job name and its arguments (without the '&').

Users are able to tell the shell to wait for a particular job to terminate by using the built-in command 'myw'. When the shell 
sees the 'myw' command along with the jid of a job, it waits for the specified job to complete before accepting more input. 
When the job completes, the shell writesthe message, " : Job terminated" to STDOUT, if in interactive mode, the shell 
should displays the prompt on the next line.

If the 'myw' command is used with an invalid jid, then the shell should immediately return and print the message, "Invalid jid ".
