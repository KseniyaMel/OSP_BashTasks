//
// Created by andrey.zavodov on 01.12.18.
//

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "utils.h"
#include "interpreter.h"

int main(int argc, char *argv[])
{

    program_state state;

    state.FS = ' ';
    state.RS = '\n';
    state.OFS = ' ';
    state.ORS = '\n';

    char *program = NULL;

    int iargc;
    for (iargc = 1; iargc < argc; iargc++) {

        if (strcmp(argv[iargc], "--") == 0)
            break;

        if (argv[iargc][0] == '-' && argv[iargc][1] == 'F') {
            if (argv[iargc][2]) {
                if (argv[iargc][3] == '\0') {
                    state.FS = argv[iargc][2];
                } else {
                    message("ERROR", "Too many characters specified for -F\n");
                }
            } else {
                iargc++;
                if (iargc != argc && strlen(argv[iargc]) == 1) {
                    state.FS = argv[iargc][0];
                } else {
                    message("ERROR", "No character specified for -F\n");
                }
            }
            continue;
        }

        if (!program && strcmp(argv[iargc], "-f") == 0) {
            iargc++;
            if (iargc != argc) {
                int fd = open(argv[iargc], O_RDONLY);

                if (fd < 0) {
                    perror(argv[iargc]);
                    message("ERROR", "File cannot be read\n");
                }

                struct stat fsbuf;
                if (fstat(fd, &fsbuf) < 0) {
                    perror(argv[iargc]);
                    message("ERROR", "File cannot be read\n");
                }

                if ((program = mmap(NULL, (size_t) fsbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
                    perror(argv[iargc]);
                    message("ERROR", "File cannot be read\n");
                }

                if (close(fd) < 0) {
                    perror(argv[iargc]);
                    message("ERROR", "File cannot be read\n");
                }
            } else {
                message("ERROR", "No file specified for -f\n");
            }
            continue;
        }

        if (!program && argv[iargc][0] == '\'') {

            program = (char*)malloc(strlen(argv[iargc]));

            do {
                program = (char*)realloc(program, strlen(program) + strlen(argv[iargc]));
                strcat(program, argv[iargc]);
                strcat(program, " ");
                iargc++;
            } while (program[strlen(program) - 2] != '\'');

            iargc--;
            continue;
        }

        break;

    }

    if (!program) {
        message("ERROR", "Usage: awk_by_zavodov [-Fc] [-f program.file | ' program '] file...\n");
    }


    if (iargc == argc || (iargc == (argc - 1) && argv[iargc][0] == '-')) {

        state.FILENO = STDIN_FILENO;
        do_awk(program, state);

    } else {

        for (; iargc < argc; iargc++) {
            int fd = open(argv[iargc], O_RDONLY);
            if (fd < 0) {
                perror(argv[iargc]);
                message("ERROR", "File cannot be read\n");
            }

            state.FILENO = fd;
            do_awk(program, state);

            if (close(fd) < 0) {
                message("ERROR", "File cannot be read\n");
            }
        }

    }

    return 0;

}
