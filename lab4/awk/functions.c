//
// Created by andrey.zavodov on 14.12.18.
//

#include "functions.h"


char* _parse_argument(char* argument, program_state state){

    if (!argument){
        return "";
    }

    if (argument[0] == '$'){
        if (isnumber(argument + 1)){
            long number = strtol(argument + 1, (char **)NULL, 10);
            if (number < 0) {
                return argument;
            }
            if (number == 0){
                return state.RECORD;
            }
            if (number > state.NF){
                return "";
            }
            return state.FIELDS[number - 1];
        } else {
            // TODO add processing user variables
            return argument;
        }
    }

    return argument;

}

int print(token** tok_ptr, program_state state){

    (*tok_ptr) = (*tok_ptr)->next;
    token* tok = *tok_ptr;

    while (tok->token && strcmp(tok->token, "}") != 0 && !strstr(tok->separator, ";")) {
        char* parsed_argument = _parse_argument(tok->token, state);

        write(STDOUT_FILENO, parsed_argument, strlen(parsed_argument));

        if (strstr(tok->separator, ",")){
            write(STDOUT_FILENO, " ", strlen(" "));
        }

        (*tok_ptr) = (*tok_ptr)->next;
        tok = *tok_ptr;
    }

    write(STDOUT_FILENO, &state.ORS, strlen(&state.ORS));

    return 0;
}
