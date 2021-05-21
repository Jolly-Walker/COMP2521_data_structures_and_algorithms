#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: ./pagerank <d> <diffPR> <maxIterations>\n");
        exit(1);
    }

    FILE *fp = fopen("collection.txt", "r");
    if (fp == NULL) {
        printf("Couldnt open %s", "collection.txt");
        exit(1);
    }

    //array of filenames, the index is the id
    //filenameIndex[3] = "url42" means that url42 has an id of 4 in the graph
    char *filenameIndex[MAX_NODE];

    char *read = malloc(sizeof(read));
    double damp = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    Graph g = newGraph();
    int collectCount = 0;

    while (fscanf(fp,"%s", read) == 1) {
        filenameIndex[collectCount] = my_strdup(read);
        collectCount++;
    }
    //index collected!
    fclose(fp);

    for (int j = 0; j < collectCount; j++) {
        // open every url and put them into the graph
        char *filename = my_strdup(filenameIndex[j]);
        strcat(filename, ".txt");

        FILE *fp = fopen(filename, "r");

        int startIndicator = 0;
        int endIndicator = 0;
        int sectionIndicator = 0;

        while (fscanf(fp,"%s", read)== 1) {

            if (endIndicator == 1) {
                endIndicator = 0;
                sectionIndicator = 0;
                break;
            }

            if (strcmp(read, "#start") == 0) {
                startIndicator = 1;
                continue;
            }

            if (strcmp(read, "#end") == 0) {
                endIndicator = 1;
                continue;
            }

            if (startIndicator == 1) {
                if (strcmp(read, "Section-1") == 0) {
                    sectionIndicator = 1;
                    startIndicator = 0;
                } else if (strcmp(read, "Section-2") == 0) {
                    sectionIndicator = 2;
                    startIndicator = 0;
                }
                continue;
            }

            if (sectionIndicator == 1) {
                //dont add self loops
                if (strcmp(read, filenameIndex[j]) == 0) continue;

                char *dupOutName = my_strdup(read);
                int tempID;
                for (tempID = 0; tempID < collectCount; tempID++) {
                    if (strcmp(read, filenameIndex[tempID]) == 0) {
                        break;
                    }
                }
                Node listNode = newNode(dupOutName, tempID);
                if (g->files[j] == NULL) {
                    g->files[j] = listNode;
                } else {
                    //insertNode wont insert parallel edges
                    insertNode(g->files[j], listNode);
                }
            }

        }
        fclose(fp);
    }


    // ------- Graph set-up complete ------------

    double *pr = malloc(sizeof(double) * collectCount); //array of PageRanks[i iteration]
    double *PrevPr = malloc(sizeof(double) * collectCount); //array of the previous PageRanks
    int inDeg[MAX_NODE] = {0}; // array of inDeg
	int outDeg[MAX_NODE] = {0}; // array of outDeg

    int i;
    for (i = 0; i < collectCount; i++) {
        Node curr = g->files[i];
		int outs = 0;
		while (curr != NULL) {
			inDeg[curr->urlID]++;
			outs++;
			curr = curr->next;
		}
		outDeg[i] = outs;
        pr[i] = 1 / (double)collectCount;
        PrevPr[i] = pr[i];
    }

    double w_In[MAX_NODE][MAX_NODE]={{0}};
    double w_Out[MAX_NODE][MAX_NODE]={{0}};

    for (int i = 0; i < collectCount; i++) {
        Node zCurr = g->files[i];
        while(zCurr != NULL) {
            w_In[i][zCurr->urlID] = calcIn(g, i, zCurr->urlID, inDeg);
            w_Out[i][zCurr->urlID] = calcOut(g, i, zCurr->urlID, outDeg);
            zCurr = zCurr->next;
        }
    }

    double diff = diffPR;
    i = 0;
    while (i < maxIterations && diff >= diffPR) {
        for (int z = 0; z < collectCount; z++) {
            PrevPr[z] = pr[z];

            double placeholder = 0;
            for (int k = 0; k < collectCount; k++) {
                if (w_In[k][z] != 0) {
                    double val = PrevPr[k] * w_In[k][z] * w_Out[k][z];
                    placeholder += val;
                }
            }

            pr[z] = ((1 - damp)/collectCount) + (damp * placeholder);
        }
        diff = calcDiff(PrevPr, pr, collectCount);
        i++;
    }

    FILE *dest = fopen("pagerankList.txt", "w");

    int chosenIndex = 0;
    for (int j = 0; j < collectCount; j++) {
        for (int z = 0; z < collectCount; z++) {
            if (pr[chosenIndex] != -1) break;
            chosenIndex = z;
        }
        for (int z = 0; z < collectCount; z++) {
            if (pr[z] > pr[chosenIndex]) chosenIndex = z;
        }
        fprintf(dest, "%s, %d, %.7f\n", filenameIndex[chosenIndex], outDeg[chosenIndex], pr[chosenIndex]);
        pr[chosenIndex] = -1;
    };

    free(g);


    return 0;
}

