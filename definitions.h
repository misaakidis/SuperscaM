#ifndef DEFINITIONS_H_INCLUDED  // header guard (all our header files have a similar one)
#define DEFINITIONS_H_INCLUDED

/** Standard library inclusions: **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>            // C99

#ifndef DEBUG
    #define NDEBUG              // needed *before* #include-ing <assert.h>
#endif                          // to disable assertions for non-DEBUG builds

#include <assert.h>             // NDEBUG must be properly #define-d *before* this line!


/** Other inclusions (our own header files): **/
/*
#include "list.h"        // linked-list implementation
#include "parser.h"      // parser.c visible functions (incl. a crude dictionary implementation)
#include "simulator.h"   // simulator.c visible functions
#include "errors.h"      // return codes for abnormal situations
*//* see below the typedefs etc. */


/** CONSTANTS used throughout the program: **/

//#define NUM_OF_INT_REG      11    // number of integer registers
//#define NUM_OF_FLO_REG      32    // number of floating-point registers

//#define NUM_OF_RINSTR		4
//#define NUM_OF_IINSTR		3
//#define NUM_OF_BINSTR		1

#define COMMAND_LINE_BUFFER 255   // max length of an (unparsed) instruction
#define MAX_LABEL_NUM       2048  // max number of labels (=default dictionary size)
#define MAX_LABEL_LEN       64    // max label length (DISABLED - now we store hashes)
#define MAX_CMD_LEN         6     // must be large enough to fit "SUBD", "MULTD" etc. as strings
#define MAX_PRINTABLE_DIGITS 5     // number of digits (for output alignment)
#define MAX_PRINTABLE_CYCLES 99999 // number of cycles (for output alignment)

/** TYPES and STRUCTURES used throughout the program: **/

//static const char * RtypeCommands[] = {"ADDD", "SUBD", "MULTD", "DIVD"};
//static const char * ItypeCommands[] = {"ADDI", "SD", "LD"};
//static const char * BtypeCommands[] = {"BEQZ"};
static char cmd_delim[] = "\t ,.$";
static char cfg_delim[] = "\t =";


/** Other inclusions (our own header files): **/

#include "instruction_set.h"
#include "list.h"               // linked-list implementation
//#include "info_list.h"          // linked-list implementation (for output info)
#include "parser.h"             // parser.c visible functions (incl. a crude dictionary implementation)
#include "simulator.h"          // simulator.c visible functions
#include "errors.h"             // return codes for abnormal situations
#include "config_parser.h"      // global values defined in the configuration file
#include "registerFile.h"       // register file
#include "reservation_stations.h"   // reservation stations
#include "functional_units.h"   // semaphore-like counters for available int anf f.p. units
#include "printer.h"            // printing the output to the screen / a file


#endif /* DEFINITIONS_H_INCLUDED */
