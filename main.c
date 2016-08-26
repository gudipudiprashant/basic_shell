#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


#define BUFFER_SIZE 100

int pwd_func(char **);
void initialize(void);

struct command{
	char *name;
	int (*func)(char **);
		
};


struct command **built_in_list;
int built_in_size;
char *pwd;
struct command *pwd_com,*exit_com;

int main(){

	char *input;
	int i,is_built_in=0;
	input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	initialize();

	while(1){

		is_built_in = 0;
		//print the prompt
		printf("basic_shell:%s$ ",pwd);


		//get input
		fgets(input,BUFFER_SIZE-1,stdin);
		input = strtok(input,"\n");

		/*if((*input) == (*leave)){
			break;
		}
		*/

		for(i = 0;i<built_in_size;i++){
			if(strcmp(input,built_in_list[i]->name) == 0){
				(*(built_in_list[i]->func))(NULL); // NULL for now. Will have to add args pointer in place of void after tokening input
				is_built_in = 1;
			}
		}

		if(!is_built_in){
			printf("%s\n",input);	
		}
		
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



	//this must be at the end
	built_in_size = i;

}