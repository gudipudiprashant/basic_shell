#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"main.h"
#include"inbuilt.c"

#define PRINT_CMD 0


int main(){

	pid_t pid;
	int status;

	int i,is_inp_empty,b_i_index,no_args;

	initialize();

	//need to be done only once in the program.
	ar_his = (char **)malloc(sizeof(char *)*(HIS_SIZE+1));
	cur_p = (queue_p *)malloc(sizeof(queue_p));



	while(1){

		no_args = 0;

		//print the prompt
		printf("basic_shell:%s$ ",pwd);
		
		//get command entered into memory pointed by input
		is_inp_empty = get_input();

		if(is_inp_empty){
			continue;
		}
		
		access_history(input);


		//add input into history
		maintain_his(input);

		//parse input. input string is modified.
		no_args = parse_input();

		//b_i_index is -1 if not built-in.
		b_i_index = built_in_index(args[0]);

		//built_in_list contains all structs of built-in-commands.
		if(b_i_index > (-1)){
			(*(built_in_list[b_i_index]->func))(args);
		}
		else {

			pid = fork();
			if(pid == 0){
				//in the child process
				execvp(args[0],args);
				exit(0);
			}
			else if(pid > 0){
				wait(&status);
			}

		}
		
		if(PRINT_CMD){
			print_command();
		}


		free(input);
		free(args); 
		//only freeing the array coz the other pointers 
		//point to locations in input block and that is already freed.
	}

	return 0;
}


int get_input(void){

	//takes input from the user.
	//returns 1 if the string is empty.
	//returns 0 if the string is not-empty.
	
	input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	
	fgets(input,BUFFER_SIZE-1,stdin);

	//fgets has \n in the end of the input string.

	//check for empty string
	if(strcmp(input,"\n") == 0){
		return 1;
	}

	input = strtok(input,"\n"); //removes the \n at the end of the string.

	return 0;
	
}

int parse_input(void){
	//creates a array of pointers to tokens.
	//No extra memory is used to store tokens. The input string is modified.
	//All pointers point to the memory block of input.
	//returns number of arguments.

	int no_args = 0;
	char *token = NULL;

	args = (char **)malloc(sizeof(char *)*(MAX_TOKENS));

	token = strtok(input,TOKEN_DELIM); 

	while(token!=NULL){
		args[no_args] = token;
		no_args++;
		token = strtok(NULL,TOKEN_DELIM); //strtok maintains a static pointer,hence NULL
	}
	args[no_args] = NULL;

	return no_args;

}

void access_history(char *cmd_name){
	int last_cmd_index;
	char *temp_1,*temp_2; 
	int val;
	temp_1 = (char *)malloc(3*sizeof(char));
	strcpy(temp_1,"!!");


	if(strcmp(cmd_name,temp_1) == 0){
		free(input);
		input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
		last_cmd_index = (cur_p->tail + HIS_SIZE)%(HIS_SIZE + 1);
		strcpy(input,ar_his[last_cmd_index]);
		return;
	}
	else if((cmd_name[0]) == '!'){
		val = atoi(&cmd_name[1]);
		free(input);
		input = (char *)malloc(sizeof(char)*BUFFER_SIZE);
		temp_2 = his_n(val);
		strcpy(input,temp_2);
		return;
	}
	else{
		return;
	}
}

int built_in_index(char *cmd_name){
	//Checks if input command is built-in.
	//returns index if belongs to list.
	//returns -1 if not in list.

	int i;

	for(i = 0;i<built_in_size;i++){
		if(strcmp(cmd_name,built_in_list[i]->name) == 0){
			//(*(built_in_list[i]->func))(args);
			return i;
		}
	}

	return -1;
}

void print_command(void){

	int token_pos = 0;

	printf("\nCommand: ");

	while(args[token_pos]!=NULL){
		printf("%s ",args[token_pos]);
		token_pos++;
	}
	printf("\n");
}