/* Karen Lee kglee
 * pa01
 * intList.h
 * Declares the functions and the struct IntListNode for the IntList
 * ADT. Declares all the access and constructor functions needed for
 * the ADT and graph01.c to work.
 */

#ifndef C101IntList
#define C101IntList
/* Multiple typedefs for the same type are an error in C. */

typedef struct IntListNode * IntList;

/** intNil denotes the empty IntList */
extern const IntList intNil;

/* Access functions
 * Preconditions: IntList
 */

/** first
 *  Preconditions: IntList that is not NULL
 */
int intFirst(IntList oldL);

/** rest
 *  Preconditions: IntList that is not NULL
 */
IntList intRest(IntList oldL);

/* Constructors
 */

/** cons
 * Preconditions: int != 0, IntList that is not NULL if there is a preexisting IntList
 * Postconditions: Returns a new IntList with the new element and links to the old
 *                 IntList
 */
IntList intCons(int newE, IntList oldL);

#endif

