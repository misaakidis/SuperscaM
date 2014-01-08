//#include "errors.h"
#include "definitions.h"


static
void act1 (ERRORNUM n, char *msg)
{
    fprintf(stderr, msg);
    fprintf(stderr, " (ERR NO:%2d)\n", n);
}

static
void act2 (ERRORNUM n, char *msg)
{
    exit(n);
}

static
void act3 (ERRORNUM n, char *msg)
{
    fprintf(stderr, msg);
    fprintf(stderr, " (ERR NO:%2d)\n", n);
    exit(n);
}


/** extend the following function (and the header file errors.h) as needed: **/

void error(ERRORNUM n, ERRORACTION act)
{
    char* msg;
    ACTION fn[3] = {act1, act2, act3}; // careful with the order of the function pointers!

    switch (n) {  // n is an enumeration
        case E_NO_PROBLEM:  msg = "No problem, exiting normally...";

                            break;
        case E_INSUF_ARGS:  msg = "Insufficient arguments (missing assembly source code file).";

                            break;
        case E_FILE_NOT_F:  msg = "Assembly source code file not found (or could not be"
                                  "opened).";
                            break;
        case E_XMALLOC:     msg = "Memory allocation failure! Terminating execution...";

                            break;
        case E_JUMPINSTR:   msg = "JUMP (or unknown) instruction found!";

                            break;
        case E_NO_BRANCHES: msg = "It seems that the input asm file has no branching "
                                  "instructions at all!";
                            break;
        case E_CMDLINE_BUF: msg = "Cannot parse line, too large. Ignoring line.";

        					break;
        case E_CMD_INVALID: msg = "Invalid command line at the assembly input file.";

        					break;
        case E_CMD_REG_INV: msg = "Invalid register in assembly command.";

                            break;
        case E_CFG_MISS:    msg = "Missing line(s) in the configuration file.";

        					break;
        case E_CFG_REPEAT:  msg = "Repeated declaration in configuration file.";

                            break;
        case E_ATOI:        msg = "Cannot parse the given string to int value.";

                            break;
        case E_CFG_INV_VAL: msg = "Invalid value at configuration file.";

                            break;
        case E_CFG_INV_LINE:msg = "Invalid line in configuration file.";

                            break;
        case E_DIV_ZERO:    msg = "Division with zero.";

                            break;
        case E_NO_RES_ST:   msg = "Stall on issue, no reservation station available.";

                            break;
        case E_WRITE_TO_R0: msg = "Command will write in the R0 register. Ignored.";

                            break;
        case E_DUPL_LABEL:  msg = "Duplicate label found during parsing of the assembly input.";

                            break;
        case E_INV_BR:      msg = "The Label of a Branch command could not be found in the assembly file.";

                            break;

        case E_NO_CDB_AVAIL:msg = "Not free cdb to broadcast.";

                            break;

        case E_DATA_HAZ:    msg = "Data Hazard.";

                            break;

        case E_FUNC_UN:     msg = "Not available functional unit.";

                            break;
        case E_GENERAL:     //falling through to the default message


        default:            msg = "An error was found (we might terminate the execution)...";
                            break;
    }

    fn[act](n, msg);
}
