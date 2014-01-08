#ifndef INSTRUCTION_SET_H_INCLUDED
#define INSTRUCTION_SET_H_INCLUDED

typedef enum{
    ADDD,
    SUBD,
    MULTD,
    DIVD,
    ADDI,
    BEQZ,
    SGTI,
    SD,
    LD
} cmdEnum;


enum cmdtype {
	RTYPE,
	ITYPE,
	BTYPE,	// NOTE: BTYPE is actually an ITYPE command (that does branching)
	JTYPE,  // no really needed, since our MIPS-like instrucion set lacks jump (J and JAL)
	BREAKPOINT
};

typedef struct {
    cmdEnum cmd;
	int  dstReg;
	int  src1Reg;
	int  src2Reg;
} Rinstr;

typedef struct {
    cmdEnum cmd;
    int  dstReg;
	int  srcReg;
	int  immediate;
} Iinstr;

typedef struct {
    cmdEnum cmd;
    int  dstReg;
	//int  srcReg;
	union {
		uint64_t label;
		void * branchp;		//TODO Change to something better
	} br;
} Binstr;

typedef struct {
    cmdEnum cmd;
	int  dstReg;
	int  address;
} Jinstr;

typedef struct {
    enum cmdtype type;
    bool isBreakpoint;
    union{
        Rinstr r;
        Iinstr i;
        Binstr b;
        Jinstr j;
    } instr;
} ASMinstr;


char * getCmdStr(cmdEnum );

#endif // INSTRUCTION_SET_H_INCLUDED
