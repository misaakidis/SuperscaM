#include "definitions.h"

static int remaining_broadcasts;

bool there_are_free_broadcasts()
{
    if (remaining_broadcasts > 0)
    {
        return true;
    }
    error(E_NO_CDB_AVAIL, ACTION_PRINTMSG);
    return false;
}

static
bool get_available_bus()
{
    if (remaining_broadcasts > 0)
    {
        remaining_broadcasts--;
        return true;
    }
    return false;
}

static
bool broadcast_result_float(int *freg, double value)
{
    if(get_available_bus())
    {
        //set_freg_value(*freg, value);
        return true;
    }
    return false;

}

static
bool broadcast_result_int(int *rreg, int value)
{
    if(get_available_bus())
    {
        //set_rreg_value(*rreg, value);
        return true;
    }
    return false;
}

bool broadcast_result(ASMinstr* instr, double result)
{
    bool wasBroadcasted = false;
    //fprintf(stderr, "Broadcasting result of %s to register %d\n", getCmdStr(instr->instr.r.cmd), instr->instr.r.dstReg);
    if( isRRinstrE(instr->instr.r.cmd) || isIinstrE(instr->instr.i.cmd) || isBinstrE(instr->instr.b.cmd) )
        wasBroadcasted = broadcast_result_int(&instr->instr.r.dstReg, (int) result);
    else
        wasBroadcasted = broadcast_result_float(&instr->instr.r.dstReg, result);

    return wasBroadcasted;
}


void update_cdb()
{
    remaining_broadcasts = num_of_simultaneous_cdb_broadcasts;
}
