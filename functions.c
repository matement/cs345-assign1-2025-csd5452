#include "hy345sh.h"


void pipeline(char* input) {
    int n = 1;
    for (int i = 0; input[i]; i++)
        if (input[i] == '|') n++;

    char** commands = malloc(n * sizeof(char*));
    if (!commands) {
        perror("malloc commands");
        exit(EXIT_FAILURE);
    }

    // ✅ duplicate input to avoid destroying original
    char *temp = strdup(input);
    if (!temp) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    // ✅ split input into individual commands
    char *saveptr;
    char *command = strtok_r(temp, "|", &saveptr);
    int i = 0;
    while (command != NULL && i < n) {
        while (*command == ' ') command++; // trim leading spaces
        commands[i++] = command;
        command = strtok_r(NULL, "|", &saveptr);
    }

    int (*pipes)[2] = NULL;
    if (n > 1) {
        pipes = malloc((n - 1) * sizeof(int[2]));
        if (!pipes) {
            perror("malloc pipes");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n - 1; i++)
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
    }

    // ✅ create child processes
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // child
            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);
            if (i < n - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            if (n > 1) {
                for (int j = 0; j < n - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            // tokenize arguments
            char* args[100];
            char* argsptr;
            char* token = strtok_r(commands[i], " \t", &argsptr);
            int k = 0;
            while (token != NULL && k < 99) {
                args[k++] = token;
                token = strtok_r(NULL, " \t", &argsptr);
            }
            args[k] = NULL;

            if (args[0] == NULL) exit(EXIT_SUCCESS);

            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // ✅ parent closes pipes and waits
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }

    for (int i = 0; i < n; i++)
        wait(NULL);

    free(commands);
    free(temp);
    free(pipes);
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

        if(strchr(command, '|')){
            pipeline(command);
            command = strtok_r(NULL, ";", &saveptrcommand);
            continue;
        }

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

