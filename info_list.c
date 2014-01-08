/*  This file (info_list.c), along with info_list.h, implements a linked list used for the
 *  output of simulator info at the final stage of the simulation.
 *
 *  It's heavily based on files list.c and list.h. We prefer a small amount of code duplication
 *  to using "generic" lists based on void* and type-casting, because the latter is unsafe
 *  and messy.
 *
 *  NOTE: This list implementations is a bit simpler.
 */


#include "info_list.h"
#include "definitions.h"



printinfo *create_printinfo(ASMinstr *cmd, int cycle_issued, int stalls_ex, int stalls_wb)
{
    xmalloc(sizeof(char)); //A BIYOUTIFOOL workaround
    printinfo *p = xmalloc(sizeof(printinfo));
    memcpy(p->cmd, cmd, sizeof(ASMinstr));
    p->cycle_issued = (uint_fast32_t) cycle_issued;
    p->stalls_ex = (uint_fast8_t) stalls_ex;
    p->stalls_wb = (uint_fast8_t) stalls_wb;

    return p;
}


info_list *create_info_list()
{
    info_list *newlist = xmalloc(sizeof(info_list));

    newlist->head = NULL;
    newlist->size = 0;

    return newlist;
}


static
void free_info_sublist(info_node *SL)
{
    if (SL != NULL)
        free_info_sublist(SL->next);

    free(SL);   // Freeing NULL is OK and "SL = NULL;" is not needed because SL is a local var.
}


void free_info_list(info_list *L)
{
    if (L == NULL)
        return;

    free_info_sublist(L->head);
    free(L);
}


static
void print_info_sublist(FILE *f, info_node *SL, void (*callback)(FILE *, printinfo *))
{
    if (SL != NULL){
        callback(f, &SL->info);
        print_info_sublist(f, SL->next, callback);
    }
}


void print_info_list(FILE *f, info_list *L, void (*callback)(FILE *, printinfo*))
{
    print_info_sublist(f, L->head, callback);
}


static
info_node *add_info_node_recursive(info_node *SL, printinfo *value)
{
   if (SL == NULL){
        SL = xmalloc(sizeof(info_node));
        memcpy(&SL->info, value, sizeof(info_node));
        SL->next = NULL;
        return SL;
    }
    else
        SL->next = add_info_node_recursive(SL->next, value);
    return SL;
}


void add_info_node(info_list *L, printinfo *value)
{
    if (L == NULL)
        L = create_info_list();

    if (L->head != NULL)
        add_info_node_recursive(L->head, value);
    else
        L->head = add_info_node_recursive(L->head, value);

    L->size+=1;
}


long output_info_sublist_size(info_node *);


