#include "hy345sh.h"


void pipeline(char* input){

}

void execute(char** args){
    if(args[0] != NULL){
        char* command = args[0];
        char* eq = strchr(command, '=');

        //handle global variables
        if((eq != NULL) && (eq[0] == '=')){
                *eq = '\0';
                char* var = command;
                char* value = eq + 1;
                setenv(var, value, 1);
                return;
        }


        //hande cd input 
        if(strcmp(command, "cd") == 0){
            
            if(args[1] == NULL){
                char* home = getenv("HOME");
                if(home == NULL) 
                    home = "/";
            
                if(chdir(home) != 0){
                    perror("cd");
                }
            }
            else{
                if(chdir(args[1]) != 0){
                    perror("cd");
                }
            }
            return;
            
        }

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
            if(token[0] == '$'){
                char* varName = token + 1;
                char* varValue = getenv(varName);
                if(varValue){
                    args[position] = varValue;
                }
                else{
                    args[position] = strdup("");
                }
            }
            else{
                args[position] = token;
            }
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

