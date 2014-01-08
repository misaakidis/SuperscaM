#include "definitions.h"
#include "info_list.h"


//                                      8  9 10 11 12 13 14 15
// entoli cmd dst src1 src2 | ......... I  E  E  E  S  S  S  W
/*
    leave space for a command
    printf("     |");
    for(i=0; i<curreny_cycle
        printf("%5d|", i)

    //foreach elem in infoprint
    printf %5sgetEnumStrcmd)
    for(i=0; i<cycle_issued; i++)
       printf("     |");
    printf"  I  "
*/



/** Using terminal "color codes" (control sequences) to set foreground and background colors.
    Not very portable, but pretty.

    Tested with the terminal emulator "rxvt-unicode", properly configured.

    To see the output correctly formatted, use the command "less -RS -~ -6 /full/path/to/file"
**/
    /*
    Black       0;30     Dark Gray     1;30
    Blue        0;34     Light Blue    1;34
    Green       0;32     Light Green   1;32
    Cyan        0;36     Light Cyan    1;36
    Red         0;31     Light Red     1;31
    Purple      0;35     Light Purple  1;35
    Brown       0;33     Yellow        1;33
    Light Gray  0;37     White         1;37
    */


static FILE* fp;    // to make setting colors easier


static inline
void setRed(void){
    fprintf(fp, "%c[%dm", 0x1B, 31);
}


static inline
void setLBLue(void){
    fprintf(fp, "%c[%dm", 0x1B, 34);
}


static inline
void setBlue(void){
    fprintf(fp, "%c[%dm", 0x1B, 34);
}


static inline
void setLPurple(void){
    fprintf(fp, "%c[%dm", 0x1B, 35);
}


static inline
void setCyan(void){
    fprintf(fp, "%c[%dm", 0x1B, 36);
}


static inline
void setLCyan(void){
    fprintf(fp, "%c[%dm", 0x1B, 36);
}


static inline
void setWhite(void){
    fprintf(fp, "%c[%dm", 0x1B, 37);
}


static inline
void setGreen(void){
    fprintf(fp, "%c[%dm", 0x1B, 32);
}


//static inline fprint_spaces(FILE *f)
//{
//
//}


void print_info_node(FILE *f, printinfo *p)
{
    fp = f;

    setGreen();
    fprintf(f, "%5s |", getCmdStr(p->cmd->instr.r.cmd));

    /*
    for(int i = 0; i < p.cycle_issued; ++i)
    //    fprintf(f, "%s", "          "[MAX_PRINTABLE_DIGITS]); //up to 10 printable spaces ;)
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "          ");
    fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "I  ");

    for(int i = 0; i < p.stalls_ex; ++i)
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "          ");
    fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "S  ");


    for(int i = 0; i < p.stalls_ex; ++i)
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "          ");
    for(int i = 0; i < get_instr_ex_cycles(p.cmd); ++i)
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "E  ");


    if (p.cmd.type != BEQZ){
        for(int i = 0; i < p.stalls_wb; ++i)
            fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "          ");
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "S  ");


        for(int i = 0; i < p.stalls_wb; ++i)
            fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "          ");
        fprintf(f, "%"MAX_PRINTABLE_DIGITS"s", "WB ");
    }
    */

    setBlue();
    for(int i = 1; i < p->cycle_issued; ++i)
        fprintf(f, "     ");
    fprintf(f, "%5s", "  I  ");

    setRed();
    for(int i = 0; i < p->stalls_ex; ++i)
        fprintf(f, "%5s", "  S  ");

    setLPurple();
    for(int i = 0; i < get_instr_ex_cycles(&p->cmd); ++i)
        fprintf(f, "%5s", "  E  ");

    setRed();
    if (p->cmd->type != BEQZ){
        for(int i = 0; i < p->stalls_wb; ++i)
            fprintf(f, "%5s", "  S  ");

        setLCyan();
        fprintf(f, "%5s", "  WB ");
    }


    fprintf(f, "\n");
}


  /* Function print_all() uses the address of func. print_info_node() as a callback for the
   * func. print_info_list(), which traverses the list recursively. */
void print_all(FILE *f, info_list *IL, int current_cycle)
{
    //assert(current_cycle <= MAX_PRINTABLE_CYCLES); // else our layout breaks!

    /* print header */
    fprintf(f, " Cmd  |"); //print 5 spaces
    for(int i = 1; i < current_cycle; ++i)
        fprintf(f, "%4d|", i);
    fprintf(f, "\n");

    /* print all info foreach instruction before a breakpoint or a terminated execution */
    print_info_list(f, IL, print_info_node);
}

