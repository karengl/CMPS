// $Id: bigint.c,v 1.8 2016-02-28 20:06:33-08 - - $
// Karen Lee kglee

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char)+1);
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}


bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *do_add (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result = new_bigint(this->capacity + 1);
   result->size = this->size;
   int carry = 0;
   for (size_t index = 0; index < this->size; ++index) {
      int digit = this->digits[index] + that->digits[index] + carry;
      result->digits[index] = digit % 10;
      carry = digit / 10;
   }
   return result;
}

bigint *do_sub (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result = new_bigint(this->capacity);
   result->size = this->size;
   int borrow = 0;
   for (size_t index = 0; index < this->size; ++index) {
      int digit = this->digits[index] - that->digits[index] - borrow
                                                            + 10;
      result->digits[index] = digit % 10;
      borrow = 1 - digit / 10;
   }
   trim_zeros(result);
   return result;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this) {
   DEBUGS ('b', show_bigint (this));
   int count = 0;
   if (this->negative) printf("-");
   for (size_t index = this->size-1; index > 0; --index) {
      ++count;
      if (count == 70 && this->size > 70) {
         printf("\\\n");
         count = 1;
      }
      printf("%d", this->digits[index]);
   }
   printf("%d\n", this->digits[0]);
}

bigint *add_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   if (that == NULL) return NULL;
   bigint *sum = NULL;
   int eq = 1;
   if (this->negative == that->negative) {
      sum = do_add(this, that);
      sum->negative = this->negative;
   }else {
      if (this->size > that->size) {
         sum = do_sub(this, that);
         sum->negative = this->negative;
      }else if (this->size < that->size) {
         sum = do_sub(that, this);
         sum->negative = that->negative;
      }else {
         for (size_t index = 0; index < this->size - 1; ++index) {
            if (this->digits[index] > that->digits[index]) {
               sum = do_sub(this, that);
               sum->negative = this->negative;
               eq = 0;
               break;
            }else if (this->digits[index] < that->digits[index]) {
               sum = do_sub(that, this);
               sum->negative = that->negative;
               eq = 0;
               break;
            }
         }
         if (eq == 1) sum = do_sub(this, that);
      }
   }
   return sum;
}

bigint *sub_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   if (that == NULL) return NULL;
   bigint *diff = NULL;
   int eq = 1;
   if (this->negative != that->negative) {
      diff = do_add(this, that);
      diff->negative = this->negative;
   }else {
      if (this->size > that->size) {
         diff = do_sub(this, that);
         diff->negative = this->negative;
      }else if (this->size < that->size) {
         diff = do_sub(that, this);
         if (!that->negative) diff->negative = true;
         else diff->negative = false;
      }else {
         for (size_t index = 0; index < this->size - 1; ++index) {
            if (this->digits[index] > that->digits[index]) {
               diff = do_sub(this, that);
               diff->negative = this->negative;
               eq = 0;
               break;
            }else if (this->digits[index] < that->digits[index]) {
               diff = do_sub(that, this);
               if (!that->negative) diff->negative = true;
               else diff->negative = false;
               eq = 0;
               break;
            }
         }
         if (eq == 1) diff = do_sub(this, that);
      }
   } 
   return diff;
}


bigint *mul_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result = new_bigint(this->capacity + that->capacity);
   result->size = this->size + that->size;
   for (size_t i = 0; i < this->size; ++i) {
      int c = 0;
      int k = i;
      for (size_t j = 0; j < that->size; ++j) {
         int d = this->digits[i] * that->digits[j] + c
                                 + result->digits[k];
         c = d / 10;
         result->digits[k] = d % 10;
         ++k;
      }
      result->digits[k] = c;
      ++k;
   }
   trim_zeros(result);
   if (this->negative == that->negative)
      result->negative = false;
   else
      result->negative = true;
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

