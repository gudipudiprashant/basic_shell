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
    int (*func)(char **);
        
};


int pwd_func(char **);
int exit_func(char **);
int history_func(char **);
int cd_func(char **);


void maintain_his(const char*);
void print_his(void);


void initialize(void);



char *input;
struct command **built_in_list;
int built_in_size;
char *pwd;
struct command *pwd_com,*exit_com,*history_com, *cd_com;
queue_p *cur_p;
char ** ar_his, **args; // circular queue to maintain history


void initialize(void){
    pwd = (char *)malloc(sizeof(char)*BUFFER_SIZE);
    int i=0;

    pwd = getcwd(pwd,BUFFER_SIZE-1);
    //this needs to be modified whenever a built-in-command is added
    built_in_list = (struct command **)malloc(2*sizeof(struct command *));


    pwd_com = (struct command *)malloc(1*sizeof(struct command));
    pwd_com->name = "pwd";
    pwd_com->func = &pwd_func;
    built_in_list[i] = pwd_com;
    i++;

    exit_com = (struct command *)malloc(sizeof(struct command));
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