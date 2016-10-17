// $Id: hashset.h,v 1.3 2016-03-10 13:01:27-08 - - $
// Karen Lee kglee

#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <stdbool.h>

typedef struct hashset hashset;

//
// Create a new hashset with a default number of elements.
//
hashset *new_hashset (void);

//
// Frees the hashset, and the words it points at.
//
void free_hashset (hashset*);

void print_hashset (hashset*, int);

//
// Inserts a new string into the hashset.
//
void put_hashset (hashset*, const char*);

void resize_hashset(hashset*);

//
// Looks up the string in the hashset and returns true if found,
// false if not found.
//
bool has_hashset (hashset*, const char*);

#endif

