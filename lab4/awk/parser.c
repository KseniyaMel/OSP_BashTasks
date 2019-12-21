//
// Created by andrey.zavodov on 10.12.18.
//

#include "parser.h"

char* _get_block(char** code_ptr, char start_br, char end_br){

    int br = 0;
    char* block_start = NULL;
    char* block_end = NULL;
    char* code = *code_ptr;

    for (int i = 0; i < strlen(code); i++){
        if (code[i] == start_br) {
            br++;
            if (br == 1) {
                block_start = (code + i);
            }
        }
        if (code[i] == end_br && br > 0) {
            br--;
            if (br == 0) {
                block_end = (code + i);
                break;
            }
        }
    }

    if (!block_start || !block_end){
        return NULL;
    }

    size_t len = block_end - block_start + 1;
    char* new_string = (char*)malloc(len);

    if (strncpy(new_string, block_start, len)) {
        *code_ptr = block_end;
        return new_string;
    } else {
        return NULL;
    }
}

void _merge_literals(token* start_token){

    token* current_token = start_token;

    while (current_token->token){
        if (current_token->token[0] == '"' && current_token->token[strlen(current_token->token)-1] != '"'){

            if (!current_token->next || !current_token->next->token){
                message("ERROR", "Invalid sequence of brackets.");
            }

            do {

                char* new_str = (char*)malloc(strlen(current_token->token) + strlen(current_token->next->token) + 1);
                strcat(new_str, current_token->token);
                strcat(new_str, current_token->separator);
                strcat(new_str,current_token->next->token);

                current_token->token = new_str;
                current_token->separator = current_token->next->separator;
                token* tmp_tok = current_token->next;
                current_token->next = current_token->next->next;


            } while (current_token->token[strlen(current_token->token)-1] != '"' && current_token->next);

        } else {
            current_token = current_token->next;
        }
    }

}

token* _parse_code_block(char* code){

    if (!code){
        return NULL;
    }

    char code_copy[strlen(code)];
    strcpy(code_copy, code);

    char separators[] = " ;,\t\n";

    token* start_token = (token*)malloc(sizeof(token));
    start_token->prev = NULL;
    start_token->token = strtok(code, separators);
    start_token->separator = "";

    token* current_token = start_token;
    while (current_token->token) {
        token* new_token = (token*)malloc(sizeof(token));

        new_token->prev = current_token;
        new_token->token = strtok(NULL, separators);
        new_token->next = NULL;
        new_token->separator = "";

        if (new_token->token){
            char* start = code_copy + (current_token->token + strlen(current_token->token) - code);
            char* end = code_copy + (new_token->token - code);
            size_t len = end - start;
            char* new_sep = (char*)malloc(len);
            current_token->separator = strncpy(new_sep, start, len);
        }

        current_token->next = new_token;
        current_token = new_token;
    }

    _merge_literals(start_token);

    return start_token;
}

void free_structure(token* tok){

    while(tok && tok->next){
        tok = tok->next;
        free(tok->prev);
    }
    free(tok);

}

program parse_blocks(char* code){

    program pr;

    char* begin_block = strstr(code, "BEGIN");
    if (begin_block != NULL){
        code = begin_block;
        pr.begin_block = _parse_code_block(_get_block(& code, '{', '}'));
    } else {
        pr.begin_block = NULL;
    }

    pr.main_block = _parse_code_block(_get_block(& code, '{', '}'));

    char* end_block = strstr(code, "END");
    if (end_block != NULL){
        code = end_block;
        pr.end_block = _parse_code_block(_get_block(& code, '{', '}'));
    } else {
        pr.end_block = NULL;
    }

    return pr;

}

int _execute_token(token** tok_ptr, program_state state){

    token* tok = *tok_ptr;

    if (strcmp(tok->token, "print") == 0){
        return print(tok_ptr, state);
    } else {
        (*tok_ptr) = tok->next;
        return 0;
    }

}

int execute_block(token* tok, program_state state){

    while (tok && tok->token){

        int status = _execute_token(&tok, state);

        if (status){
            return status;
        }

    }

    return 0;
}
