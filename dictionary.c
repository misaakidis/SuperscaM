/** functions create_dict(), add_to_dict(), retrieve_from() and delete_from(), along with the
    definitions in "parser.h", implement a very crude dictionary facility.
**/

#include "dictionary.h"

/** Variant of Dan Bernstein's string-to-int hashing function,
    see comment in dictionary.h for the source:
**/
inline
//uint64_t hash(unsigned char *str)
uint64_t hash(char *str) // was: unsigned char *str
{
    if (str == NULL)
        return 0;

    uint64_t hash_result = 5381;
    int c;

    while (c = *str++)
            hash_result = ((hash_result << 5) + hash_result) + c; /* hash * 33 + c */

    return hash_result;
}


dictionary *create_dict(int size)
{
    dictionary *tmp = xmalloc(sizeof(dictionary) + size*sizeof(dict_entry));
    tmp->size = size;
    tmp->entries = 0;

    for(int i = 0; i < tmp->size; i++){    // initialise to 0/NULL all entries
        tmp->e[i].hashcode  = 0;
        tmp->e[i].goto_node = NULL;
    }

    return tmp;
}


void add_to_dict(dictionary *dict, uint64_t hash, node *goto_node)
{
    if(hash == 0)       // hash of the empty string
        return;         // do nothing;

    if(retrieve_from(dict, hash) != NULL)
        error(E_DUPL_LABEL, ACTION_PRINTMSG_AND_EXIT);

    assert(dict->entries <= dict->size);    // assert we still have space in the dictionary!
    assert(goto_node != NULL);              // ...and that the node is a valid one

    dict->e[dict->entries].hashcode  = hash;
    dict->e[dict->entries].goto_node = goto_node;
    dict->entries++;
}


node *retrieve_from(dictionary *dict, uint64_t hash)
{
    for(int i = dict->entries - 1; i >= 0; --i){        // just linear search ;)
        if(dict->e[i].hashcode == hash)
            return dict->e[i].goto_node;
    }
    return NULL;
}


void delete_from(dictionary *dict, uint64_t hash) // untested (and unneeded)
{
    for(int i = dict->entries; i >= 0; --i){
        if(dict->e[i].hashcode == hash){   // entry found! Slide the rest of the entries to delete:
            for(int j = i; j <= (dict->entries - 2); ++j){
                dict->e[j].hashcode  = dict->e[j+1].hashcode;
                dict->e[j].goto_node = dict->e[j+1].goto_node;
            }
            dict->e[dict->entries - 1].hashcode  = 0;   // fix the last entry
            dict->e[dict->entries - 1].goto_node = NULL;
            --dict->entries;
            break;  // break for-loop
        }
    }
}


void update_entry(dictionary *dict, uint64_t hash, node *new_goto_node)
{
    for(int i = dict->entries; i >= 0; --i){
        if((dict->e[i].hashcode == hash)){
            dict->e[i].goto_node = new_goto_node;
            break;  // break for-loop
        }
    }
    /* If "label" not in "dict", do nothing. */
}
