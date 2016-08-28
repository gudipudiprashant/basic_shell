#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"main.h"
#include"inbuilt.c"



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
			printf("\nCommand: ");
			token_pos = 0;

			while(args[token_pos]!=NULL){
				printf("%s ",args[token_pos]);
				token_pos++;
			}
			printf("\n");

		}
		
		free(input);
		free(args); 
		//only freeing the array coz the other pointers 
		//point to locations in input block and that is already freed.
	}

	return 0;
}


