#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
extern int errno;

#define MAX_ARGS 10
#define MAX_ARG_SIZE 10

#define READ_BIT 1
#define WRITE_BIT 2

int execute(char *cmd, int fd[], int pipe_st) {



    char **argv = malloc(MAX_ARGS * sizeof(char *));
    int i, idx = 0;
    for (i = 0; i< MAX_ARGS; ++i) {
        argv[i] = malloc(MAX_ARG_SIZE * sizeof(char));
    }

    char *token = strtok(cmd, " \0");

    while (token) {
        argv[idx++] = token;
        token = strtok(NULL, " \0");
    }
    argv[idx] = NULL;

    // Check and execute built-in commands
    int b_i_index = built_in_index(*argv);
    if(b_i_index > (-1)){
        (*(built_in_list[b_i_index]->func))(argv, fd, pipe_st);
        return 1;
    }

    // Else execute external command.
    pid_t child = fork();
    if (child == 0)  {
        
            // pipe I/O, pipe_st is a 2 bit value indicating the status bits
            // printf("Pipe_st is %d\n", pipe_st);
            if (pipe_st & READ_BIT) {
                // Read from the fd open as fd[0]
                // printf("Piping inp\n");
                dup2(fd[0], STDIN_FILENO);
            }
            if (pipe_st & WRITE_BIT) {
                // Write to fd open as fd[1]
                // printf("Piping outp\n");
                dup2(fd[1], STDOUT_FILENO);
            }
        
        // Child Here
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
            printf("*** ERROR: exec of %s failed\n", *argv);
            exit(errno);
        }

    }
    else {
        //Parent stuff, wait for child to terminate and return
        // with error handling
        int cstatus;
        // printf("Waiting here\n");
        pid_t rpid = waitpid(child, &cstatus, 0);  
        // printf("Finished waiting for cmd %s\n", cmd);
        if (rpid != child) {
            // Error handling
            return 0;
        }
        else {
            if (WIFEXITED(cstatus))
                return cstatus;
            else {
                return 0;
            } 
        }


    }
    
}

void pipe_cmd(char **argv) {

    int idx;
    int fd[2] = {STDIN_FILENO, STDOUT_FILENO};
    char pipemask = 3;
    int st_flag = 1;

    // pipe(fd);
    // //  int status = 
    // execute(argv[0], fd, 2);
    // execute(argv[1], fd, 1);

    for (idx = 0; argv[idx]; ++idx) {
        if (idx > 0)
            st_flag = 0;
        // Create a new pipe
        int temp[2];
        if (argv[idx + 1])
            pipe(temp);

        pipemask = 0b11;
        if (st_flag) 
            pipemask &= 0b10;
        if (!argv[idx + 1])
            pipemask &= 0b01;

        // Close previous command's write fd

        if (fd[1] != STDOUT_FILENO)
            close(fd[1]);
        fd[1] = temp[1];

        int status = execute(argv[idx], fd, pipemask); 

        // Post exec, the current prog. write desc. is next prog's read
        if (fd[0] != STDIN_FILENO)
            close(fd[0]);

        fd[0] = temp[0];
    }

    return;

}

int redir_cmd(char *cmd, char *fname, int pipemask) {
    int fd[2];
    if (pipemask == 1) {
        fd[0] = open(fname, O_RDONLY);
        if (fd[0] == -1)
            printf("File or dir does not exist");
            return -1;
    }
    else if (pipemask == 2) {
        fd[1] = open(fname, O_RDONLY | O_CREAT);
        if (fd[1] == -1)
            printf("Error opening file");
            return -1;
    }

    int status = execute(cmd, fd, pipemask);
    if (status > 0)
        return 0;
    else
        return 1;
}

int parse_inp(char *inp) {
    
    int idx = 0;
    char **argv_l = malloc(10 * sizeof(char *));
    // Redirection vs. pipe
    if (strchr(inp, '<')) {
        char *cmd = strtok(inp, "<\0");
        char *rd_fname = strtok(NULL, "<\0");
        if (cmd && rd_fname && !strtok(NULL, ">\0"))
            redir_cmd(cmd, rd_fname, 1);
        else {
            printf("Invalid command format\n");
            return -1;
        }

    }

    else if (strchr(inp, '>')) {
        char *cmd = strtok(inp, ">\0");
        char *wr_fname = strtok(NULL, ">\0");
        if (cmd && wr_fname && !strtok(NULL, ">\0"))
            redir_cmd(cmd, wr_fname, 2);
        else {
            printf("Invalid command format\n");
            return -1;
        }

    } 

    else {
        char *token = strtok(inp, "|\0");
        while(token) {
            argv_l[idx++] = token;
            token = strtok(NULL, "|\0");
        }
        argv_l[idx] = NULL;


        pipe_cmd(argv_l);

        return 0;
    }

}