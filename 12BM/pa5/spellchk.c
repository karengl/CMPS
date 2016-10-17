// $Id: spellchk.c,v 1.7 2016-03-10 16:20:44-08 - - $
// Karen Lee kglee

#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

int debug = 0;     // Keep track of the -x option

void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", program_name, object, message);
   fflush (NULL);
   exit_status = 2;
}

FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (const char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      char *word = yytext;
      if (!has_hashset(hashset, word)) {
         int i = 0;
         char *tmp = word;
         while (word[i]) {
            tmp[i] = tolower(word[i]);
            ++i;
         }
         word = tmp;
         if (!has_hashset(hashset, word)) {
            printf("%s: %d: %s\n", filename, yylineno, yytext);
            exit_status = 1;
         }
      }
   }
}

void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   FILE *file = open_infile(dictionary_name);
   char buffer[256];
   for (;;) {
      char *line = fgets(buffer, sizeof buffer, file);
      if (line == NULL) break;
      char *nlpos = strchr(buffer, '\n');
      if (nlpos != NULL) {
         *nlpos = '\0';
      }else {
         print_error(buffer, "unterminated line");
         continue;
      }
      char *word = strtok(line, " ");
      while (word != NULL) {
         put_hashset(hashset, word);
         word = strtok(NULL, " ");
      }
   }
   fclose(file);
}


void scan_options (int argc, char** argv,
                   char **default_dictionary,
                   char **user_dictionary) {
   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': *user_dictionary = optarg;
                   break;
         case 'n': *default_dictionary = NULL;
                   break;
         case 'x': if (debug == 1) debug = 2;    // -xx
                   else debug = 1;               // -x
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debug_flags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }
}


int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   scan_options (argc, argv, &default_dictionary, &user_dictionary);

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);
   if (default_dictionary == NULL && user_dictionary == NULL) {
      fprintf(stderr, "%s: dictionary is empty\n", program_name);
      exit_status = 2;
      return exit_status;
   }
   // Read and do spell checking on each of the files.
   if (debug != 0) {
      print_hashset(hashset, debug);
   }else {
      if (optind >= argc) {
         yyin = stdin;
         spellcheck (STDIN_NAME, hashset);
      }else {
         for (int fileix = optind; fileix < argc; ++fileix) {
            DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
            char *filename = argv[fileix];
            if (strcmp (filename, STDIN_NAME) == 0) {
               yyin = stdin;
               spellcheck (STDIN_NAME, hashset);
            }else {
               yyin = open_infile (filename);
               if (yyin == NULL) continue;
               spellcheck (filename, hashset);
               fclose (yyin);
            }
         }
      }
   }
   free_hashset(hashset);
   yylex_destroy ();
   return exit_status;
}

