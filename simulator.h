#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED

#include <stdbool.h>

/** Prototypes of non-static functions: */
int process_ASMinstr(ASMinstr);      // process single instruction (eg. put it in the pipeline)
void process_whole_list(list *);                // useful for (2nd pass) parsing
void process_list_following_branches(list *);   // useful for actual execution


#endif // SIMULATOR_H_INCLUDED
