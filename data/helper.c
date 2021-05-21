// BSTree.c ... implementation of binary search tree ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "helper.h"
#include "invertedIndex.h"


//makes a new FileListNode pointer
FileList newFileListNode(char *name) {
	FileList new = malloc (sizeof *new);
	if (new == NULL) {
		printf("Malloc failed\n");
		exit(1);
	}
	new->filename = my_strdup(name);
	new->tf = 1.0;
	new->next = NULL;
	return new;
}

//inserting new FileListNode, if the node already exists then increase the wordcount
void insertFileListNode(InvertedIndexBST t,char *filename) {
	//checking if list already has the node with filename
	FileList curr = t->fileList;
	while (curr != NULL) {
		if (strcmp(filename, curr->filename) == 0) {
			curr->tf += 1;
			return;
		}
		curr = curr->next;
	}
	//Node does not exist, so we add a new node
	FileList list = t->fileList;
	FileList node = newFileListNode(filename);

	//empty insert
	if (list == NULL) {
		t->fileList = node;
	}

	//head insert
	if (strcmp(node->filename, list->filename) < 0) {
		node->next = list;
		t->fileList = node;
	} else {
		FileList prev = list;
		curr = list->next;
		while (prev != NULL) {
			//tail insert
			if (curr == NULL && strcmp(node->filename, prev->filename) > 0) {
				prev->next = node;
				break;
			}
			//normal insert
			if (strcmp(node->filename,curr->filename) < 0) {
				prev->next = node;
				node->next = curr;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}

}

//well its self explanatory, this fuction counts the list size
int countListSize(FileList head) {
	int count = 0;
	while (head != NULL) {
		count++;
		head = head->next;
	}
	return count;
}

//Makes a new tree node containing with the word given
InvertedIndexBST newTreeNode(char *str, char* filename) {
	InvertedIndexBST new = malloc (sizeof *new);
	if (new == NULL) {
		printf("Malloc failed\n");
		exit(1);
	}
	new->word = my_strdup(str);
	new->fileList = newFileListNode(filename);
	new->left = new->right = NULL;
	return new;
}

//Inserts new words into a InvertedIndexBST tree, if word is already there, increase word count
InvertedIndexBST treeInsert(InvertedIndexBST t, char *str, char* filename) {
	if (t == NULL) {
		return newTreeNode(str, filename);
	} else if (strcmp(str, t->word) < 0) {
		t->left = treeInsert (t->left, str, filename);
	} else if (strcmp(str, t->word) > 0) {
		t->right = treeInsert (t->right, str, filename);
	} else {
		//means the word is already in the tree
		insertFileListNode(t, filename);
	}
	return t;
}

//Finds a word in the tree and return the node, if word not in tree return NULL
InvertedIndexBST treeFind(InvertedIndexBST t, char *str) {
	if (t == NULL) return NULL;

	if (strcmp(str, t->word) < 0) {
		return treeFind(t->left, str);
	} else if (strcmp(str, t->word) > 0) {
		return treeFind(t->right, str);
	} else {
		return t;
	}

}

//makes a newTfIdfListNode
TfIdfList newTfIdfList(char *filename, double tfIdf) {
	TfIdfList new = malloc(sizeof *new);
	new->filename = filename;
	new->tfidf_sum = tfIdf;
	new->next = NULL;

	return new;
}

//inserts in order
TfIdfList insertTfIdfList(TfIdfList list,char *filename, double tfIdf) {
	if (list == NULL) return newTfIdfList(filename, tfIdf);
	TfIdfList new = newTfIdfList(filename, tfIdf);

	//adding at head of list
	if (tfIdf > list->tfidf_sum) {
		new->next = list;
		return new;
	} else if (tfIdf <= list->tfidf_sum && list->next == NULL) {
		//if only 1 item in list
		if (tfIdf == list->tfidf_sum) {
			if (strcmp(filename, list->filename) < 0) {
				new->next = list;
				return new;
			}
		}
		list->next = new;

		return list;
	} else {

		TfIdfList prev = list;
		TfIdfList node = list->next;
		//If the list head has same tfIdf value
		if (tfIdf == prev->tfidf_sum && strcmp(filename, prev->filename) < 0) {
			new->next = prev;
			return new;
		}

		while (node != NULL) {
			//if they equal, and strcmp < 0
			if (tfIdf == node->tfidf_sum) {
				if (strcmp(filename, node->filename) < 0) {
					prev->next = new;
					new->next = node;
					break;
				}
			}

			//insertNormal
			if (prev->tfidf_sum >= tfIdf && node->tfidf_sum < tfIdf) {
				prev->next = new;
				new->next = node;
				break;
			}

			//if we already reach the end of the list
			if (node->next == NULL) {
				node->next = new;
				break;
			}
			prev = node;
			node = node->next;
		}
		return list;
	}
}

//This function must only be called when filename exists is already in the list
//it sums the tfIdf of the node containing filename, and then reOrders the list
TfIdfList sumAndReorder(TfIdfList list,char *filename, double tfIdf) {
	TfIdfList head = list;
	TfIdfList prev = list;
	TfIdfList node = list->next;

	if (strcmp(filename, head->filename) == 0) {
		head->tfidf_sum += tfIdf;
	} else {
		while (node != NULL) {
			if (strcmp(filename, node->filename) == 0) {
				//found our node, now lets sum it up and reorder
				node->tfidf_sum += tfIdf;
				if (prev->tfidf_sum <= node->tfidf_sum) {
					prev->next = node->next;
					node->next = NULL;
					head = reAdd(head, node);
				}
				break;
			}
			prev = node;
			node = node->next;
		}
	}
	return head;
}

//reads the node in order back into the list
//only called when node is no longer in order of the list
TfIdfList reAdd(TfIdfList list, TfIdfList node) {

	if (node->tfidf_sum > list->tfidf_sum) {
		node->next = list;
		return node;
	} else if (node->tfidf_sum == list->tfidf_sum && strcmp(node->filename, list->filename) < 0) {
		node->next = list;
		return node;
	} else if (list->next == NULL) {
		list->next = node;
		return list;
	}

	TfIdfList head = list->next;
	TfIdfList prev = list;
	while (head != NULL) {
		//if equal, and strcmp < 0
		if (node->tfidf_sum == node->tfidf_sum) {
			if (strcmp(node->filename, node->filename) < 0) {
				prev->next = node;
				node->next = head;
				break;
			}
		}
		//normal add
		if (prev->tfidf_sum >= node->tfidf_sum && head->tfidf_sum < node->tfidf_sum) {
			prev->next = node;
			node->next = head;
			break;
		}
		//dont need add from tail since we know this fuction is only called when node is
		//no longer in order
		prev = head;
		head = head->next;
	}

	return list;
}

//returns 1 if filename in list, 0 if filename not in list
int inList(TfIdfList list, char *filename) {
	TfIdfList node = list;
	while (node != NULL) {
		if (strcmp(filename, node->filename) == 0) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}


// strdup not available so heres one
char *my_strdup(char *str) {
    int size = strlen(str) + 1;
    char *new = malloc(size);
    char *newp = new;
    while (*str) {
        *newp++ = *str++;
    }
    *newp = '\0';
    return new;
}

