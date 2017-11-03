// Karen Lee
// kglee
// pa2
// Lex.c
// Uses the List ADT. Gets input from a file and sorts the
// output lexicographically into an output file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

#define MAX_LEN 255

int main(int argc, char* argv[]) {
    int j = 0;
    int count= 0;
    int cmp = 0;
    FILE *in, *out;
    char line[MAX_LEN];
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (in == NULL) {
        fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }
    if (out == NULL) {
        fprintf(stderr, "Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    // Counts amount of lines in file and close file
    while (fgets(line, MAX_LEN, in) != NULL) {
        count++;
    }
    fclose(in);

    // Reopen file and copy strings to array
    in = fopen(argv[1], "r");
    char list[count][MAX_LEN];
    while(fgets(line, MAX_LEN, in) != NULL) {
        strcpy(list[j], line);
        j++;
    }

    j = 0;
    List L = newList();
    prepend(L, 0);
    for (int i = 1; i < count; i++) {
        j = i - 1;
        moveBack(L);
        while (j > -1) {
            cmp = strcmp((char *) list[i], list[get(L)]);
            if (cmp > 0)
                break;
            j--;
            movePrev(L);
        }
        if (index(L) > -1)
            insertAfter(L, i);
        else
            prepend(L, i);
    }

    moveFront(L);
    while (index(L) > -1) {
        fprintf(out, "%s", list[get(L)]);
        moveNext(L);
    }

    freeList(&L);
    fclose(in);
    fclose(out);

    return 0;
}
