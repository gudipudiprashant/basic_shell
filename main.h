#include<ctype.h>

#define BUFFER_SIZE 100
#define HIS_SIZE 10
#define MAX_TOKENS 50
#define TOKEN_DELIM " \t\r\n\a"



// queue variables head and tail
typedef struct
{
    int tail;
    int head;
} queue_p;

struct command{
    char *name;
    int (*func)(char **, int [], int);
        
};


void initialize(void);

//built in functions
int pwd_func(char **, int [], int);
int exit_func(char **, int [], int);
int history_func(char **, int [], int);
int cd_func(char **, int [], int);

//history related functions
void maintain_his(const char*);
void print_his(int [], int);
char *his_n(int);

//execution functions
int get_input(void);
void access_history(char *);
int built_in_index(char *);
void print_command(void);

//built-in related:
struct command **built_in_list;
int built_in_size;
struct command *pwd_com,*exit_com,*history_com, *cd_com;

//history related:
queue_p *cur_p;
char ** ar_his; // circular queue to maintain history

//input related:
char *input;
char **args;

char *pwd;


void initialize(void){

    pwd = (char *)malloc(sizeof(char)*BUFFER_SIZE);
    int i=0;

    pwd = getcwd(pwd,BUFFER_SIZE-1);

    //MODIFY SIZE WHEN A NEW COMMAND IS ADDED
    built_in_list = (struct command **)malloc(4*sizeof(struct command *));


    pwd_com = (struct command *)malloc(1*sizeof(struct command));
    pwd_com->name = "pwd";
    pwd_com->func = &pwd_func;
    built_in_list[i] = pwd_com;
    i++;

    exit_com = (struct command *)malloc(1*sizeof(struct command));
    exit_com->name = "exit";
    exit_com-> func = &exit_func;
    built_in_list[i] = exit_com;
    i++;

    history_com = (struct command *)malloc(1*sizeof(struct command));
    history_com->name = "history";
    history_com->func = &history_func;
    built_in_list[i] = history_com;
    i++;

    cd_com = (struct command *)malloc(1*sizeof(struct command));
    cd_com->name = "cd";
    cd_com->func = &cd_func;
    built_in_list[i] = cd_com;
    i++;


    //this must be at the end
    built_in_size = i;

}

// Trim leading and trailing whitespaces for
// the string.
int trimSpc(char *str) {
    if (!str)
        return -1;
    char *end;
    char *start = str;

    // Trim leading space
    while(isspace(*str)) 
        str++;

    if(*str == 0)  // All spaces?
        return 0;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) 
        end--;

    // Write new null terminator
    *(end+1) = 0;

    int sz = 0;
    while(*str) {
        *start = *str;
        str++; start++;
        sz++;
    }

    *start = 0;

    return sz;
}
