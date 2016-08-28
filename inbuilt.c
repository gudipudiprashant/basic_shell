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
char * his_n(int n){
    int tail = cur_p->tail, head = cur_p->head;
    int x=(tail-head+HIS_SIZE+1)%(HIS_SIZE+1);
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
