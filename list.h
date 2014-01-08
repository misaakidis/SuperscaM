#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "definitions.h"

/** This isn't really a linked-list implementation, since there might be
    a few cycles, corresponding to branches!
**/



typedef struct node_struct {
    ASMinstr instr;
    struct node_struct *next;
    struct node_struct *branch; // will be NULL for most instructions
} node;

typedef struct {
    node *head;
    long size;
} list;

typedef struct {
    long size;
    ASMinstr ar[0];     // GCC extension, useful for dynamic array allocation
} arrayStruct;

/** Prototypes of non-static functions: */
void *xmalloc(size_t);
list *create_list();                            // self-explanatory
void free_list(list *);                         // self-explanatory
void consume_list(list *, void (*)(ASMinstr));  // call a void func() on each element
void execute_list(list *, int (*)(ASMinstr));  // call a bool func on each; if true take branch
arrayStruct *list_to_array(list *);             // turn the list into an array
void add_node(list *, ASMinstr);                // add an element to the list
long sublist_size(node *);          // will become static in the final version of the program

node *add_node_and_get_address(list *, ASMinstr);   // needed for intructions after labels :(
node *get_node_at(list*, int);                      // returns node* at given index (unused?)


static list *L1;
static list *L2;

#endif /* LIST_H_INCLUDED */
