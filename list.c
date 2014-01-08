//#include "list.h"
#include "definitions.h"


//static
void *xmalloc(size_t size)
{
    register void *addr = malloc(size);     /* Line copied from the GNU C Library documentation!
                                               The "register" keyword is a hint to the compiler
                                               that the variable should be kept in a processor
                                               register, if possible. */
    if (addr == NULL)
        error(E_XMALLOC, ACTION_PRINTMSG_AND_EXIT);

    return addr;
}


list *create_list()
{
    list *newlist = xmalloc(sizeof(list));

    newlist->head = NULL;
    newlist->size = 0;

    return newlist;
}


static
node *add_node_recursive(node *SL, ASMinstr value)
{
    if (SL == NULL){
        SL = xmalloc(sizeof(node));
        SL->instr  = value;
        SL->next   = NULL;
        SL->branch = NULL;
        return SL;      // Return the address of the (new) node with the instruction "value"...
    }
    else
        SL->next = add_node_recursive(SL->next, value);

    return SL;
}


node *add_node_iterative(node *SL, ASMinstr value)
{
    node *tmp = xmalloc(sizeof(node));
    tmp->instr  = value;
    tmp->next   = NULL;
    tmp->branch = NULL;

    if (SL == NULL)
        SL = tmp;
    else {
        node *n;
        for (n = SL; n->next != NULL; n = n->next); // now n->next == NULL

        n->next = tmp;
    }
    return tmp;
}


/*
node* get_node_at(int index, list* L)
{
    node* currentNode;
    currentNode = L->head;
    for(int i=0; i<index; i++)
    {
        currentNode = currentNode->next;
    }
    return currentNode;
}
*/


void add_node(list *L, ASMinstr value)
{
    if (L == NULL)
        L = create_list();

    if (L->head != NULL)
        add_node_recursive(L->head, value);
    else
        L->head = add_node_recursive(L->head, value);
        //L->head = add_node_iterative(L->head, value);

    L->size+=1;
}


node *add_node_and_get_address(list *L, ASMinstr value) // needed for intructions after labels :(
{
    if (L == NULL)
        L = create_list();

    node *tmp;

    if (L->head != NULL)
        tmp = add_node_recursive(L->head, value);
    else
        tmp = L->head = add_node_recursive(L->head, value);

    L->size+=1;

    tmp = L->head;          // !!
    while(tmp->next != NULL)
        tmp = tmp->next;

    return tmp;
}


node *get_node_at(list* L, int index)
{
    node* currentNode;
    currentNode = L->head;
    for(int i=0; i<index; i++){
        currentNode = currentNode->next;
    }
    return currentNode;
}


static
void free_sublist(node *SL)
{
    if (SL != NULL)
        free_sublist(SL->next);

    free(SL);   // Freeing NULL is OK and "SL = NULL;" is not needed because SL is a local var.
}

void free_list(list *L)
{
    if (L == NULL)
        return;

    free_sublist(L->head);
    free(L);
}


static
void consume_sublist(node *SL, void (*callback)(ASMinstr))  // perhaps callback() should be bool
{                                                           // with its return value ignored
    if (SL != NULL){
        callback(SL->instr);
        consume_sublist(SL->next, callback);
    }
}


void consume_list(list *L, void (*callback)(ASMinstr))      // perhaps callback() should be bool
{                                                           // with its return value ignored
    consume_sublist(L->head, callback);
}


static
void execute_sublist(node *SL, int (*callback)(ASMinstr))
{
    if (SL != NULL){
        if (callback(SL->instr)){
            assert(SL->branch != NULL); // Branching ins. are expected to point to a valid label
            execute_sublist(SL->branch, callback); // without assertions, we would stop anyway:)
        }
        else
            execute_sublist(SL->next, callback);
    }

}


void execute_list(list *L, int(*callback)(ASMinstr)) // difference: this also executes branches
{
    execute_sublist(L->head, callback);
}


static
void sublist_to_array(node *SL, arrayStruct *array, long *position) /** PROBABLY NOT READY **/
{                                                                   /** ... AND NOT USEFUL **/
    if (SL == NULL)
        return;

    array->ar[(*position)++] = SL->instr;
    sublist_to_array(SL->next, array, position);
}


arrayStruct *list_to_array(list *L) /** PROBABLY NOT READY **/
{                                   /** ... AND NOT USEFUL (right now) **/
    if (L == NULL)
        return NULL;

    /* dynamic array allocation! */
    arrayStruct *tmp =  xmalloc(sizeof(arrayStruct) + L->size*sizeof(ASMinstr));
    long position = 0;
    sublist_to_array(L->head, tmp, &position);
    tmp->size = L->size;

    return tmp;
}


static
void delete_node(list *L, int index)                    /* NOT YET IMPLEMENTED */
{

}


long sublist_size(node *SL)
{
    if (SL == NULL)
        return 0;

    return sublist_size(SL->next) + 1;
}


static
void prettyprint_list(list *L)                          /* NOT YET IMPLEMENTED */
{
    if (L == NULL)  // if T empty, abort
        return;

}
