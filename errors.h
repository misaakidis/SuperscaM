#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED


/* return codes for abnormal situations...
   Defined as an enumeration and supposed to be returned to the shell. */

typedef enum {
	E_NO_PROBLEM,        // NO_PROBLEM value is 0 (normal exit)
	E_GENERAL     = -1,  // general error case
	E_INSUF_ARGS  = -2,  // insufficient arguments
	E_FILE_NOT_F  = -3,  // file not found (or could not be opened)
	E_XMALLOC     = -4,  // malloc failure
	E_JUMPINSTR   = -5,  // JUMP instruction found
	E_NO_BRANCHES = -6,  // input asm file has no branching instructions at all!
	E_CMDLINE_BUF = -7,  // cannot parse line, too large.
	E_CMD_INVALID = -8,  // invalid command line at the assembly input file
	E_CMD_REG_INV = -9,	 // invalid register in assembly command
	E_CFG_MISS    = -10, // missing line in configuration file
	E_CFG_REPEAT  = -11, // repeated declaration in configuration file
	E_ATOI        = -12, // cannot parse the given string to int value
	E_CFG_INV_VAL = -13, // invalid value at configuration file
	E_CFG_INV_LINE= -14, // invalid line in configuration file
    E_DIV_ZERO    = -15, // division with zero
    E_NO_RES_ST   = -16, // not available reservation station
    E_WRITE_TO_R0 = -17, // command's dstReg is the R0 register
    E_DUPL_LABEL  = -18, // duplicate label found during parsing
    E_INV_BR      = -19, // label could not be found in the dictionary
    E_NO_CDB_AVAIL= -20, // the cdb has already been acquired by other reservation stations
    E_DATA_HAZ    = -21, // found data hazard
    E_FUNC_UN     = -22  // not available functional units
} ERRORNUM;

typedef void (*ACTION)(ERRORNUM, char*);

// Possible actions (the array of function pointers in errors.c needs to be maintained by hand):
typedef enum {
    ACTION_PRINTMSG,
    ACTION_EXIT,
    ACTION_PRINTMSG_AND_EXIT
} ERRORACTION;

/** Prototypes of non-static functions: */
void error(ERRORNUM, ERRORACTION);  // process an error, also choosing the appropriate action


#endif /* ERRORS_H_INCLUDED */
