/*  This file (output_list.c), along with output_list.h, implements a linked list used for the
 *  output of simulator info at the final stage of the simulation.
 *
 *  It's heavily based on files list.c and list.h. We prefer a small amount of code duplication
 *  to using "generic" lists based on void* and casting, because the latter is unsafe and messy.
 */

