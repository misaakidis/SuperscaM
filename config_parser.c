#include "definitions.h"
#include <math.h>

/*int num_of_int_registers,
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
    */

bool bnum_of_int_registers,
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

static
void initializeCfgFlags()
{
    bnum_of_int_registers = false;
    bnum_of_float_registers = false;
    bnum_of_float_functional_units = false;
    bnum_of_int_functional_units = false;
    bnum_of_reservation_stations = false;
    bnum_of_simultaneous_cdb_broadcasts = false;
    bADDD_EX_cycles = false;
    bMULTD_EX_CYCLES = false;
    bLD_EX_cycles = false;
    bSD_EX_cycles = false;
    bint_instruction_cycles = false;
    bnum_of_instr_issued_per_cycle = false;
}

bool xstrcmp(char* str1, char* str2)
{
    if(str1 == NULL || str2 == NULL)
        return false;
    if(strlen(str1) != strlen(str2))
        return true;

    for(int i=0; i<strlen(str1); i++)
    {
        if(str1[i] != str2[i])
            return true;
    }

    return false;
}

void xstrcpy(char* dest_string, char* orig_string)
{
    int i = 0;
    while(orig_string[i] != '\0')
    {
        i++;
    }
    dest_string = malloc(sizeof(char) * i);
    for(int j=0; j<=i; j++)
        dest_string[j] = orig_string[j];
}


static
int xatoiCfg(char token[])
{
	int i = 0;
	int zerosInBegin = 0;
	int result = 0;
	while(token[zerosInBegin] == '0')
		{
			zerosInBegin++;
		}
	i += zerosInBegin;
	char simplestr[5];
	strcpy(simplestr, token);
	while(token[i]>='0' && token[i]<='9')
	{
		result = (token[i] - '0') + result * 10;
		i++;
		strcpy(token, simplestr);
	}
	if(token[i] != '\0' &&  (token[i]<'0' || token[i]>'9'))
		result = -1;
	return result;
}

static
double xatofCfg(char token[])
{
	int i = 0;
	int zerosInBegin = 0;
	double result = 0;
	while(token[zerosInBegin] == '0')
		{
			zerosInBegin++;
		}
	i += zerosInBegin;
	char simplestr[5];
	strcpy(simplestr, token);
	while(token[i]>='0' && token[i]<='9')
	{
		result = (token[i] - '0') + result * 10;
		i++;
		strcpy(token, simplestr);
	}
	if(token[i]=='.')
	{
	    i++;
	    int smallFactor = -1;
        while(token[i]>='0' && token[i]<='9')
        {
        result += (token[i] - '0') * (float) pow(10, smallFactor--);
        i++;
        strcpy(token, simplestr);
        }
	}
	if(token[i] != '\0' &&  (token[i]<'0' || token[i]>'9'))
		result = -1;
	return result;
}

static
void splitCommandLineCfg(char* commandLine, int currentLine)
{
    char * token = xstrtok(commandLine, cfg_delim);
    if(!xstrcmp(token, "num_of_int_registers"))
    {
        if (bnum_of_int_registers)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_int_registers = xatoiCfg(token);
        if (num_of_int_registers == 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_int_registers <= 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        RFile = malloc(sizeof(Rreg) * num_of_int_registers);
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_int_registers = true;
    }
    else if(!xstrcmp(token, "num_of_float_registers"))
    {
        if (bnum_of_float_registers)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_float_registers = xatoiCfg(token);
        if (num_of_float_registers == 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_float_registers <= 0 || num_of_float_registers%2 != 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        FFile = malloc(sizeof(Freg) * (num_of_float_registers/2));
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_float_registers = true;
    }
    else if(!xstrcmp(token, "num_of_float_functional_units"))
    {
        if (bnum_of_float_functional_units)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_float_functional_units = xatoiCfg(token);
        if (num_of_float_functional_units== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_float_functional_units <= 0 || num_of_float_functional_units > 256)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_float_functional_units = true;
    }
    else if(!xstrcmp(token, "num_of_int_functional_units"))
    {
        if (bnum_of_int_functional_units)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_int_functional_units = xatoiCfg(token);
        if (num_of_int_functional_units== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_int_functional_units != 1)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_int_functional_units = true;
    }
    else if(!xstrcmp(token, "num_of_reservation_stations"))
    {
        if (bnum_of_reservation_stations)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_reservation_stations = xatoiCfg(token);
        if (num_of_reservation_stations== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_reservation_stations <=0 || num_of_reservation_stations > 128)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        ResStations = malloc(sizeof(Res_Station *) * num_of_reservation_stations);
        initializeResStations();
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_reservation_stations = true;
    }
    else if(!xstrcmp(token, "num_of_simultaneous_cdb_broadcasts"))
    {
        if (bnum_of_simultaneous_cdb_broadcasts)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_simultaneous_cdb_broadcasts = xatoiCfg(token);
        if (num_of_simultaneous_cdb_broadcasts== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_simultaneous_cdb_broadcasts <=0 || num_of_simultaneous_cdb_broadcasts > 16)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_simultaneous_cdb_broadcasts = true;
    }
    else if(!xstrcmp(token, "ADDD_EX_cycles"))
    {
        if (bADDD_EX_cycles)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        ADDD_EX_cycles = xatoiCfg(token);
        if (ADDD_EX_cycles== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (ADDD_EX_cycles <=0 || ADDD_EX_cycles > 5)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bADDD_EX_cycles = true;
    }
    else if(!xstrcmp(token, "num_of_simultaneous_cdb_broadcasts"))
    {
        if (bnum_of_simultaneous_cdb_broadcasts)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_simultaneous_cdb_broadcasts = xatoiCfg(token);
        if (num_of_simultaneous_cdb_broadcasts== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_simultaneous_cdb_broadcasts <=0 || num_of_simultaneous_cdb_broadcasts > 16)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_simultaneous_cdb_broadcasts = true;
    }
    else if(!xstrcmp(token, "MULTD_EX_CYCLES"))
    {
        if (bMULTD_EX_CYCLES)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        MULTD_EX_CYCLES = xatoiCfg(token);
        if (MULTD_EX_CYCLES== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (MULTD_EX_CYCLES <=0 || MULTD_EX_CYCLES > 10)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bMULTD_EX_CYCLES = true;
    }
    else if(!xstrcmp(token, "LD_EX_cycles"))
    {
        if (bLD_EX_cycles)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        LD_EX_cycles = xatoiCfg(token);
        if (LD_EX_cycles== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (LD_EX_cycles <=0 || LD_EX_cycles > 3)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bLD_EX_cycles = true;
    }
    else if(!xstrcmp(token, "SD_EX_cycles"))
    {
        if (bSD_EX_cycles)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        SD_EX_cycles = xatoiCfg(token);
        if (SD_EX_cycles== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (SD_EX_cycles <=0 || SD_EX_cycles > 3)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bSD_EX_cycles = true;
    }
    else if(!xstrcmp(token, "int_instruction_cycles"))
    {
        if (bint_instruction_cycles)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        int_instruction_cycles = xatoiCfg(token);
        if (int_instruction_cycles== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (int_instruction_cycles != 1)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bint_instruction_cycles = true;
    }
    else if(!xstrcmp(token, "num_of_instr_issued_per_cycle"))
    {
        if (bnum_of_instr_issued_per_cycle)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_REPEAT, ACTION_PRINTMSG);
        }
        token = xstrtok(NULL, cfg_delim);
        num_of_instr_issued_per_cycle = xatoiCfg(token);
        if (num_of_instr_issued_per_cycle== 0)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_ATOI, ACTION_PRINTMSG_AND_EXIT);
        }
        if (num_of_instr_issued_per_cycle < 1)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_VAL, ACTION_PRINTMSG_AND_EXIT);
        }
        fprintf(stderr, "Configured Line %3d: %s\n", currentLine, commandLine);
        bnum_of_instr_issued_per_cycle = true;
    }
    else
    {
        char samplestr[10];
        strcpy(samplestr, token);
        int regNum = strToRegNum(samplestr);
        if(regNum<=0 || regNum > num_of_int_registers + num_of_float_registers)
        {
            fprintf(stderr, "Line %d: %s\n", currentLine, commandLine);
            error(E_CFG_INV_LINE, ACTION_PRINTMSG);
            return;
        }
        token = xstrtok(NULL, cfg_delim);
        strcpy(samplestr, token);
        if(regNum <= num_of_int_registers)
            RFile[regNum].value = xatoiCfg(token);
        else
        {
            regNum -= num_of_int_registers;
            if(regNum % 2 == 0)
            {
                double result = xatofCfg(token);
                FFile[regNum].value.single = result;
            }
            else
                FFile[regNum - 1].value.split.low = xatoiCfg(token);
        }
    }

    return;
}


void parse_cfg_file(FILE *cfgfile)
{
    initializeCfgFlags();

	char commandLine[COMMAND_LINE_BUFFER];
	int c;
	int charsRead;
	int currentLine = 0;

	do
	{
		charsRead = 0;
		currentLine++;
		c = fgetc(cfgfile);
		commandLine[0] = '\0';
		while(c != '\n' && c != '#' && c != EOF)
		{
			if (charsRead++ > COMMAND_LINE_BUFFER)
			{
			    fprintf(stderr, "%s : ", commandLine);
				error(E_CMDLINE_BUF, ACTION_PRINTMSG);
				break;
			}
			commandLine[charsRead-1] = c;
			commandLine[charsRead] = '\0';
			c = fgetc(cfgfile);
		}
		if (commandLine[0] != '\0')
		{
			splitCommandLineCfg(commandLine, currentLine);
		}
		if (c == '#')
			while ((c = fgetc(cfgfile))!= '\n' && c != EOF);
	}while(c != EOF);

    //Asserting that all essential configuration options have been parsed correctly
    if (! (bnum_of_int_registers && bnum_of_float_registers && bnum_of_float_functional_units && bnum_of_int_functional_units && bnum_of_reservation_stations && bnum_of_simultaneous_cdb_broadcasts &&bADDD_EX_cycles && bMULTD_EX_CYCLES && bLD_EX_cycles && bSD_EX_cycles && bint_instruction_cycles && bnum_of_instr_issued_per_cycle ))
        error(E_CFG_MISS, ACTION_PRINTMSG_AND_EXIT);

    fprintf(stderr, "\n\n");
    return;
}

void print_cfg_options()
{
    fprintf(stdout, "Parsing configure file finished:\n");
    fprintf(stdout, "[registers]\n");
    fprintf(stdout, "num_of_int_registers = %d\n", num_of_int_registers);
    fprintf(stdout, "num_of_float_registers = %d\n", num_of_float_registers);
    fprintf(stdout, "[functional design]\n");
    fprintf(stdout, "num_of_float_functional_units = %d:\n", num_of_float_functional_units);
    fprintf(stdout, "num_of_int_functional_units = %d:\n", num_of_int_functional_units);
    fprintf(stdout, "num_of_reservation_stations = %d:\n", num_of_reservation_stations);
    fprintf(stdout, "num_of_simultaneous_cdb_broadcasts = %d:\n", num_of_simultaneous_cdb_broadcasts);
    fprintf(stdout, "[instructions]\n");
    fprintf(stdout, "ADDD_EX_cycles = %d:\n", ADDD_EX_cycles);
    fprintf(stdout, "MULTD_EX_CYCLES = %d:\n", MULTD_EX_CYCLES);
    fprintf(stdout, "LD_EX_cycles = %d:\n", LD_EX_cycles);
    fprintf(stdout, "SD_EX_cycles = %d:\n", SD_EX_cycles);
    fprintf(stdout, "int_instruction_cycles = %d:\n", int_instruction_cycles);
    fprintf(stdout, "[superscalar]\n");
    fprintf(stdout, "num_of_instr_issued_per_cycle = %d:\n", num_of_instr_issued_per_cycle);
    fprintf(stdout, "[register file]\n");
    print_register_values();
}
