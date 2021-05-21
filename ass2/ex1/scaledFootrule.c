
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "part3.h"

// Part-3 was solved using the Hungarian Algorithm
int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("Usage: ./scaledFootrule <list of rankings>\n");
        exit(1);
    }

    //Reads all files given and puts them into Graph g
    //Also maintains a Set of filenames
    char *read = malloc(sizeof(read));
    Graph g = newGraph();
    Set mySet = newSet();
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        int j = 0;
        while (fscanf(fp, "%s", read) == 1) {
            char *fileName = my_strdup(read);

            Node listNode = newNode(fileName, j);
            if (g->files[i] == NULL) {
                g->files[i] = listNode;
            } else {
                //insertNode wont insert parallel edges
                insertNode(g->files[i], listNode);
            }
            insertInto(mySet, fileName);
            j++;
        }
        fclose(fp);
    }

    //Creating an array named len, which stores the length of each rankings list given from args
    int *len = malloc(sizeof(len) * argc);

    for (int i = 1 ; i < argc; i++) {
        Node curr = g->files[i];
        int count = 0;
        while (curr != NULL) {
            curr = curr->next;
            count++;
        }
        len[i] = count;
    }

    // Creating an index of the Set, which assigns each set element to an integer id
    char **index = malloc(sizeof(index) * nElems(mySet));
    int indexCounter = 0;
    for (Link new = mySet->elems; new != NULL; new = new->next) {
        index[indexCounter] = new->val;
        indexCounter++;
    }

    // Now, I havs a SET OF N ELEMENTS
    // and an array of rankings lists and the length of each list

    // Making a N x N martix
    // scaledDistance[ELEMENT_ID][POSITION]
    // so for example, if index[3] = "url42"
    // scaledDistance[3][0] would tell you the scaledFootrule for url43 at position 3
    // Making 2 matrices because the Algorithm will change values of the martix
    // Hence, we need a copy for the final calculation
    double scaledDistance[MAX_LIST][MAX_LIST];
    double scaledDistanceCopy[MAX_LIST][MAX_LIST];

    //i is the id of the elements, j is the placing of the elements
    for (int i = 0; i < nElems(mySet); i++) {
        for (int j = 0; j < nElems(mySet); j++) {

            double placeholder = 0;
            for (int z = 1 ; z < argc; z++) {
                Node curr = g->files[z];
                int count = 1;
                while (curr != NULL) {
                    if (strcmp(index[i], curr->urlName) == 0) {
                        placeholder += fabs( ((double)count/len[z]) - ((double)(j + 1)/nElems(mySet)));
                        break;
                    }
                    curr = curr->next;
                    count++;
                }
            }
            scaledDistance[i][j] = placeholder;
            scaledDistanceCopy[i][j] = scaledDistance[i][j];
        }
    }

    // Now with a NxN matrix, we need to decide on a combination of all
    // elements and positions (with no repetition) to get the lowest total sum
    // ------------Start of the Hungarian Algorithm -------------------------

    // Note: rows == elements; columns == positions

    // Step-1:

    // Find the lowest value for each row, and subtract the entire row by it
    for (int i = 0; i < nElems(mySet); i++) {
        double min = scaledDistance[i][0];
        for (int j = 0; j < nElems(mySet); j++) {
            if (scaledDistance[i][j] < min) {
                min = scaledDistance[i][j];
            }
        }
        for (int j = 0; j < nElems(mySet); j++) {
            scaledDistanceCopy[i][j] -= min;
        }
    }

    // Step-2:
    // Find the lowest value for each col, and subtract the entire col by it
    for (int j = 0; j < nElems(mySet); j++) {
        double min = scaledDistanceCopy[0][j];
        for (int i = 0; i < nElems(mySet); i++) {
            if (scaledDistanceCopy[i][j] < min) {
                min = scaledDistanceCopy[i][j];
            }
        }
        for (int i = 0; i < nElems(mySet); i++) {
            scaledDistanceCopy[i][j] -= min;
        }
    }

    // Before we begin, we count how mant zeros are in the matrix
    int zeroCount = 0;
    for (int i = 0; i < nElems(mySet); i++) {
        for (int j = 0; j < nElems(mySet); j++) {
            if (scaledDistanceCopy[i][j] == 0) zeroCount++;
        }
    }

    // now the graph will contain many 0 values, where there are 0s in the matrix
    // that element-position combination is a desirable one for our final rankings

    // Step-3:
    // This is the most demanding part of the entire algorithm
    // Cross-out all the 0s in the matrix with the least number of horizontal/vertical lines
    // i.e. eliminate all the 0s of the matrix, where the only way to eliminate 0s
    // is to eliminate an entire row, or an entire column


    //First we make 2 arrays representing the status of each element or position
    // 0 means not eliminated
    // 1 mean eliminated
    int *positionSet = calloc(nElems(mySet), sizeof(positionSet));
    int *elementSet = calloc(nElems(mySet), sizeof(positionSet));

    int zeroCountCopy = zeroCount;
    int totalCrosses = 0;


    while (totalCrosses != nElems(mySet)) {
        // All the functions in this loop are O(n^2)
        // and the loop iterates at most n times
        // Making this loop below O(n^3)
        while (zeroCountCopy != 0) {

            int largestX = countZerosRow(nElems(mySet), scaledDistanceCopy, elementSet, positionSet);
            int largestY = countZerosCol(nElems(mySet), scaledDistanceCopy, elementSet, positionSet);

            int crossed;

            if (largestX >= largestY) {
                crossed = crossRow(nElems(mySet), largestX, scaledDistanceCopy, elementSet, positionSet);

                totalCrosses += crossed;
                zeroCountCopy -= (largestX * crossed);
            } else {
                crossed = crossCol(nElems(mySet), largestY, scaledDistanceCopy, elementSet, positionSet);

                totalCrosses += crossed;
                zeroCountCopy -= (largestY * crossed);
            }

        }


        // After crossing out all the zeros, if the number of lines used is not N
        // then, Proceed to Step-4
        // else, Proceed to Step-5
        if (totalCrosses == nElems(mySet)) {
            break;
        } else {
            //Step-4;
            //go through whole graph and find the smallest val that wasnt crossed
            double localMin = nElems(mySet);
            for (int i = 0; i < nElems(mySet); i++) {
                if (elementSet[i] == 1) continue;
                for (int j = 0; j < nElems(mySet); j++) {
                    if (positionSet[j] == 1) continue;
                    if (scaledDistanceCopy[i][j] < localMin) {
                        localMin = scaledDistanceCopy[i][j];
                    }
                }
            }
            //go through whole graph and subtract the smallest val, foreach not crossed number
            for (int i = 0; i < nElems(mySet); i++) {
                if (elementSet[i] == 1) continue;
                for (int j = 0; j < nElems(mySet); j++) {
                    if (positionSet[j] == 1) continue;
                    scaledDistanceCopy[i][j] -= localMin;
                    // This was needed because newly generated zeros were not
                    // being caught by: == 0
                    if (scaledDistanceCopy[i][j] < 0.000001) {
                        scaledDistanceCopy[i][j] = 0;
                    }
                }
            }

            //Double crossed values, need to be increased by localMin
            for (int i = 0; i < nElems(mySet); i++) {
                if (elementSet[i] == 0) continue;
                for (int j = 0; j < nElems(mySet); j++) {
                    if (positionSet[j] == 0) continue;
                    scaledDistanceCopy[i][j] += localMin;
                }
            }

            // Step-4 Complete now going back to Step-3

            // reseting values
            for (int i = 0; i < nElems(mySet); i++) {
                positionSet[i] = 0;
                elementSet[i] = 0;
            }
            totalCrosses = 0;

            //Zeros could have been added or removed, so we need to update the value
            int UpdatedZeroCount = 0;
            for (int i = 0; i < nElems(mySet); i++) {
                for (int j = 0; j < nElems(mySet); j++) {
                    if (scaledDistanceCopy[i][j] == 0) UpdatedZeroCount++;
                }
            }
            zeroCountCopy = UpdatedZeroCount;
        }
    }


    // Step-5 Generating our ranking list


    //reset my sets
    for (int i = 0; i < nElems(mySet); i++) {
        positionSet[i] = 0;
        elementSet[i] = 0;
    }

    // myFinalList index represents the position index
    // the array contains the element id
    int *myFinalList = malloc(sizeof(myFinalList) * nElems(mySet));
    int itemsAdded = 0;

    /*
    Stratergy to generate the list:
        Start by going through every position foreach element
        if that element only has one position which is a 0, choose it
        and eliminate that element and position
        then do the same with every position, repeating until either done
        or in a state where this algorithm cannot select anymore.
    */
    while (itemsAdded < nElems(mySet)) {
        int beginItemsAdded = itemsAdded;

        for (int i = 0; i < nElems(mySet); i++) {
            if (elementSet[i] == 1) continue;
            int anotherZ = 0;
            int jMem;
            for (int j = 0; j < nElems(mySet); j++) {
                if (positionSet[j] == 1) continue;
                if (scaledDistanceCopy[i][j] == 0) {
                    jMem = j;
                    anotherZ++;
                }
            }
            if (anotherZ == 1) {
                myFinalList[jMem] = i;
                positionSet[jMem] = 1;
                elementSet[i] = 1;
                itemsAdded++;
            }
        }

        for (int i = 0; i < nElems(mySet); i++) {
            if (positionSet[i] == 1) continue;
            int anotherZ = 0;
            int jMem;
            for (int j = 0; j < nElems(mySet); j++) {
                if (elementSet[j] == 1) continue;
                if (scaledDistanceCopy[j][i] == 0) {
                    jMem = j;
                    anotherZ++;
                }
            }
            if (anotherZ == 1) {
                myFinalList[i] = jMem;
                positionSet[i] = 1;
                elementSet[jMem] = 1;
                itemsAdded++;
            }
        }

        // This is when the previous Algorithm cannot make selections anymore
        /*
        New Stratergy:
            now we know that every element remaining has >1 zeros
            and every column remaining >1 zeros
            We then have a queue of elements, in ascending order by number
            of 0s in the row.
            Then process the queue, giving each element to its first matched
            position
        */
        if (itemsAdded == beginItemsAdded) {
            int itemsNeeded = nElems(mySet) - itemsAdded;
            int *queue = malloc(sizeof(queue) *nElems(mySet));
            int queueIndex = 0;
            while (queueIndex != itemsNeeded) {
                int lowest = nElems(mySet);
                for (int i = 0; i < nElems(mySet); i++) {
                    if (elementSet[i] == 1) continue;
                    int check = 0;
                    for (int k = 0; k < queueIndex; k++) {
                        if (queue[k] == i) check = 1;
                    }
                    if (check) continue;

                    int anotherZ = 0;
                    for (int j = 0; j < nElems(mySet); j++) {
                        if (positionSet[j] == 1) continue;
                        if (scaledDistanceCopy[i][j] == 0) {
                            anotherZ++;
                        }
                    }
                    if (anotherZ < lowest) lowest = anotherZ;
                }

                for (int i = 0; i < nElems(mySet); i++) {
                    if (elementSet[i] == 1) continue;
                    int anotherZ = 0;
                    for (int j = 0; j < nElems(mySet); j++) {
                        if (positionSet[j] == 1) continue;
                        if (scaledDistanceCopy[i][j] == 0) {
                            anotherZ++;
                        }
                    }
                    if (anotherZ == lowest) {
                        queue[queueIndex] = i;
                        queueIndex++;
                    }
                }
            }
            for (int k = 0; k < queueIndex; k++) {
                //find the first 0 of queue[k] and take that position
                for (int j = 0; j < nElems(mySet); j++) {
                    if (positionSet[j] == 1) continue;
                    if (scaledDistanceCopy[queue[k]][j] == 0) {
                        myFinalList[j] = queue[k];
                        positionSet[j] = 1;
                        elementSet[queue[k]] = 1;
                        itemsAdded++;
                        break;
                    }
                }
            }
        }
    }

    double finalSum = 0;
    for (int i = 0; i < nElems(mySet); i++) {
        finalSum += scaledDistance[myFinalList[i]][i];
    }
    printf("%.6f\n", finalSum);
    for (int i = 0; i < nElems(mySet); i++) {
        printf("%s\n", index[myFinalList[i]]);
    }
    return 0;
}

