// BSTree.h ... interface to binary search tree ADT

#ifndef BSTREE_H
#define BSTREE_H
#include "invertedIndex.h"
FileList newFileListNode(char *name);
void insertFileListNode(InvertedIndexBST t,char *filename);
int countListSize(FileList head);
InvertedIndexBST newTreeNode(char *str, char* filename);
InvertedIndexBST treeInsert(InvertedIndexBST t, char *str, char* filename);
InvertedIndexBST treeFind(InvertedIndexBST t, char *str);
TfIdfList newTfIdfList(char *filename, double tfIdf);
TfIdfList insertTfIdfList(TfIdfList list,char *filename, double tfIdf);
TfIdfList sumAndReorder(TfIdfList list,char *filename, double tfIdf);
TfIdfList reAdd(TfIdfList list, TfIdfList node);
int inList(TfIdfList list, char *filename);
char *my_strdup(char *str);
#endif

