// $Id: hashset.c,v 1.9 2016-03-12 00:38:18-08 - - $
// Karen Lee kglee

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %zd, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   assert(this != NULL);
   hashnode *node = NULL;
   hashnode *old = NULL;
   for (size_t index = 0; index < this->size; ++index) {
      node = this->chains[index];
      while (node != NULL) {
         old = node;
         node = node->link;
         free(old->word);
         free(old);
      }
   }
   free(this->chains);
   free(this);
}

void print_hashset (hashset *this, int debug) {
   if (debug == 1) {
      fprintf(stderr, "%5lu words in the hash set\n", this->load);
      fprintf(stderr, "%5lu size of the hash array\n", this->size);
   }
   size_t index = 0;
   size_t count = 0;
   int num[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // chain sizes 0-10
   for (index = 0; index < this->size; ++index) {
      if (this->chains[index] != NULL) {
         for (hashnode *node = this->chains[index]; node != NULL;
                                                node = node->link) {
            size_t hash_code = strhash(node->word) % this->size;
            if (this->chains[hash_code] != NULL) {
               ++count;
               if (debug == 2) {
                  if (count == 1)
                     fprintf(stderr, "array[%10zu] = %20lu \"%s\"\n",
                                     index, hash_code, node->word);
                  else
                     fprintf(stderr, "%17s = %20lu \"%s\"\n", "",
                                            hash_code, node->word);
               }
            }
         }
         if (count < 11)
            ++num[count];
         count = 0;
      }
   }
   if (debug == 1) {
      for (int ind = 0; ind < 10; ++ind) {
         if (num[ind] != 0)
            fprintf(stderr, "%5d chains of size %2d\n", num[ind], ind);
      }
   }
}

void put_hashset (hashset *this, const char *item) {
   if (has_hashset(this, item)) return;
   hashnode *node = malloc(sizeof(hashnode));
   node->word = strdup((char *) item);
   node->link = NULL;
   size_t hash_code = strhash(node->word) % this->size;
   node->link = this->chains[hash_code];
   this->chains[hash_code] = node;
   ++this->load;
   
   // Resizes the hashset
   if (this->load * 2 > this->size) {
      resize_hashset(this);
   }
}

void resize_hashset(hashset *this) {
   hashset *new = new_hashset();
   // Free the size of the default chains
   free(new->chains);
   size_t old_size = this->size;
   new->size = 2 * old_size + 1;
   // Allocate the new size for chains
   size_t list_size = new->size * sizeof(hashnode *);
   new->chains = malloc(list_size);
   assert (new->chains != NULL);
   memset(new->chains, 0, list_size);
   for (size_t index = 0; index < old_size; ++index) {
      while (this->chains[index] != NULL) {
         put_hashset(new, (const char*) this->chains[index]->word);
         hashnode *node = this->chains[index];
         this->chains[index] = this->chains[index]->link;
         free(node->word);
         free(node);
      }
   }
   hashset tmp = *this;
   *this = *new;
   *new = tmp;
   free_hashset(new);
}

bool has_hashset (hashset *this, const char *item) {
   assert(this->chains != NULL);
   size_t hash_code = strhash(item) % this->size;
   for (hashnode *node = this->chains[hash_code]; node != NULL;
                                                node = node->link) {
      if (strcmp(node->word, item) == 0) return true;
   }
   return false;
}
