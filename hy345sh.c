#include "hy345sh.h"

#define true 1
#define false 0

#include <limits.h>


int main(){
    char* input = NULL;
    size_t len = 0;


    while(true){
        printf("%s", command_prompt());
        ssize_t read = getline(&input, &len, stdin);

        if(read == -1){
            printf("Error reading input\n");
            exit(-1);
        }

        input[strcspn(input, "\n")] = '\0';
        parse_input(input);
        
    }
}
