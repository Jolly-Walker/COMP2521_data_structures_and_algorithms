
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

#define MAX_LINE_LEN 1000

static int notTerm(char* word, char **searchTerms, int totalTerms);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./searchPagerank <search terms>\n");
        exit(1);
    }
    char **searchTerms = malloc(sizeof(searchTerms) * argc);
    for (int i = 1; i < argc; i++) {
        searchTerms[i - 1] = argv[i];
    }

    char *read = malloc(sizeof(read));
    char *fileName = malloc(sizeof(fileName));
    int num;
    double val;
    FILE *rankP = fopen("pagerankList.txt", "r");
    if (rankP == NULL) {
        fprintf(stderr, "Couldnt open %s\n", "searchPagerank.txt");
        exit(1);
    }

    int fileCount = 0;
    Rank rankList = NULL;
    while (fgets(read, MAX_LINE_LEN, rankP) != NULL) {
        sscanf(read, "%s %d, %lf", fileName, &num, &val);
        //chomping the filename
        fileName[strlen(fileName) - 1] = '\0';
        Rank new = newRank(my_strdup(fileName), fileCount);
        new->next = rankList;
        rankList = new;
        fileCount++;
    }

    int termsCount = argc - 1;

    FILE *fp = fopen("invertedIndex.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldnt open %s\n", "invertedIndex.txt");
        exit(1);
    }

    char *line = malloc(sizeof(line));
    char *word = malloc(sizeof(word));
    List myList = NULL;
    char *item = malloc(sizeof(item));
    while (fgets(line, MAX_LINE_LEN, fp) != NULL) {

        word = strtok(line," \n");
        if (notTerm(word, searchTerms, termsCount)) continue;
        word = strtok(NULL," \n");
        while (word != NULL) {
            myList = insertList(myList, word, rankList);
            word = strtok(NULL, " \n");
        }
    }

    List node = myList;
    int i = 0;
    while (node != NULL) {
        if (i == 30) break;
        printf("%s\n", node->urlName);
        node = node->next;
        i++;
    }

    return 0;
}

//helper function
static int notTerm(char* word, char **searchTerms, int totalTerms) {
    for (int i = 0; i < totalTerms; i++) {
        if (strcmp(word, searchTerms[i]) == 0) {
            return 0;
        }
    }
    return 1;
}
