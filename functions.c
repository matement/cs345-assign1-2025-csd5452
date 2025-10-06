#include "hy345sh.h"


void execute(char** args){
    if(args[0] != NULL){
        char* command = args[0];
        int pid = fork();

        if(pid == 0){
            execvp(command, args);
        }
        else if(pid>0){
            waitpid(pid, NULL, 0);
        }
        else{
            printf("Fork failed\n");
            exit(-1);
        }
    }
    else{
        printf("No command given\n");
    }
}

void parse_input(char* input){
    char* saveptrtoken;
    char* saveptrcommand;

    char* command = strtok_r(input, ";", &saveptrcommand);
    /*split the string to ;*/
    while(command != NULL){
        char** args = malloc(64 * sizeof(char*));
        char* token;
        int position = 0;
        if(!args){
            printf("malloc failed\n Exit");
            exit(-1);
        }
        token = strtok_r(command, " ", &saveptrtoken);
        /*put all the arguments in an array*/
        while(token != NULL){
            args[position] = token;
            position++;
            token = strtok_r(NULL, " ", &saveptrtoken);
        }
        args[position] = NULL;
        if(args[0] != NULL){
            execute(args);
        }
        free(args);

        command = strtok_r(NULL, ";", &saveptrcommand);
    }
}

char* command_prompt(){
    char cwd[PATH_MAX];

    char* user = getlogin();
    char* dir = getcwd(cwd, sizeof(cwd));
    char* am = "csd5452";

    char* prompt = malloc(PATH_MAX + 128);
    if(prompt)
        snprintf(prompt, PATH_MAX + 128,"%s@%s-hy345sh:%s$ ", user, am, dir);
    else{
        printf("malloc failed\n Exit");
        exit(-1);
    }
    return prompt;
}

