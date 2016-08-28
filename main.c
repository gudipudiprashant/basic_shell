#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

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



int pwd_func(char **);
int exit_func(char **);
int history_func(char **);
int cd_func(char **);


void maintain_his(const char*);
void print_his(void);


void initialize(void);

struct command{
	char *name;
	int (*func)(char **);
		
};

char *input;
struct command **built_in_list;
int built_in_size;
char *pwd;
struct command *pwd_com,*exit_com,*history_com, *cd_com;
queue_p *cur_p;
char ** ar_his, **args; // circular queue to maintain history

int main(){

	int i,is_built_in=0,token_pos = 0,no_args = 0;
	char *token;
	ar_his = (char **)malloc(sizeof(char *)*(HIS_SIZE+1));
	cur_p = (queue_p *)malloc(sizeof(queue_p));

	pid_t pid;
	int status;

	initialize();



	while(1){

		input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
		args = (char **)malloc(sizeof(char *)*(MAX_TOKENS));
		is_built_in = 0;
		token_pos = 0;
		no_args = 0;
		token = NULL;
		// how to clear args?


		//print the prompt
		printf("basic_shell:%s$ ",pwd);


		//get input
		fgets(input,BUFFER_SIZE-1,stdin);
		if(strcmp(input,"\n") == 0){
			continue;
		}
		input = strtok(input,"\n");

		maintain_his(input);

		token = strtok(input,TOKEN_DELIM); //input will get modified
		no_args++;
		while(token!=NULL){
			args[token_pos] = token;
			token_pos++;
			no_args++;
			token = strtok(NULL,TOKEN_DELIM); //strtok maintains a static pointer
		}
		args[token_pos] = NULL;


		for(i = 0;i<built_in_size;i++){
			if(strcmp(input,built_in_list[i]->name) == 0){
				(*(built_in_list[i]->func))(args); // NULL for now. Will have to add args pointer in place of void after tokening input
				is_built_in = 1;
			}
		}


		if(!is_built_in){

			pid = fork();
			if(pid == 0){
				//in the child process
				execvp(args[0],args);
				exit(0);
			}
			else if(pid > 0){
				wait(&status);
			}


			//print command
			// printf("\nCommand: ");
			// token_pos = 0;

			// while(args[token_pos]!=NULL){
			// 	printf("%s ",args[token_pos]);
			// 	token_pos++;
			// }
			// printf("\n");

		}
		
		free(input);
		free(args); 
		//only freeing the array coz the other pointers 
		//point to locations in input block and that is already freed.
	}

	return 0;
}

int pwd_func(char **args){
	pwd = getcwd(pwd,BUFFER_SIZE-1);
	printf("%s\n",pwd);
	return 0;
}

int exit_func(char **args){
	exit(0);
}

int history_func(char **args){
	print_his();
	return 0;
}

int cd_func(char **args){
	if(args[1] != NULL){
		chdir(args[1]);
		
		pwd_func(NULL);
		return 0;
	}
}

// returns the changed values of head and tail
void maintain_his(const char* cmd ){
	static int head=0, tail=0, flag=0;
	char *temp;
	int t;

	temp = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	temp = strcpy(temp,cmd);


	if (( tail-head )==( HIS_SIZE)){
		flag = 1;
	}
	
	//int check=(tail-head+HIS_SIZE)%HIS_SIZE;
	// instead can store a static variable flag which once set implies queue is full

	ar_his[tail]=(char *)temp;

	if(flag){
		// freeing the oldest command
		free(ar_his[head]);
		head=(head+1)%(HIS_SIZE+1);
		tail=(tail+1)%(HIS_SIZE+1);
		// circular increment
	}

	else
		tail++;

	cur_p->tail=tail;
	cur_p->head=head;
}

// returns the nth command from starting
char * his_n(queue_p* ptr, int n){
	int tail = ptr->tail, head = ptr->head;
	int x=(tail-head+HIS_SIZE)%HIS_SIZE;
	n--;

	if(n<=x)
		return ar_his[head+n];

	else
		return NULL;
	//  if given n doesn't lie in the range, returns NULL
}

// prints the entire history from 1 to HIS_SIZE(oldest to newest)
void print_his(void){
	int i=1,j=cur_p->head;
	while(j!=cur_p->tail){
		printf("%d %s\n",i,ar_his[j]);
		
		j=(j+1)%(HIS_SIZE+1);
		i++;

	}

}

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