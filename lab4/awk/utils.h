//
// Created by andrey.zavodov on 08.12.18.
//

#ifndef AWK_UTILS_H
#define AWK_UTILS_H

#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void message(char *type, char *msg);
int isnumber(const char* str);

#endif //AWK_UTILS_H
