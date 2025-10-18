#ifndef HY345SH_H
#define HY345SH_H

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>
#include <ctype.h>


char* command_prompt();

void execute(char** args);

void parse_input(char* input);

void if_hanlder(char* input);

void for_handler(char* input);

#endif // HY345SH_H