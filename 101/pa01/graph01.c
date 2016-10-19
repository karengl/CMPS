// Karen Lee
// kglee
// pa01
// graph01.c
// Reads a file as input and organizes the coordinates of a
// graph into the ADT, IntList

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intList.h"

typedef struct {
    int from;
    int to;
    double weight;
} Edge;

// parseEdge
// Precondition: line is not NULL
Edge parseEdge(char line[]) {
    Edge newE;
    int numFields;
    char junk;
    numFields = sscanf(line, "%d %d %lf %s", &newE.from, &newE.to, &newE.weight, &junk);
    if (numFields < 2 || numFields > 3) {
        printf("Bad edge: %s", line);
        exit(1);
    }
    if (numFields == 2)
        newE.weight = 0.0;
    return newE;
}

int main (int argc, char **argv) {
    int m = 0; 
    int n = 0;
    char buffer[1024];
    if (argc < 2) {
        printf("Usage: graph input.data\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File does not exist\n");
        exit(1);
    }
    printf("Opened %s for input.\n", argv[1]);

    // Gets n and tests if there is only 1 int on
    // the first line of the file
    int numFields;
    char junk;
    char* line = fgets(buffer, sizeof buffer, file);
    numFields = sscanf(buffer, "%d %s", &n, &junk);
    if (numFields > 1) {
        printf("Bad line 1: %d %d\n", n, junk - '0');
        exit(1);
    }
    printf("n = %d\n", n);
    if (n < 1) {
        printf("n must be greater than 0\n");
        exit(1);
    }

    // Initialize E
    IntList E[n + 1];
    for (int i = 1; i <= n; i++)
        E[i] = intNil;

    // Read file and place edges in E
    while (line == buffer) {
        if (m != 0) {
            Edge e = parseEdge(buffer);
            if (E[e.from] == NULL) {
                E[e.from] = intCons(e.to, NULL);
            }else {
                E[e.from] = intCons(e.to, E[e.from]);
            }
        }
        m++;
        line = fgets(buffer, sizeof buffer, file);
        if (line == NULL) break;
    }
    if (!feof(file))
        fprintf(stderr, "Error\n");
    fclose(file);
    m--;
    printf("m = %d\n", m);

    // Print the IntList
    for (int i = 1; i <= n; i++) {
        printf("%-8d", i);
        if (E[i] == NULL) {
            printf("null\n");
        } else {
            // Print all numbers in the E[i]
            printf("[");
            while (intRest(E[i]) != NULL) {
                printf("%d, ", intFirst(E[i]));
                E[i] = intRest(E[i]);
            }
            printf("%d", intFirst(E[i]));
            printf("]\n");
        }
    }
    return 0;
}
