#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "helper.h"
#include "invertedIndex.h"

static void divideByCount(InvertedIndexBST t, char *filename, int total);
static void actualPrint(InvertedIndexBST tree, FILE * fp);


//-------------------------Part 1----------------------------------
char *normaliseWord(char *str) {
    int i = 0;
    for (; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    i--;
    char punctuations[] = ".,;?";
    for (int j = 0; j < 4; j++) {
        if (str[i] == punctuations[j]){
            str[i] = '\0';
        }
    }
    return str;
}

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    InvertedIndexBST new = NULL;
    FILE *fp = fopen(collectionFilename, "r");
    if (fp == NULL) {
        printf("Couldnt open %s", collectionFilename);
        exit(1);
    }

    char filename[100];
    while (fscanf(fp,"%s", filename) == 1) {
        FILE *fp2 = fopen(filename, "r");
        if (fp2 == NULL) {
            printf("Couldnt open %s", filename);
            exit(1);
        }

        char word[100];
        int totalWordCount = 0;

        while (fscanf(fp2, "%s", word) == 1) {
            char *normalised = normaliseWord(word);
            new = treeInsert(new, normalised, filename);
            totalWordCount++;
        }
        //Currently, the tree doesn not contain the tf, we still need to divide it by the totalWordCount
        divideByCount(new, filename, totalWordCount);

        fclose(fp2);

    }

    fclose(fp);
    return new;
}

void printInvertedIndex(InvertedIndexBST tree) {
    FILE *fp = fopen("invertedIndex.txt", "w");
    actualPrint(tree, fp);
    fclose(fp);
}


//--------------------- Part 1 helper functions--------------------------------
// go to every node of the tree and divide t->fileList->tf by total
static void divideByCount(InvertedIndexBST t, char *filename, int total) {
    //base
    if (t == NULL) return;

    //go divide the tf by total if the filename exists for t
    FileList node = t->fileList;
    while (node != NULL) {
        if (strcmp(filename, node->filename) == 0) {
            node->tf = node->tf / total;
            break;
        } else if (strcmp(filename, node->filename) < 0) {
            break;
        }
        node = node->next;
    }

    divideByCount(t->left, filename, total);
    divideByCount(t->right, filename, total);
}

static void actualPrint(InvertedIndexBST tree, FILE * fp) {
    if (tree == NULL) return;
    actualPrint(tree->left, fp);
    fprintf(fp, "%s ", tree->word);
    FileList node = tree->fileList;
    while(node != NULL) {
        fprintf(fp, "%s ", node->filename);
        node = node->next;
    }
    fprintf(fp, "\n");
    actualPrint(tree->right, fp);
}


//--------------------------- Part 2 -----------------------------------
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D) {
    InvertedIndexBST treeNode = treeFind(tree, searchWord);
    if (treeNode == NULL) {
        //word was not found in tree
        return NULL;
    }
    FileList head = treeNode->fileList;
    double idf = log10((double)D/countListSize(head));
    TfIdfList new = NULL;
    while (head != NULL) {
        new = insertTfIdfList(new, head->filename, head->tf * idf);
        head = head->next;
    }
    return new;
}

TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D) {
    TfIdfList myList = calculateTfIdf(tree, searchWords[0], D);
    InvertedIndexBST treeNode;
    FileList node;
    double idf;

    for (int i = 1; searchWords[i] != NULL; i++) {
        treeNode = treeFind(tree, searchWords[i]);
        if (treeNode == NULL) continue;

        node = treeNode->fileList;
        idf = log10((double)D/countListSize(node));

        while (node != NULL) {
            if (inList(myList, node->filename) == 0) {
                myList = insertTfIdfList(myList, node->filename, node->tf * idf);
            } else {
                myList = sumAndReorder(myList, node->filename, node->tf * idf);
            }
            node = node->next;
        }

    }
    return myList;
}

