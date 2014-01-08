#ifndef OUTPUT_LIST_H_INCLUDED
#define OUTPUT_LIST_H_INCLUDED

#include "definitions.h"

/** a real linked-list implementation (this time ;)
**/



typedef struct output_node_struct {
    ASMinstr instr;
    struct output_node_struct *next;
} node;

typedef struct {
    node *head;
    long size;
} output_list;


/** Prototypes of non-static functions: */
list *create_output_list();                             // self-explanatory
void free_output_list(list *);                          // self-explanatory
void consume_output_list(list *, void (*)(ASMinstr));   // call a void func() on each element
void add_output_node(list *, ASMinstr);                 // add an element to the list
long output_sublist_size(node *);          // will become static in the final version of the program

#endif // OUTPUT_LIST_H_INCLUDED
