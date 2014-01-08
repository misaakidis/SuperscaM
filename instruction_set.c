#include "definitions.h"

bool isRFinstr(char instr[])
{
	if(!strcmp(instr, "ADDD") || !strcmp(instr, "SUBD") ||!strcmp(instr, "MULTD") ||!strcmp(instr, "DIVD") )
		return true;
	return false;
}

bool isRRinstr(char instr[])
{
	if( !strcmp(instr, "SGTI") )
		return true;
	return false;
}

bool isSDinstr(char instr[])
{
	if(!strcmp(instr, "SD"))
		return true;
	return false;
}

bool isLDinstr(char instr[])
{
	if(!strcmp(instr, "LD"))
		return true;
	return false;
}

bool isIinstr(char instr[])
{
	if(!strcmp(instr, "ADDI"))
		return true;
	return false;
}

bool isBinstr(char instr[])
{
    if(!strcmp(instr, "BEQZ"))
        return true;
    return false;
}

bool isRFinstrE(cmdEnum instr)
{
	if((instr == ADDD) || (instr == SUBD) || (instr == MULTD) || (instr == DIVD) )
		return true;
	return false;
}

bool isRRinstrE(cmdEnum instr)
{
	if( instr == SGTI )
		return true;
	return false;
}

bool isSDinstrE(cmdEnum instr)
{
	if(instr == SD )
		return true;
	return false;
}

bool isLDinstrE(cmdEnum instr)
{
	if(instr == LD)
		return true;
	return false;
}

bool isIinstrE(cmdEnum instr)
{
	if(instr == ADDI)
		return true;
	return false;
}

bool isBinstrE(cmdEnum instr)
{
    if(instr == BEQZ)
        return true;
    return false;
}

char * getCmdStr(cmdEnum cmdenum)
{
    if(cmdenum == ADDD)
        return "ADDD";
    else if (cmdenum == SUBD)
        return "SUBD";
    else if (cmdenum == MULTD)
        return "MULTD";
    else if (cmdenum == DIVD)
        return "DIVD";
    else if (cmdenum == ADDI)
        return "ADDI";
    else if (cmdenum == BEQZ)
        return "BEQZ";
    else if (cmdenum == SGTI)
        return "SGTI";
    else if (cmdenum == SD)
        return "SD";
    else if (cmdenum == LD)
        return "LD";

    return "Undef";
}

cmdEnum getCmdEnum(char instr[])
{
    if(!strcmp(instr, "ADDD"))
        return ADDD;
    else if (!strcmp(instr, "SUBD"))
        return SUBD;
    else if (!strcmp(instr, "MULTD"))
        return MULTD;
    else if (!strcmp(instr, "DIVD"))
        return DIVD;
    else if (!strcmp(instr, "ADDI"))
        return ADDI;
    else if (!strcmp(instr, "BEQZ"))
        return BEQZ;
    else if (!strcmp(instr, "SGTI"))
        return SGTI;
    else if (!strcmp(instr, "SD"))
        return SD;
    else if (!strcmp(instr, "LD"))
        return LD;
}

