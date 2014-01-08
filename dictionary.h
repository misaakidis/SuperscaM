#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

#include "definitions.h"

/** functions create_dict(), add_to_dict(), retrieve_from() and delete_from(), along with the
    definitions in "dictionary.h", implement a very crude dictionary facility.
**/

//typedef struct {
//    char *label;        // or perhaps "char label[MAX_LABEL_LEN];" ???
//    node *goto_node;
//} dict_entry;

/** Since C "has some inherent problems with string handling" (= it sucks), we switched our
    dictionary implementation to using hashes instead of labels (char *).

    For the hashing we have used Dan Bernstein's [string -> int] hashing algorithm, implemented
    in dictionary.c .
    Source : http://www.cse.yorku.ca/~oz/hash.html
**/


typedef struct {
    uint64_t hashcode;
    node     *goto_node;
} dict_entry;


typedef struct {
    int size;
    int entries;
    dict_entry e[0];    // GCC extension, useful for dynamic array allocation
} dictionary;


/** Prototypes of non-static functions: */
uint64_t hash(char *);
dictionary *create_dict(int);
void add_to_dict(dictionary *, uint64_t, node *);
node *retrieve_from(dictionary *, uint64_t);
void delete_from(dictionary *, uint64_t);
void update_entry(dictionary *, uint64_t, node *);


#endif // DICTIONARY_H_INCLUDED
