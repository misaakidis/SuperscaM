#include "definitions.h"

static int available_float_functional_units;
static int available_int_functional_units;

void initialize_functional_units()
{
    available_float_functional_units = num_of_float_functional_units;
    available_int_functional_units = num_of_int_functional_units;
}

bool there_are_free_functional_units(ASMinstr instr)
{
    if(isSDinstrE(instr.instr.r.cmd) || isLDinstrE(instr.instr.r.cmd) || isRRinstrE(instr.instr.r.cmd) || isBinstrE(instr.instr.r.cmd))
    {
        if(available_int_functional_units > 0)
        {
            return true;
        }
        error(E_FUNC_UN, ACTION_PRINTMSG);
        return false;
    }
    else
    {
    if(available_float_functional_units > 0)
        return true;
    }
    error(E_FUNC_UN, ACTION_PRINTMSG);
    return false;
}

void update_functional_units()
{
    //The floating point units are pipelined
    available_float_functional_units = num_of_float_functional_units;
}

bool get_functional_unit(ASMinstr instr)
{
    if(isSDinstrE(instr.instr.r.cmd) || isLDinstrE(instr.instr.r.cmd) || isRRinstrE(instr.instr.r.cmd) || isBinstrE(instr.instr.r.cmd))
    {
        if(available_int_functional_units > 0)
        {
            available_int_functional_units--;
            return true;
        }
        return false;
    }
    else
    {
        if(available_float_functional_units>0)
        {
            available_float_functional_units--;
            return true;
        }
    }
    return false;
}

void free_functional_unit(ASMinstr cmd)
{
    if(isSDinstrE(cmd.instr.r.cmd) || isLDinstrE(cmd.instr.r.cmd) || isRRinstrE(cmd.instr.r.cmd) || isBinstrE(cmd.instr.r.cmd))
        available_int_functional_units++;
    else
        available_float_functional_units++;
}
