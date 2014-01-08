//#include "simulator.h"
#include "definitions.h"
#include "info_list.h"

void
print_register_values()
{
    for(int i=0; i<num_of_int_registers; i++)
        fprintf(stdout, "$R%2d = %d\n", i, RFile[i].value);
    for(int i=0; i<num_of_float_registers; i+=2)
        fprintf(stdout, "$F%2d = %f\n", i, FFile[i].value.single);
}

void
print_instruction_list(list* L)
{
    node *currentNode;
    currentNode = L->head;
    int currentCycle = 0;
    while (currentNode != NULL)
    {
        currentNode = currentNode->next;
        currentCycle++;
    }

}


static
void print_Rinstr(Rinstr assembly)
{
    printf("R-type instruction: %5s, dst: %2d, src1: %2d, src2: %2d\n",
           getCmdStr(assembly.cmd), assembly.dstReg, assembly.src1Reg, assembly.src2Reg);
}


static
void print_Iinstr(Iinstr assembly)
{
    printf("I-type instruction: %5s, dst: %2d, src:  %2d, immediate: %4d\n",
           getCmdStr(assembly.cmd), assembly.dstReg, assembly.srcReg, assembly.immediate);
}

static
void print_Binstr(Binstr assembly)
{
    printf("B-type instruction: %5s, dst: %2d, Hashed Label: %lu\n",
           getCmdStr(assembly.cmd), assembly.dstReg, assembly.br.label);
}

static
bool process_Rinstr(Rinstr assembly)
{
    print_Rinstr(assembly);
    return false; // always
}


static
bool process_Iinstr(Iinstr assembly)
{
    print_Iinstr(assembly);
    return false; // always
}


static
bool process_Binstr(Binstr assembly)
{
    print_Binstr(assembly);
    return false; // always
}


static
bool process_Jinstr(Jinstr assembly)
{
    /* Emit an error message; there are no J-type instructions in our instruction set (yet)! */
    error(E_JUMPINSTR, ACTION_PRINTMSG);
    return true; // ...or false
}

//static copy of info_list iL for breakpoint usage:
static iLcopy = NULL;

static
bool process_Breakpoint()
{
    fprintf(stderr, "Breakpoint!\n");
    print_register_values();
    for(int i=0; i<num_of_reservation_stations; i++)
    {
        //if (ResStations[i].isFree)                                                                 //Segmentatio fault in fprintf
            //fprintf(stderr, "Reservation Station: Command-%s ExecutionCycle-%d IsWaiting-%d \n", ResStations[i].instr.r.cmd, ResStations[i].ex_cycles_executed, ResStations[i].isWaiting);

        /* print iLcopy here?! */

    }

    getchar();

    return false;
}


int process_ASMinstr(ASMinstr assembly)
{
    bool branch_taken;

    switch (assembly.type) {  // assembly.type is an enumeration
        case RTYPE: branch_taken = process_Rinstr(assembly.instr.r);
                    break;
        case ITYPE: branch_taken = process_Iinstr(assembly.instr.i);
                    break;
        case BTYPE: branch_taken = process_Binstr(assembly.instr.b);
                    break;
        case JTYPE: branch_taken = process_Jinstr(assembly.instr.j);
                    break;
        case BREAKPOINT: branch_taken = process_Breakpoint();
                    break;
    }

    return branch_taken;
}

double execute_ASMinstr(ASMinstr *assembly)
{
    fprintf(stderr, "Completed EXecution of %s\n", getCmdStr(assembly->instr.r.cmd));
    if(assembly->instr.b.cmd != BEQZ && assembly->instr.r.dstReg == 0)
    {
        error(E_WRITE_TO_R0, ACTION_PRINTMSG);
        return 0;
    }
    if(assembly->instr.r.cmd == ADDD)
        return index_to_freg_value(assembly->instr.r.src1Reg) + index_to_freg_value(assembly->instr.r.src2Reg);
    else if(assembly->instr.r.cmd == SUBD)
        return index_to_freg_value(assembly->instr.r.src1Reg) - index_to_freg_value(assembly->instr.r.src2Reg);
    else if(assembly->instr.r.cmd == MULTD)
        return index_to_freg_value(assembly->instr.r.src1Reg) * index_to_freg_value(assembly->instr.r.src2Reg);
    else if(assembly->instr.r.cmd == DIVD)
    {
        if(index_to_freg_value(assembly->instr.r.src2Reg) <= 0.001 && index_to_freg_value(assembly->instr.r.src2Reg) >= -0.001)
        {
            fprintf(stderr, "\n\nMy drill is the drill that will pierce the heavens!\n");
            error(E_DIV_ZERO, ACTION_PRINTMSG_AND_EXIT);
        }
        return index_to_freg_value(assembly->instr.r.src1Reg) / index_to_freg_value(assembly->instr.r.src2Reg);
    }
    else if(assembly->instr.i.cmd == ADDI)
        return index_to_rreg_value(assembly->instr.i.srcReg) + assembly->instr.i.immediate;
    else if(assembly->instr.r.cmd == SGTI)
    {
        if (index_to_rreg_value(assembly->instr.r.src1Reg) > index_to_rreg_value(assembly->instr.r.src2Reg))
            return 1;
        else
            return 0;
    }
    else if(assembly->instr.i.cmd == LD)//TODO What about here?
        return (double) (index_to_freg_value(assembly->instr.i.immediate) + index_to_rreg_value(assembly->instr.i.srcReg));
    else if(assembly->instr.i.cmd == LD)
        return 0; //TODO AND HERE?
    else if(assembly->instr.b.cmd == BEQZ)
    {
        if (assembly->instr.b.dstReg == 0)
            return true;
        else
            return false;
    }

    return -1;
}

char* print_ASMinstr(ASMinstr* assembly)
{
    bool branch_taken;

    switch (assembly->type) {  // assembly.type is an enumeration
        case RTYPE: branch_taken = process_Rinstr(assembly->instr.r);
                    break;
        case ITYPE: branch_taken = process_Iinstr(assembly->instr.i);
                    break;
        case BTYPE: branch_taken = process_Binstr(assembly->instr.b);
                    break;
        case JTYPE: branch_taken = process_Jinstr(assembly->instr.j);
                    break;
        case BREAKPOINT: branch_taken = process_Breakpoint();
                    break;
    }
}

int get_instr_ex_cycles(ASMinstr* assembly)
{
    int cycles_remaining = 0;
    switch (assembly->instr.r.cmd)
    {
        case ADDD:
        case SUBD:
            cycles_remaining = ADDD_EX_cycles;
            break;
        case MULTD:
            cycles_remaining = MULTD_EX_CYCLES;
            break;
        case LD:
            cycles_remaining = LD_EX_cycles;
            break;
        case SD:
            cycles_remaining = SD_EX_cycles;
            break;
        default:
            cycles_remaining = int_instruction_cycles;
    }

    return cycles_remaining;
}


info_list *execute_list_loop(list *L)
{
    info_list *iL = create_info_list();

    initialize_functional_units();
    initializeResStations();
    update_cdb();

    node *currentNode= L->head;
    printinfo *printInfo = NULL;
    int currentCycle = 0;

    while (currentNode != NULL || res_station_is_executing())
    {
        fprintf(stderr, "\nCycle %d\n", ++currentCycle);
        //Start issuing of commands
        for(int j=0; j < num_of_instr_issued_per_cycle && currentNode != NULL; j++)
        {
            if(there_are_free_res_stations())
            {
                //add_info_node_recursive(infoList, currentNode->instr);
                Res_Station* givenResStation = get_free_res_station();
                memcpy(&givenResStation->cmd, &currentNode->instr, sizeof(ASMinstr));
                givenResStation->cycles_remaining = get_instr_ex_cycles(&givenResStation->cmd);
                print_ASMinstr(&givenResStation->cmd);
                givenResStation->isWaiting = true;
                givenResStation->isIssuedInTheSameCycle = true;
                givenResStation->issuedOnCycle = currentCycle;
                givenResStation->stalls_before_ex = 0;
                givenResStation->stalls_before_wb = 0;

                /* is this the proper place?! */
                //create_info_molecule(ASMinstr cmd, int cycle_issued, int stalls_ex, int stalls_wb)));
                printInfo = create_printinfo(&currentNode->instr, currentCycle, 0, 0);
                //fprintf(stderr, "%s\n", getCmdStr(printInfo->cmd->instr.r.cmd));
                add_info_node(iL, printInfo);

                givenResStation->printInfo = printInfo;
                givenResStation->printInfo->stalls_ex = 0;
                givenResStation->printInfo->stalls_wb = 0;

                currentNode = currentNode->next;
            }
            else
                error(E_NO_RES_ST, ACTION_PRINTMSG);

        }

        update_res_stations();
        update_cdb();

    }
    //fprintf(stdout, "\n================================================\n");
    //fprintf(stdout, "The execution was completed in %3d clock cycles.\n", currentCycle);
    //fprintf(stdout, "================================================\n");
    fprintf(stdout, "\n%c[%dm================================================\n", 0x1B, 34); // pretty, but not very portable
    fprintf(stdout, "The execution was completed in %c[%dm%3d%c[%dm clock cycles.\n", 0x1B, 34, currentCycle, 0x1B, 34);
    fprintf(stdout, "================================================\n");

    FILE *outfile = fopen("/tmp/huge-and-totally-random-filename.txt", "wr");
    print_all(outfile, iL, currentCycle);

    return iL;
}


void process_list(list *L)
{
    typedef void (*f)(ASMinstr);
    consume_list(L, (f) process_ASMinstr); // casting to (hopefully) compatible function pointer
}


void process_list_following_branches(list *L)
{
    execute_list(L, process_ASMinstr);
}
