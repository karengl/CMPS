// Karen Lee
// kglee
// pa01
// intList.c
// Implements intList.h and the IntList ADT. Defines the
// ADT functions completes the declaration of IntListNode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intList.h"

typedef struct IntListNode *IntList;
struct IntListNode {
    int element;
    IntList next;
};
const IntList intNil = NULL;

/** first
 * Preconditions: IntList that is not NULL
 */
int intFirst(IntList oldL) {
    return oldL->element;
}

/** rest
 * Preconditions: IntList that is not NULL
 */
IntList intRest(IntList oldL) {
    return oldL->next;
}

/** cons
 * Preconditions: int != 0, IntList that is not NULL if there
 *                is a preexisting IntList
 */
IntList intCons(int newE, IntList oldL) {
    IntList newL = calloc(1, sizeof (struct IntListNode));
    newL->element = newE;
    newL->next = oldL;
    return newL;
}
