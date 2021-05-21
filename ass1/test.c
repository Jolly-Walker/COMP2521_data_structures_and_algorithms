#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "invertedIndex.h"
#include "invertedIndex.h"

int main (void) {
    char *str = malloc(sizeof(char) * 100);
    strcpy(str, "Hello There.");
    str = normaliseWord(str);
    printf("%s \n", str);
    return 0;
}

