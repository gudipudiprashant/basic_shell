#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

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
    // printf("For cmd %s\n", cmd);
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

int main() {
    char inp[50];
    fgets(inp, 49, stdin);
    inp[strcspn(inp,  "\n")] = 0;
    
    int idx = 0;
    char **argv_l = malloc(10 * sizeof(char *));
    char *token = strtok(inp, "|\0");
    while(token) {
        argv_l[idx++] = token;
        token = strtok(NULL, "|\0");
    }
    argv_l[idx] = NULL;

    pipe_cmd(argv_l);

    return 0;

}











// int main() {
//     char inp[50];
//     fgets(inp, 49, stdin);
//     char (*(*argv_l))[10] = malloc(10 * sizeof(char *[10]));
//     *argv_l = malloc(100 * sizeof(char));
//     int idx = 0;
//     char *token;
//     token = strtok(inp, " \0");

//     while (token) {
//         argv_l[idx] = (*argv_l) + idx;
//         strcpy(*argv_l[idx++], token);

//         token = strtok(NULL, " \0");
//     }

//     argv_l[idx] = NULL;

//     for (idx = 0; argv_l[idx]; ++idx) {
//         printf("%s\n", (*argv_l)[idx]);
//     }
//     return 0;
// }





// int main() {
//     char inp[50];
//     fgets(inp, 49, stdin);
//     char (*(*argv_l))[10] = malloc(10 * sizeof(char *[10]));
//     *argv_l = malloc(100 * sizeof(char));
//     int idx = 0;
//     char *token;
//     token = strtok(inp, " \0");

//     while (token) {
//         strcpy((*argv_l)[idx++], token);

//         token = strtok(NULL, " \0");
//     }

//     argv_l[idx] = NULL;

//     for (idx = 0; argv_l[idx]; ++idx) {
//         printf("%s\n", (*argv_l)[idx]);
//     }
//     return 0;
// }