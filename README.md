# Basic Shell #

## Contributors: ##
1. 140101050 Pranjal Agrawal 
2. 140101051 Prashant Gudipudi 
3. 140101065 Sangana Abhiram 
4. 140101080 Yash Pote 

## Contribution: ##
Pranjal Agrawal: Piping and redirection.  
Prashant Gudipudi: Built-in function implementation.  
Sangana Abhiram: History implementation.   
Yash Pote: Input parsing.

## Overview: ##
1. Prompt:
  * Shows the current working directory of the shell
2. In-built commands:
  * Each built-in command is represented by a structure, which contains the command name and its function pointer.
  * Their function declarations have been kept uniform to allow for structure implementation.
  * Built-in commands can be added into the initialize() function, which adds them to a list of built-in commands.
  * Built-in commands implemented:
    * pwd
    * cd
    * history
    * !!
    * !n
    * exit
3. History implementation:
  * History list has been implemented using a circular queue. A head and tail pointer are maintained.
  * !! and !n commands add their corresponding commands to the history, not themselves.
4. Piping:
  * Multiple piping can be achieved in one command.
  * The composite command is split into atomic commands and executed one by one.
  * A pipemask is used to determine if the current command writes to the pipe, reads from pipe or does both.
5. Redirection:
  * Redirection is checked while parsing the input string, and its respective action noted.
  * The process is similar to piping, using identifiers to check for input/output redirection.

## Flow ##
main.c: main() -> get_input() -> access_history() -> maintain_his() -> pipe.c: parse_inp() ->  
1. pipe_cmd() -> execute()  
2. redir_cmd() -> execute()
