#include "definitions.h"

Rreg* RFile;
Freg* FFile;

bool is_free_reg(int index)
{
    if(index < num_of_int_registers)
        return RFile[index].isFree;
    else
        if(index%2 == 0)
            return FFile[(index-num_of_int_registers)/2].isFree;
        else
            return FFile[(index-num_of_int_registers-1)/2].isFree;
}

void set_is_free_reg(int index, bool value)
{
    if(index < num_of_int_registers)
        RFile[index].isFree = value;
    else
        if((index-num_of_int_registers)%2 == 0)
            FFile[(index-num_of_int_registers)/2].isFree = value;
        else
            FFile[(index-num_of_int_registers-1)/2].isFree = value;
}

void set_rreg_value(int index, int value)
{
    if(index < num_of_int_registers)
        RFile[index].value = value;
    else
        if(index%2 == 0)
            FFile[(index-num_of_int_registers)/2].value.split.high = value;
        else
            FFile[(index-num_of_int_registers-1)/2].value.split.low = value;
}

void set_freg_value(int index, double value)
{
    FFile[(index-num_of_int_registers)/2].value.single = value;
}

/*
int * pointer_index_to_rreg_value(int index)
{
    int * result;
    if(index < num_of_int_registers)
        result = &RFile[index].value;
    else
        if(index%2 == 0)
            result = &FFile[(index-num_of_int_registers)/2].value.split.high;
        else
            result = &FFile[(index-num_of_int_registers-1)/2].value.split.low;
    return result;
}

int * pointer_index_to_rreg_value(int index)
{
    return * FFile[(index-num_of_int_registers)/2].value.single;
}
*/

int index_to_rreg_value(int index)
{
    return 0;
    int result;
    if(index < num_of_int_registers)
        result = RFile[index].value;
    else
        if(index%2 == 0)
            result = FFile[(index-num_of_int_registers)/2].value.split.high;
        else
            result = FFile[(index-num_of_int_registers-1)/2].value.split.low;
    return result;
}

double index_to_freg_value(int index)
{
    return 0;
    return FFile[(index-num_of_int_registers)/2].value.single;
}


void free_registers(ASMinstr instr)
{
    switch(instr.type)
    {
        case RTYPE:
            set_is_free_reg(instr.instr.r.dstReg, true);
            set_is_free_reg(instr.instr.r.src1Reg, true);
            set_is_free_reg(instr.instr.r.src2Reg, true);
            return;
        case ITYPE:
            set_is_free_reg(instr.instr.i.dstReg, true);
            set_is_free_reg(instr.instr.i.srcReg, true);
            return;
        case BTYPE:
            set_is_free_reg(instr.instr.b.dstReg, true);
            return;
    }
}


bool there_are_free_registers(ASMinstr instr)
{
    switch(instr.type)
    {
        case RTYPE:
            if (is_free_reg(instr.instr.r.dstReg) && is_free_reg(instr.instr.r.src1Reg) && is_free_reg(instr.instr.r.src2Reg))
                return true;
            break;
        case ITYPE:
            if (is_free_reg(instr.instr.i.dstReg) && is_free_reg(instr.instr.i.srcReg))
                return true;
            break;
        case BTYPE:
            if (is_free_reg(instr.instr.b.dstReg))
                return true;
            break;
    }
    //error(E_DATA_HAZ, ACTION_PRINTMSG);
    return false;
}



void acquire_registers(ASMinstr instr)  // TODO: See whether we must reset the regs in case of failure!
{
    switch(instr.type)
    {
        case RTYPE:
            set_is_free_reg(instr.instr.r.dstReg, false);
            set_is_free_reg(instr.instr.r.src1Reg, false);
            set_is_free_reg(instr.instr.r.src2Reg, false);
            break;
        case ITYPE:
            set_is_free_reg(instr.instr.i.dstReg, false);
            set_is_free_reg(instr.instr.i.srcReg, false);
            break;
        case BTYPE:
            set_is_free_reg(instr.instr.b.dstReg, false);
            break;
    }
    return;
}
