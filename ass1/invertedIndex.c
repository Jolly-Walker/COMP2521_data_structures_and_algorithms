#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "invertedIndex.h"

char *normaliseWord(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

