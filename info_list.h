#ifndef OUTPUT_LIST_H_INCLUDED
#define OUTPUT_LIST_H_INCLUDED

#include "definitions.h"

/** a real linked-list implementation (this time ;)
**/

/*
typedef struct {
    ASMinstr      *cmd;
    uint_fast32_t cycle_issued;
    uint_fast8_t  stalls_ex;
    uint_fast8_t  stalls_wb;
} printinfo;
*/


typedef struct info_node_struct {
    printinfo info;
    struct    info_node_struct *next;
} info_node;


typedef struct {
    info_node *head;
    long size;
} info_list;


/** Prototypes of non-static functions: */
info_list *create_info_list();                                 // self-explanatory
void free_info_list(info_list *);                              // self-explanatory
void print_info_list(FILE *, info_list *, void (*)(FILE *, printinfo *)); // call a void func() on each element
void add_info_node(info_list *, printinfo *);                    // add an element to the list
long output_sublist_size(info_node *);                         // will become static in the final version of the program

#endif // OUTPUT_LIST_H_INCLUDED
