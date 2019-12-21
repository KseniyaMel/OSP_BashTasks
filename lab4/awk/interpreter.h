//
// Created by andrey.zavodov on 07.12.18.
//

#ifndef AWK_INTERPRETER_H
#define AWK_INTERPRETER_H

#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "utils.h"

typedef struct {
    int FILENO;
    char FS;        // fields separator
    char RS;        // rows separator
    size_t NF;         // number of fields
    size_t NR;         // number of rows
    char OFS;       // output fields separator
    char ORS;       // output rows separator
    char *RECORD;   // current record
    char **FIELDS;  // fields in current record
} program_state;

typedef struct {
    char** array;
    size_t size;
} string_array_t;

typedef struct token{
    struct token* prev;

    char* token;
    char* separator;

    struct token* next;
} token;

int do_awk(char* program, program_state state);

#endif //AWK_INTERPRETER_H
