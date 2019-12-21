//
// Created by andrey.zavodov on 07.12.18.
//

#include "interpreter.h"
#include "parser.h"

string_array_t str_split(const char *b_str, const char a_delim) {
    char a_str[strlen(b_str)];
    strcpy(a_str, b_str);
    char *tmp = a_str;

    char **result = 0;
    size_t count = 0;

    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    string_array_t arr;
    arr.array = result;
    arr.size = count - 1;

    return arr;
}


int do_awk(char* code, program_state state){

    state.NF = 0;
    state.NR = 0;
    state.FIELDS = NULL;
    state.RECORD = NULL;

    program pr;
    pr = parse_blocks(code);

    execute_block(pr.begin_block, state);
    free_structure(pr.begin_block);
    pr.begin_block = NULL;

    struct stat fsbuf;
    if (fstat(state.FILENO, &fsbuf) < 0) {
        message("ERROR", "File cannot be read\n");
    }

    char *buf = (char*)calloc(1, 1);
    size_t buf_size = sizeof(*buf);

    for (size_t offset = 0; offset < fsbuf.st_size; ){

        char *record = (char*)calloc(1, 1);

        do{
            read(state.FILENO, buf, buf_size);
            offset += buf_size;
            record = (char*)realloc(record, strlen(record) + buf_size);
            strcat(record, buf);
        } while (buf[0] != state.RS && offset < fsbuf.st_size);

        if (buf[0] == state.RS){
            record[strlen(record) - 1] = '\0'; // remove separator
        }

        state.RECORD = record;
        state.NR++;

        string_array_t arr = str_split(state.RECORD, state.FS);
        state.FIELDS = arr.array;
        state.NF = arr.size;

        execute_block(pr.main_block, state);

    }

    free_structure(pr.main_block);
    pr.main_block = NULL;

    execute_block(pr.end_block, state);
    free_structure(pr.end_block);
    pr.end_block = NULL;

    return 0;

}
