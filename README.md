# Basic Shell #

## Contributors: ##
1. 140101050 Pranjal Agrawal - Piping and redirection.  
2. 140101051 Prashant Gudipudi - Built-in function implementation
3. 140101065 Sangana Abhiram - History implementation, Documentation
4. 140101080 Yash Pote - Input parsing, Error handling


## Installation ##
1. Change to directory
`cd PATH_TO_DIR/basic_shell`

2. Compile basic_shell
`make install`

3. Run Shell
`make run`


## Usage ##
Run the shell
`make run` 

1. Simple commands
`pwd`
`history`
`ls .`

2. History substitutions
`!!` //Runs ls . (last run command)
`!n` // Runs command indexed 'n' in `history` list

3. Piping
`ls . | grep a`
`cat main.c | grep void | grep input`

4. Redirection(I/O)
`ls /bin/ > bin_list.txt` // Redirect output to file 
`grep nano < bin_list.txt` // Redirect input from file



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
  * Output can be redirected to file(which will be created if it does not exist, or truncated if it does) or input can be redirected from file.



## Flow of Program##
main.c: main() -> get_input() {//Gets user input} -> access_history() {// Checks if the command is of the '!n' format} 
               -> maintain_his() {// Add the current command to history} -> exec.c: parse_inp() { //Break input into tokens based on pipe char or '>' or  '<'} ->  
                      1. pipe_cmd() -> execute()  // If the input is in pipe format
                      2. redir_cmd() -> execute() // if the input is in redirection format
