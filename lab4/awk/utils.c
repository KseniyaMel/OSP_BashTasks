//
// Created by andrey.zavodov on 08.12.18.
//

#include <ctype.h>
#include "utils.h"

void message(char *type, char *msg){
    char message[strlen(type) + strlen(msg) + 5];
    strcpy(message, type);
    strcat(message, " - ");
    strcat(message, msg);

    write(STDERR_FILENO, message, strlen(message));

    if (strcmp(type, "ERROR") == 0) {
        exit(EXIT_FAILURE);
    }
}

int isnumber(const char* str){

    if (!str){
        return 0;
    }

    for (int i=0; i < strlen(str); i++){
        if (! isdigit(str[i])){
            return 0;
        }
    }

    return 1;
}
