#include "hy345sh.h"

#include <limits.h>


int main(){
   
    printf(strcat(command_prompt(), "\n"));
    return 0;
}

char* command_prompt(){
    char cwd[PATH_MAX];

    char* user = getlogin();
    char* dir = getcwd(cwd, sizeof(cwd));
    char* am = "csd5452";

    char* prompt = malloc(5*PATH_MAX*sizeof(char));
    if(prompt){
        strcat(prompt, user);
        strcat(prompt, "@");
        strcat(prompt, am);
        strcat(prompt, "-hy345sh:");
        strcat(prompt, dir);
        strcat(prompt, "$");

    }
    else{
        printf("malloc failed\n Exit");
        exit(-1);
    }
    return prompt;
    return 0;
}

void execute(char** args){

}