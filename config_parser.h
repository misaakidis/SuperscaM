#ifndef CFG_PARSER_H_INCLUDED
#define CFG_PARSER_H_INCLUDED


int         num_of_int_registers,
            num_of_float_registers,
            num_of_float_functional_units,
            num_of_int_functional_units,
            num_of_reservation_stations,
            num_of_simultaneous_cdb_broadcasts,
            ADDD_EX_cycles,
            MULTD_EX_CYCLES,
            LD_EX_cycles,
            SD_EX_cycles,
            int_instruction_cycles,
            num_of_instr_issued_per_cycle;

/*
extern bool bnum_of_int_registers,
            bnum_of_float_registers,
            bnum_of_float_functional_units,
            bnum_of_int_functional_units,
            bnum_of_reservation_stations,
            bnum_of_simultaneous_cdb_broadcasts,
            bADDD_EX_cycles,
            bMULTD_EX_CYCLES,
            bLD_EX_cycles,
            bSD_EX_cycles,
            bint_instruction_cycles,
            bnum_of_instr_issued_per_cycle;
*/

#endif // CFG_PARSER_H_INCLUDED
