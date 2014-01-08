//#include "parser.h"
#include "definitions.h"
#include "dictionary.h"
#include <math.h>


static char *previouslabel = NULL;  // global variable, useful for instructions after a (valid) label.
static node *node_mod;


static
void add_node_parser(list *L, ASMinstr value, dictionary *D) // a hack due to bad design!
{
    node_mod = add_node_and_get_address(L, value);
    fprintf(stderr, "%d ", node_mod);

    /*using a global variable; the following line will do nothing if previouslabel == NULL */
    add_to_dict(D, hash(previouslabel), node_mod);

    /* free(previouslabel); */          // DO NOT free previouslabel's space; dict only has a
                                        // pointer to that space, not a copy!

    previouslabel = NULL;               // add it only once!
}

/*
static
bool strIncludedInArray(char * string, char ** strArray)
{
	for(int i=0; i < strlen(strArray); i++)
	{
		if (strcmp(string, strArray[i]) == 0)
			return true;
	}
	return false;
}
*/


static
bool charIncludedInString(char c, char * string)
{
	for(uint_fast16_t i=0; i<strlen(string); i++)
	{
		if(c == string[i])
			return true;
	}
	return false;
}

char *xstrtok(char *line, char *delims)
{
	//Custom strtok function. Improvements:
	//1. Identifies consecutive delimiters
	//2. Does not modify the original string
	//3.
	static char *saveline = NULL;
	int n;

	if(line != NULL)
	{
		saveline = xmalloc(sizeof(char*));
		strcpy(saveline, line);
		int i = 0;
		while(charIncludedInString(saveline[i++], delims));
		saveline += i-1;
	}
	/*
	 *see if we have reached the end of the line
	 */
	if(saveline == NULL || *saveline == '\0')
		return(NULL);

	/*
	 *return the number of characters that aren't delims
	 */
	n = strcspn(saveline, delims);
	//p = saveline; /*save start of this token*/
	char p[n+1];
	strncpy(p, saveline, n);
	p[n] = '\0';
	saveline += n; /*bump past the delim*/

	if(*saveline != '\0') /*trash the delim if necessary*/
		*saveline++ = '\0';

	return(p);
}

int xatoi(char token[])
{
	int i = 1;
	int zerosInBegin = 0;
	int result = 0;
	while(token[zerosInBegin] == '0')
		{
			zerosInBegin++;
		}
	i += zerosInBegin;
	while(token[i]>='0' && token[i]<='9')
	{
		result += (token[i] - '0') * ((int) pow(10, i-1-zerosInBegin));
		i++;
	}
	if(token[i] != '\0' &&  (token[i]<'0' || token[i]>'9'))
		result = -1;
	return result;
}

/*
static
long xatoiImm(char* token)
{
    long returnLong;
    if(token[0] == '-')
    {
        returnLong = - atoi(token + 1);
    }
    else
        returnLong = atoi(token);
    return returnLong;
}
*/

int strToRegNum(char* token)
{
	int regNum = -1;
	char * sentToken = malloc(sizeof(char*));
	strcpy(sentToken, token);
	int regType = token[0];
	switch (regType)
	{
		case 'r':
		case 'R':
			regNum = xatoi(token);
			break;
		case 'f':
		case 'F':
			regNum = num_of_int_registers + xatoi(token);
			break;
		default:
			//error(E_CMD_REG_INV, ACTION_PRINTMSG);
			break;
	}
	return regNum;
}


static
int strToRRegNum(char* token)
{
	int returnReg = strToRegNum(token);
	//if(returnReg >= num_of_int_registers)
      //  returnReg -= num_of_int_registers;
	return returnReg;
}


static
int strToFRegNum(char* token)
{
	int returnReg = strToRegNum(token);
	if(returnReg <= num_of_int_registers)
        returnReg += num_of_int_registers;
	return returnReg;
}


static
void parseRinstr(char* commandLine, dictionary *D)
{
	Rinstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToFRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.src1Reg = strToFRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.src2Reg = strToFRegNum(token);
	if(cmd.src1Reg < num_of_int_registers || cmd.src2Reg < num_of_int_registers)
	{
	    fprintf(stderr, "%s: ", commandLine);
		fprintf(stderr, "Register invalid, not included in the FPR\n");
	}
	else
	{
		//TODO Put cmd in list
		ASMinstr a;
		a.type = RTYPE;
		a.isBreakpoint = false;
		a.instr.r = cmd;
		add_node_parser(L1, a, D);
		fprintf(stdout, "R-type command: %-6.6s %6d %6d %6d\n", getCmdStr(cmd.cmd), cmd.dstReg, cmd.src1Reg, cmd.src2Reg);
	}
}


static
void parseRRinstr(char* commandLine, dictionary *D)
{
	Rinstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToRRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.src1Reg = strToRRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.src2Reg = strToRRegNum(token);
	if(cmd.src1Reg == -1 || cmd.src2Reg == -1 || cmd.dstReg == -1)
	{
	    fprintf(stderr, "%s: ", commandLine);
		fprintf(stderr, "Register invalid, not included in the FPR\n");
	}
	else
	{
		//TODO Put cmd in list
		ASMinstr a;
		a.type = RTYPE;
		a.isBreakpoint = false;
		a.instr.r = cmd;
		add_node_parser(L1, a, D);
		fprintf(stdout, "R-type command: %-6.6s %6d %6d %6d\n", getCmdStr(cmd.cmd), cmd.dstReg, cmd.src1Reg, cmd.src2Reg);
	}
}


static
void parseIinstr(char* commandLine, dictionary *D)
{
	Iinstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToRRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.srcReg = strToRRegNum(token);
    if(cmd.srcReg == -1 || cmd.dstReg == -1)
	{
	    fprintf(stderr, "%s: ", commandLine);
     	error(E_CMD_REG_INV, ACTION_PRINTMSG);
     	return;
	}
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.immediate = atoi(token);
	if(cmd.immediate == 0)
	{
	    fprintf(stderr, "%s: ", commandLine);
	    fprintf(stderr, "Cannot parse the given string to int value\n");
	    return;
	}
    ASMinstr a;
	a.type = ITYPE;
	a.isBreakpoint = false;
	a.instr.i = cmd;
	add_node_parser(L1, a, D);
    fprintf(stdout, "I-type command: %-6.6s %6d %6d %6d\n", getCmdStr(cmd.cmd), cmd.dstReg, cmd.srcReg, cmd.immediate);

}

static
void parseBinstr(char* commandLine, dictionary *D)
{
	Binstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToRRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	//cmd.br.label = malloc(sizeof(uint64_t));
	cmd.br.label = hash(strcat(token, ":"));
	//xstrcpy(cmd.br.label, token);
	//for(uint_fast16_t i=0; i<strlen(token); i++)
    //    cmd.br.label[i] = token[i];
	//strncpy(cmd.br.label, token, strlen(token));
    //TODO Put cmd in list
    ASMinstr a;
	a.type = BTYPE;
	a.isBreakpoint = false;
	a.instr.b = cmd;
	add_node_parser(L1, a, D);
    fprintf(stdout, "B-type command: %-6.6s %6d %lu\n", getCmdStr(cmd.cmd), cmd.dstReg, cmd.br.label);

    add_node(L2, a); // also adding to the list of the 2nd pass
}


static
void parseSDinstr(char * commandLine, dictionary *D)
{
    Iinstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.immediate = atoi(token);
	if(cmd.immediate == 0)
	{
	    fprintf(stderr, "%s: ", commandLine);
        error(E_ATOI, ACTION_PRINTMSG);
	    return;
	}
    token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.srcReg = strToRRegNum(token);

	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToFRegNum(token);
    ASMinstr a;
	a.type = ITYPE;
	a.isBreakpoint = false;
	a.instr.i = cmd;
	add_node_parser(L1, a, D);
    fprintf(stdout, "I-type command: %-6.6s %6d %6d %6d\n", getCmdStr(cmd.cmd), cmd.immediate, cmd.srcReg, cmd.dstReg);

}


static
void parseLDinstr(char * commandLine, dictionary *D)
{
    Iinstr cmd;
	cmd.cmd = getCmdEnum(xstrtok(commandLine, cmd_delim));
	char * token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.dstReg = strToFRegNum(token);
	token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.immediate = atoi(token);
	if(cmd.immediate == 0)
	{
	    fprintf(stderr, "%s: ", commandLine);
	    error(E_ATOI, ACTION_PRINTMSG);
	    return;
	}
    token = xstrtok(NULL, cmd_delim);
	if(token == NULL)
	{
	    fprintf(stderr, "%s: ", commandLine);
		error(E_CMD_INVALID, ACTION_PRINTMSG);
		return;
	}
	cmd.srcReg = strToRRegNum(token);
    ASMinstr a;
	a.type = ITYPE;
	a.isBreakpoint = false;
	a.instr.i = cmd;
	add_node_parser(L1, a, D);
    fprintf(stdout, "I-type command: %-6.6s %6d %6d %6d\n", getCmdStr(cmd.cmd), cmd.dstReg, cmd.immediate, cmd.srcReg);
}


static
void splitCommandLine(char* commandLine, int currentLine, dictionary *D)
{
	//ASMinstr cmd;
	static consecutive_labels = 0;

	char * token = xstrtok(commandLine, cmd_delim);
	//if (strIncludedInArray(token, RtypeCommands))
	if (!strcmp(token, "breakpoint"))
	{
        ASMinstr a;
        a.type = BREAKPOINT;
        a.isBreakpoint = true;
        add_node(L1, a);
                    /* ?!?!?! MAYBE add_node(L1, a); is better
                       than add_node_parser(L1, a, D) for breakpoints; */
        //add_node_parser(L1, a, D);
        fprintf(stderr, "Found breakpoint: %s\n", commandLine);
        //When there is a breakpoint in a line, anything following will be considered as comment
        return;
        //token = xstrtok(NULL, cmd_delim);
        //if (!xstrcmp(token, "arloumpes"))
        //    return;
	}
	if (isRFinstr(token))
	{
		//cmd.type = RTYPE;
		//cmd.instr.r = parseRinstr(commandLine);
		parseRinstr(commandLine, D);
	}
	//else if (strIncludedInArray(token, ItypeCommands))
	else if (isIinstr(token))
	{
		//cmd.type = ITYPE;
		//cmd.instr.i = parseIinstr(commandLine);
		parseIinstr(commandLine, D);
	}
	else if (isBinstr(token))
	{
	    parseBinstr(commandLine, D);
	}
	else if (isSDinstr(token))
        parseSDinstr(commandLine, D);
    else if (isLDinstr(token))
        parseLDinstr(commandLine, D);
    else if (isRRinstr(token))
        parseRRinstr(commandLine, D);
	else
	{
		if(token[strlen(token)-1] == ':')
		{
		    //token = strncpy(token, token, strlen(token)-2);
		    //token[strlen(token)+1] = '\0';
			fprintf(stdout, "New Label: %s\n", token); //TODO Put label in dict

            /*
            if (previouslabel != NULL) //|| isFirstLabel
            {
                //isFirstLabel = false;
                add_to_dict(D, hash(token), node_mod);
                consecutive_labels++;
            }
            else
                while(consecutive_labels > 0)
                {
                    D->e[D->entries-consecutive_labels].goto_node = D->e[D->entries].goto_node;
                    consecutive_labels--;
                }
            */

			previouslabel = (char *)malloc(sizeof(char *));
			strcpy(previouslabel, token);
			token = xstrtok(commandLine, cmd_delim);
			char lampel[MAX_LABEL_LEN];
			strcpy(lampel, token);
            if(token[0] == '0' && token[1] == 'x')
            {
                long hexLabel = 0;
                hexLabel = strtol(lampel, NULL, 16);
                if(hexLabel > 0 && hexLabel < 65535)
                    fprintf(stderr, "The label is in hexadecimal format and within the byte addressing range.\n");
            }
            strcpy(lampel, token);
		}
		else
		{
		    fprintf(stderr, "%s: ", commandLine);
			error(E_CMD_INVALID, ACTION_PRINTMSG);
		}
	}

}


static
list *first_pass(FILE *asmfile, dictionary *D)
{

	char commandLine[COMMAND_LINE_BUFFER];
	int c;
	int charsRead;
	int currentLine = 0;

	do
	{
		charsRead = 0;
		currentLine++;
		c = fgetc(asmfile);
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
			c = fgetc(asmfile);
		}
		if (commandLine[0] != '\0')
		{
			splitCommandLine(commandLine, currentLine, D);
		}
		if (c == '#')
			while ((c = fgetc(asmfile))!= '\n' && c != EOF);
	}while(c != EOF);

    return L1;
}


static
void second_pass(list *L, dictionary *Dict)                      /** DEFINITELY NOT READY! **/
{
    if(L->head == NULL){
        error(E_NO_BRANCHES, ACTION_PRINTMSG);
        return;
    }

    //assert();     // I forgot what I wanted to assert here, but it was important!
/**/

    for(node *n = L2->head; n != NULL; n = n->next){
        assert (n->instr.type == BTYPE);
        /* which one of the following 2 lines will we implement? (I guess the first one.)
        For now, assign both: */
        node * retrieved_node = retrieve_from(Dict, n->instr.instr.b.br.label);
        if (retrieved_node == NULL)
            error(E_INV_BR, ACTION_PRINTMSG);
        n->branch = retrieved_node;
        n->instr.instr.b.br.branchp = retrieved_node;
    }

/**/
}


list *parse_file_to_list(FILE *file)
{
    L1 = create_list();
    dictionary *D = create_dict(MAX_LABEL_NUM);

    /*
    list *L2 = first_pass(L1, D, file); // 1.parse all instructions, remove junk, keep labels to
                                        //   a dictionary and save branching instructions to L2
               second_pass(L2, D);      // 2.parse branching instructions to fill their pointers

    assert(L1->size == sublist_size(L1->head));
    */

    /* Much uglier alternative due to static list declarations (It even ignores the list
       "returned" by the first_pass() function and obscures the control flow): */

    L2 = create_list();
    first_pass(file, D);
    second_pass(L2, D);

    //Print dictionary
    fprintf(stdout, "\n\nPrinting Labels from the dictionary.\n");
    fprintf(stdout, "There are %d entries:\n", D->entries);
    for(int i = 0; i < D->entries; ++i)
       fprintf(stdout, "Label: Hash Code %20lu. Next instruction @%d %5s %2d\n", D->e[i].hashcode, D->e[i].goto_node, getCmdStr(D->e[i].goto_node->instr.instr.r.cmd), D->e[i].goto_node->instr.instr.r.dstReg);
    free(D);

    fprintf(stderr,"\n\nStarting the issuing of commands:");
    return L1;
}
