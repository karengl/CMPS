// $Id: bigint.h,v 1.1 2016-02-27 15:15:59-08 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

typedef struct bigint bigint;

typedef bigint *(*bigint_binop) (bigint*, bigint*);

bigint *new_bigint (size_t capacity);

bigint *new_string_bigint (const char *string);

void free_bigint (bigint*);

void print_bigint (bigint*);

bigint *add_bigint (bigint*, bigint*);

bigint *sub_bigint (bigint*, bigint*);

bigint *mul_bigint (bigint*, bigint*);

void show_bigint (bigint*);

#endif

